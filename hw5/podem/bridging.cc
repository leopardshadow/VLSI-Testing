#include <iostream>
#include <vector>
#include "circuit.h"
#include "gate.h"
#include "bridgingFaults.h"
#include "GetLongOpt.h"

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
    cout << "Run stuck-at fault simulation" << endl;
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

    //for all undetected faults
    vector<BRIDGING_FAULT*>::iterator fite;
    for (fite = UBFlist.begin();fite!=UBFlist.end();++fite) {
        fptr = *fite;
        //skip redundant and detected faults
        if (fptr->GetStatus() == REDUNDANT || fptr->GetStatus() == DETECTED) { continue; }
        //the fault is not active
        if (fptr->GetValue() == fptr->GetInputGate()->GetValue()) { continue; }
        //the fault can be directly seen
        gptr = fptr->GetInputGate();
        if (gptr->GetFlag(OUTPUT) && (!fptr->Is_Branch() || fptr->GetOutputGate()->GetFunction() == G_PO)) {
            fptr->SetStatus(DETECTED);
            continue;
        }
        if (!fptr->Is_Branch()) { //stem
            if (!gptr->GetFlag(FAULTY)) {
                gptr->SetFlag(FAULTY); GateStack.push_front(gptr);
            }
            InjectFaultValue(gptr, fault_idx, fptr->GetValue());
            gptr->SetFlag(FAULT_INJECTED);
            ScheduleFanout(gptr);
            simulate_bflist[fault_idx++] = fptr;
        }
        else { //branch
            if (!CheckFaultyGate(fptr)) { continue; }
            gptr = fptr->GetOutputGate();
            //if the fault is shown at an output, it is detected
            if (gptr->GetFlag(OUTPUT)) {
                fptr->SetStatus(DETECTED);
                continue;
            }
            if (!gptr->GetFlag(FAULTY)) {
                gptr->SetFlag(FAULTY); GateStack.push_front(gptr);
            }
            // add the fault to the simulated list and inject it
            VALUE fault_type = gptr->Is_Inversion()? NotTable[fptr->GetValue()]: fptr->GetValue();
            InjectFaultValue(gptr, fault_idx, fault_type);
            gptr->SetFlag(FAULT_INJECTED);
            ScheduleFanout(gptr);
            simulate_bflist[fault_idx++] = fptr;
        }

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

    // remove detected faults
    for (fite = UBFlist.begin();fite != UBFlist.end();) {
        fptr = *fite;
        if (fptr->GetStatus() == DETECTED || fptr->GetStatus() == REDUNDANT) {
            fite = UBFlist.erase(fite);
        }
        else { ++fite; }
    }
    return;
}




