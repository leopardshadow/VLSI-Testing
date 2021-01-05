#include <iostream>
#include <vector>
#include "circuit.h"
#include "gate.h"
#include "bridgingFaults.h"
#include "GetLongOpt.h"

extern GetLongOpt option;

using namespace std;

void CIRCUIT::GenerateBridgingFaults() {

    // Gate* glist;
    vector<GATE*> lastGateAtLevelN;
    for(unsigned i=0; i<=MaxLevel; i++) {
        lastGateAtLevelN.push_back(nullptr);
    }

    for(unsigned i=0; i<No_Gate(); i++) {
        GATE *g2 ( Gate(i) );
        GATE *g1 ( lastGateAtLevelN[g2->GetLevel()] );

        if(g1!=nullptr && g2->GetFunction()!=G_PO) {
            BFlist.push_back(new BRIDGING_FAULT(AND, g1, g2));
            BFlist.push_back(new BRIDGING_FAULT(OR , g1, g2));
        }

        lastGateAtLevelN[g2->GetLevel()] = g2;
    }

    // copy fault list to undetected for fault simulation !!!!!
    UBFlist = BFlist;

}



void CIRCUIT::PrintBridgingFaults(string f) {

    ofstream fout;
    fout.open(f, ios::out);
    for(unsigned i=0; i<BFlist.size() ; i++) {
        fout << BFlist[i]->output() << endl;
    }
}



// fault simulation test patterns
void CIRCUIT::BFaultSimVectors()
{
    cout << "Run bridging fault simulation" << endl;
    unsigned pattern_num(0);
    if(!Pattern.eof()){ // Readin the first vector
        while(!Pattern.eof()){
            ++pattern_num;
            Pattern.ReadNextPattern();
            //fault-free simulation
            SchedulePI();
            LogicSim();
            //single pattern parallel (bridging) fault simulation
            BFaultSim();
        }
    }

    //compute fault coverage
    unsigned total_num(0);
    unsigned undetected_num(0), detected_num(0);
    unsigned eqv_undetected_num(0), eqv_detected_num(0);
    BRIDGING_FAULT* fptr;
    vector<BRIDGING_FAULT*>::iterator fite;
    for (fite = BFlist.begin();fite!=BFlist.end();++fite) {
        fptr = *fite;
        switch (fptr->GetStatus()) {
            case DETECTED:
                ++eqv_detected_num;
                detected_num += fptr->GetEqvFaultNum();
                break;
            default:
                ++eqv_undetected_num;
                undetected_num += fptr->GetEqvFaultNum();
                break;
        }
    }
    total_num = detected_num + undetected_num;
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "---------------------------------------" << endl;
    cout << "Test pattern number = " << pattern_num << endl;
    cout << "---------------------------------------" << endl;
    cout << "Total fault number = " << total_num << endl;
    cout << "Detected fault number = " << detected_num << endl;
    cout << "Undetected fault number = " << undetected_num << endl;
    cout << "---------------------------------------" << endl;
    cout << "Equivalent fault number = " << BFlist.size() << endl;
    cout << "Equivalent detected fault number = " << eqv_detected_num << endl; 
    cout << "Equivalent undetected fault number = " << eqv_undetected_num << endl; 
    cout << "---------------------------------------" << endl;
    cout << "Fault Coverge = " << 100*detected_num/double(total_num) << "%" << endl;
    cout << "Equivalent FC = " << 100*eqv_detected_num/double(BFlist.size()) << "%" << endl;
    cout << "---------------------------------------" << endl;
    return;
}



//single pattern parallel (bridging) fault simulation
//parallel fault number is defined by PatternNum in typeemu.h
void CIRCUIT::BFaultSim()
{
    register unsigned i, fault_idx(0);
    GATEPTR gptr;
    BRIDGING_FAULT *fptr;
    BRIDGING_FAULT *simulate_bflist[PatternNum];
    list<GATEPTR>::iterator gite;
    //initial all gates
    for (i = 0; i < Netlist.size(); ++i) {
        Netlist[i]->SetFaultFreeValue();
    }

    // cout << "BFs " << UBFlist.size() << " " << BFlist.size() << endl;

    //for all undetected faults
    vector<BRIDGING_FAULT*>::iterator fite;
    for (fite = UBFlist.begin();fite!=UBFlist.end();++fite) {
        fptr = *fite;
        //skip redundant and detected faults
        if (fptr->GetStatus() == REDUNDANT || fptr->GetStatus() == DETECTED) { continue; }
        
        //the fault is not active
        if (fptr->GetInputGate1()->GetValue() == fptr->GetInputGate2()->GetValue()) { continue; }
        
        // now the fault is activated
        if(fptr->getFaultType() == AND) {
            if(fptr->GetInputGate1()->GetValue() == S1) {
                gptr = fptr->GetInputGate1();
            }
            if(fptr->GetInputGate2()->GetValue() == S1) {
                gptr = fptr->GetInputGate2();
            }

        }
        if(fptr->getFaultType() == OR) {
            if(fptr->GetInputGate1()->GetValue() == S0) {
                gptr = fptr->GetInputGate1();
            }
            if(fptr->GetInputGate2()->GetValue() == S0) {
                gptr = fptr->GetInputGate2();
            }
        }
 

        if (gptr->GetFlag(OUTPUT)) {
            fptr->SetStatus(DETECTED);
            continue;
        }
        if (!gptr->GetFlag(FAULTY)) {
            gptr->SetFlag(FAULTY); GateStack.push_front(gptr);
        }
        // add the fault to the simulated list and inject it
        InjectFaultValue(gptr, fault_idx, fptr->GetValue());
        gptr->SetFlag(FAULT_INJECTED);
        ScheduleFanout(gptr);
        simulate_bflist[fault_idx++] = fptr;


        //collect PatternNum fault, do fault simulation
        if (fault_idx == PatternNum) {
            //do parallel fault simulation
            for (i = 0;i<= MaxLevel; ++i) {
                while (!Queue[i].empty()) {
                    gptr = Queue[i].front(); Queue[i].pop_front();
                    gptr->ResetFlag(SCHEDULED);
                    FaultSimEvaluate(gptr);
                }
            }

            // check detection and reset wires' faulty values
            // back to fault-free values
            for (gite = GateStack.begin();gite != GateStack.end();++gite) {
                gptr = *gite;
                //clean flags
                gptr->ResetFlag(FAULTY);
                gptr->ResetFlag(FAULT_INJECTED);
                gptr->ResetFaultFlag();
                if (gptr->GetFlag(OUTPUT)) {
                    for (i = 0; i < fault_idx; ++i) {
                        if (simulate_bflist[i]->GetStatus() == DETECTED) { continue; }
                        //faulty value != fault-free value && fault-free != X &&
                        //faulty value != X (WireValue1[i] == WireValue2[i])
                        if (gptr->GetValue() != VALUE(gptr->GetValue1(i)) && gptr->GetValue() != X 
                                && gptr->GetValue1(i) == gptr->GetValue2(i)) {
                            simulate_bflist[i]->SetStatus(DETECTED);
                        }
                    }
                }
                gptr->SetFaultFreeValue();    
            } //end for GateStack
            GateStack.clear();
            fault_idx = 0;
        } //end fault simulation
    } //end for all undetected faults

    //fault sim rest faults
    if (fault_idx) {
        //do parallel fault simulation
        for (i = 0;i<= MaxLevel; ++i) {
            while (!Queue[i].empty()) {
                gptr = Queue[i].front(); Queue[i].pop_front();
                gptr->ResetFlag(SCHEDULED);
                FaultSimEvaluate(gptr);
            }
        }

        // check detection and reset wires' faulty values
        // back to fault-free values
        for (gite = GateStack.begin();gite != GateStack.end();++gite) {
            gptr = *gite;
            //clean flags
            gptr->ResetFlag(FAULTY);
            gptr->ResetFlag(FAULT_INJECTED);
            gptr->ResetFaultFlag();
            if (gptr->GetFlag(OUTPUT)) {
                for (i = 0; i < fault_idx; ++i) {
                    if (simulate_bflist[i]->GetStatus() == DETECTED) { continue; }
                    //faulty value != fault-free value && fault-free != X &&
                    //faulty value != X (WireValue1[i] == WireValue2[i])
                    if (gptr->GetValue() != VALUE(gptr->GetValue1(i)) && gptr->GetValue() != X 
                            && gptr->GetValue1(i) == gptr->GetValue2(i)) {
                        simulate_bflist[i]->SetStatus(DETECTED);
                    }
                }
            }
            gptr->SetFaultFreeValue();    
        } //end for GateStack
        GateStack.clear();
        fault_idx = 0;
    } //end fault simulation



    // // remove detected faults
    // for (fite = UBFlist.begin();fite != UBFlist.end();) {
    //     fptr = *fite;
    //     if (fptr->GetStatus() == DETECTED || fptr->GetStatus() == REDUNDANT) {
    //         // cout << fptr->output();
    //         fite = UBFlist.erase(fite);
    //     }
    //     else { ++fite; }
    // }

    // cout << "\n";
    // cout << "BFs " << UBFlist.size() << " " << BFlist.size() << "\n=====\n\n\n";


    return;
}





void CIRCUIT::BridgingAtpg() {

    unsigned i, total_backtrack_num(0), pattern_num(0);
    ATPG_STATUS status;
    FAULT* fptr;
    ofstream OutputStrm;

    vector<BRIDGING_FAULT*>::iterator fite;

    //Prepare the output files
    if (option.retrieve("output")){
        OutputStrm.open((char*)option.retrieve("output"),ios::out);
        if(!OutputStrm){
              cout << "Unable to open output file: "
                   << option.retrieve("output") << endl;
              cout << "Unsaved output!\n";
              exit(-1);
        }
    }

    if (option.retrieve("output")){
	    for (i = 0;i<PIlist.size();++i) {
		OutputStrm << "PI " << PIlist[i]->GetName() << " ";
	    }
	    OutputStrm << endl;
    }


    for (fite = BFlist.begin();fite!=BFlist.end();++fite) {


        fptr = *fite;

        if (fptr->GetStatus() == DETECTED) { 
            cout << ((BRIDGING_FAULT*)fptr)->output() << "has been detected\n\n";
            continue; }
        
        cout << "proc: " << ((BRIDGING_FAULT*)fptr)->output() << endl;


        VALUE fv, ifv;
        if((*fite)->getFaultType() == AND) {
            fv = S0;
            ifv = S1;
        }
        else {
            fv = S1;
            ifv = S0;
        }

        fptr = new FAULT((*fite)->GetInputGate1(), (*fite)->GetInputGate1(), fv);


        //run podem algorithm
        status = BridgingPodem(fptr, total_backtrack_num, (*fite)->GetInputGate2(), fv);
        if (status == TRUE) {
            fptr->SetStatus(DETECTED);
            ++pattern_num;
            //run fault simulation for fault dropping
            for (i = 0;i < PIlist.size();++i) { 
                ScheduleFanout(PIlist[i]); 
                if (option.retrieve("output")){ OutputStrm << PIlist[i]->GetValue();}
            }
            if (option.retrieve("output")){ OutputStrm << endl;}
            for (i = PIlist.size();i<Netlist.size();++i) { Netlist[i]->SetValue(X); }
            LogicSim();
            BFaultSim();
        }
        else {



            fptr = new FAULT((*fite)->GetInputGate2(), (*fite)->GetInputGate2(), fv);

            status = BridgingPodem(fptr, total_backtrack_num, (*fite)->GetInputGate1(), fv);

            // status = Podem(fptr, total_backtrack_num);


            switch (status) {
                case TRUE:
                    fptr->SetStatus(DETECTED);
                    ++pattern_num;
                    //run fault simulation for fault dropping
                    for (i = 0;i < PIlist.size();++i) { 
                        ScheduleFanout(PIlist[i]); 
                        if (option.retrieve("output")){ OutputStrm << PIlist[i]->GetValue();}
                    }
                    if (option.retrieve("output")){ OutputStrm << endl;}
                    for (i = PIlist.size();i<Netlist.size();++i) { Netlist[i]->SetValue(X); }
                    LogicSim();
                    BFaultSim();
                    break;
                case CONFLICT:
                    fptr->SetStatus(REDUNDANT);
                    break;
                case FALSE:
                    fptr->SetStatus(ABORT);
                    break;
                
            }

            // if(fptr->GetStatus() == UNKNOWN) {
            //     fptr->SetStatus(ABORT);
            // }

        }

    } //end all faults




    //*************QQ***************************************************
    cout << "\n\nQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n";
    for (fite = BFlist.begin();fite!=BFlist.end();++fite) {
        cout << (*fite)->output() << "  " << (*fite)->GetStatus() << endl;
    }
    cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n\n\n";


    //compute fault coverage
    unsigned total_num(0);
    unsigned abort_num(0), redundant_num(0), detected_num(0);
    unsigned eqv_abort_num(0), eqv_redundant_num(0), eqv_detected_num(0);
    for (fite = BFlist.begin();fite!=BFlist.end();++fite) {

        fptr = *fite;
        switch (fptr->GetStatus()) {
            case DETECTED:
                ++eqv_detected_num;
                detected_num += fptr->GetEqvFaultNum();
                break;
            case REDUNDANT:
                ++eqv_redundant_num;
                redundant_num += fptr->GetEqvFaultNum();
                break;
            case ABORT:
                ++eqv_abort_num;
                abort_num += fptr->GetEqvFaultNum();
                break;
            default:
                cerr << "Unknown fault type exists" << endl;
                cerr << ((BRIDGING_FAULT*)fptr)->output() << endl;

                break;
        }
    }
    total_num = detected_num + abort_num + redundant_num;

    if(tracePodem) {
        cout << "========================================\n\n\n";
    }



    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "---------------------------------------" << endl;
    cout << "Test pattern number = " << pattern_num << endl;
    cout << "Total backtrack number = " << total_backtrack_num << endl;
    cout << "---------------------------------------" << endl;
    cout << "Total fault number = " << total_num << endl;
    cout << "Detected fault number = " << detected_num << endl;
    cout << "Undetected fault number = " << abort_num + redundant_num << endl;
    cout << "Abort fault number = " << abort_num << endl;
    cout << "Redundant fault number = " << redundant_num << endl;
    cout << "---------------------------------------" << endl;
    cout << "Total equivalent fault number = " << Flist.size() << endl;
    cout << "Equivalent detected fault number = " << eqv_detected_num << endl;
    cout << "Equivalent undetected fault number = " << eqv_abort_num + eqv_redundant_num << endl;
    cout << "Equivalent abort fault number = " << eqv_abort_num << endl;
    cout << "Equivalent redundant fault number = " << eqv_redundant_num << endl;
    cout << "---------------------------------------" << endl;
    cout << "Fault Coverge = " << 100*detected_num/double(total_num) << "%" << endl;
    cout << "Equivalent FC = " << 100*eqv_detected_num/double(Flist.size()) << "%" << endl;
    cout << "Fault Efficiency = " << 100*detected_num/double(total_num - redundant_num) << "%" << endl;
    cout << "---------------------------------------" << endl;
    return;

}




ATPG_STATUS CIRCUIT::BridgingPodem(FAULT* fptr, unsigned &total_backtrack_num, GATEPTR gp, VALUE v)
{
    unsigned i, backtrack_num(0);
    GATEPTR pi_gptr(0), decision_gptr(0);
    ATPG_STATUS status;

    //set all values as unknown
    for (i = 0;i<Netlist.size();++i) { Netlist[i]->SetValue(X); }
    //mark propagate paths
    MarkPropagateTree(fptr->GetOutputGate());
    //propagate fault free value
    status = SetUniqueImpliedValue(fptr);
    if(tracePodem) {
        cout << "SetUniqueImpliedValue " << status << endl;
    }
    switch (status) {
        case TRUE:
            LogicSim();
            //inject faulty value
            if (FaultEvaluate(fptr)) {
                if(tracePodem) { cout << "+ FaultEvaluate : true" << endl; }
                //forward implication
                ScheduleFanout(fptr->GetOutputGate());
                LogicSim();
            }
            //check if the fault has propagated to PO
            if (!CheckTest()) { status = FALSE; }
            break;
        case CONFLICT:
            status = CONFLICT;
            break;
        case FALSE: break;
    }

    while(backtrack_num < BackTrackLimit && status == FALSE) {
        //search possible PI decision
        pi_gptr = TestPossible(fptr);
        if (pi_gptr) { //decision found
            ScheduleFanout(pi_gptr);
            //push to decision tree
            GateStack.push_back(pi_gptr);
            decision_gptr = pi_gptr;
        }
        else { //backtrack previous decision
            while (!GateStack.empty() && !pi_gptr) {
                //all decision tried (1 and 0)
                if (decision_gptr->GetFlag(ALL_ASSIGNED)) {
                    decision_gptr->ResetFlag(ALL_ASSIGNED);
                    decision_gptr->SetValue(X);
                    ScheduleFanout(decision_gptr);
                    //remove decision from decision tree
                    GateStack.pop_back();
                    decision_gptr = GateStack.back();
                }
                //inverse current decision value
                else {
                    decision_gptr->InverseValue();
                    ScheduleFanout(decision_gptr);
                    decision_gptr->SetFlag(ALL_ASSIGNED);
                    ++backtrack_num;
                    pi_gptr = decision_gptr;
                }
            }
            //no other decision
            if (!pi_gptr) { status = CONFLICT; }
        }
        if (pi_gptr) {
            LogicSim();
            //fault injection
            if(FaultEvaluate(fptr)) {
                //forward implication
                ScheduleFanout(fptr->GetOutputGate());
                LogicSim();
            }
            if (CheckTest()) { status = TRUE; }
        }
    } //end while loop

    //clean ALL_ASSIGNED and MARKED flags
    list<GATEPTR>::iterator gite;
    for (gite = GateStack.begin();gite != GateStack.end();++gite) {
        (*gite)->ResetFlag(ALL_ASSIGNED);
    }
    for (gite = PropagateTree.begin();gite != PropagateTree.end();++gite) {
        (*gite)->ResetFlag(MARKED);
    }

    //clear stacks
    GateStack.clear(); PropagateTree.clear();
    

    //assign true values to PIs
    if (status ==  TRUE) {

        if(gp->GetValue() == X || gp->GetValue() == v) {
            gp->SetValue(v);
        }
        else {
            return CONFLICT;
        }

		for (i = 0;i<PIlist.size();++i) {
		    switch (PIlist[i]->GetValue()) {
			case S1: break;
			case S0: break;
			case D: PIlist[i]->SetValue(S1); break;
			case B: PIlist[i]->SetValue(S0); break;
			case X: PIlist[i]->SetValue(VALUE(2.0 * rand()/(RAND_MAX + 1.0))); break;
			default: cerr << "Illigal value" << endl; break;
		    }
		}//end for all PI
    } //end status == TRUE

    total_backtrack_num += backtrack_num;
    return status;
}
