#include <iostream>
#include "circuit.h"
#include "GetLongOpt.h"
using namespace std;

extern GetLongOpt option;

// Event-driven Parallel Pattern Logic simulation
void CIRCUIT::ParallelLogicSimVectors()
{
    cout << "Run Parallel Logic simulation" << endl;
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    while(!Pattern.eof()){ 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++){
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.ReadNextPattern(pattern_idx);
            }
            else break;
        }
        ScheduleAllPIs();
        ParallelLogicSim();
        PrintParallelIOs(pattern_idx);
    }
}

//Assign next input pattern to PI's idx'th bits
void PATTERN::ReadNextPattern(unsigned idx)
{
    char V;
    for (int i = 0; i < no_pi_infile; i++) {
        patterninput >> V;
        if (V == '0') {
            inlist[i]->ResetWireValue(0, idx);
            inlist[i]->ResetWireValue(1, idx);
        }
        else if (V == '1') {
            inlist[i]->SetWireValue(0, idx);
            inlist[i]->SetWireValue(1, idx);
        }
        else if (V == 'X') {
            inlist[i]->SetWireValue(0, idx);
            inlist[i]->ResetWireValue(1, idx);
        }
    }
    //Take care of newline to force eof() function correctly
    patterninput >> V;
    if (!patterninput.eof()) patterninput.unget();
    return;
}

//Assign next input pattern to PI's idx'th bits
void PATTERN::MyReadNextPattern(unsigned idx)
{
    char V;
    for (int i = 0; i < no_pi_infile; i++) {
        patterninput >> V;
        if (V == '0') {
            inlist[i]->ResetWireValue(0, idx);
            inlist[i]->ResetWireValue(1, idx);
        }
        else if (V == '1') {
            inlist[i]->SetWireValue(0, idx);
            inlist[i]->SetWireValue(1, idx);
        }
        else if (V == 'X') {
            inlist[i]->SetWireValue(0, idx);
            inlist[i]->ResetWireValue(1, idx);
        }
    }



    //Take care of newline to force eof() function correctly
    patterninput >> V;
    if (!patterninput.eof()) patterninput.unget();
    return;
}


//Simulate PatternNum vectors
void CIRCUIT::ParallelLogicSim()
{
    GATE* gptr;
    for (unsigned i = 0;i <= MaxLevel;i++) {
        while (!Queue[i].empty()) {
            gptr = Queue[i].front();
            Queue[i].pop_front();
            gptr->ResetFlag(SCHEDULED);
            ParallelEvaluate(gptr);
        }
    }
    return;
}

//Evaluate parallel value of gptr
void CIRCUIT::ParallelEvaluate(GATEPTR gptr)
{
    static int eval = 1;
    register unsigned i;
    bitset<PatternNum> new_value1(gptr->Fanin(0)->GetValue1());
    bitset<PatternNum> new_value2(gptr->Fanin(0)->GetValue2());
    switch(gptr->GetFunction()) {
        case G_AND:
        case G_NAND:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 &= gptr->Fanin(i)->GetValue1();
                new_value2 &= gptr->Fanin(i)->GetValue2();
            }
            break;
        case G_OR:
        case G_NOR:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 |= gptr->Fanin(i)->GetValue1();
                new_value2 |= gptr->Fanin(i)->GetValue2();
            }
            break;
        default: break;
    } 
    //swap new_value1 and new_value2 to avoid unknown value masked
    if (gptr->Is_Inversion()) {
        new_value1.flip(); new_value2.flip();
        bitset<PatternNum> value(new_value1);
        new_value1 = new_value2; new_value2 = value;
    }
    if (gptr->GetValue1() != new_value1 || gptr->GetValue2() != new_value2) {
        gptr->SetValue1(new_value1);
        gptr->SetValue2(new_value2);
        ScheduleFanout(gptr);
    }
    cout << "\n--\neval: " << (eval++) << endl;
    return;
}

void CIRCUIT::PrintParallelIOs(unsigned idx)
{
    register unsigned i;
    for (unsigned j=0; j<idx; j++){
	    for (i = 0;i<No_PI();++i) { 
		    if(PIGate(i)->GetWireValue(0, j)==0){ 
			   if(PIGate(i)->GetWireValue(1, j)==1){
	    			cout << "F";
			   }
			   else cout << "0";
		    }
		    else{
			   if(PIGate(i)->GetWireValue(1, j)==1){
	    			cout << "1";
			   }
			   else cout << "X";
		    }

	    }
	    cout << " ";
	    for (i = 0;i<No_PO();++i) { 
		    if(POGate(i)->GetWireValue(0, j)==0){ 
			   if(POGate(i)->GetWireValue(1, j)==1){
	    			cout << "F";
			   }
			   else cout << "0";
		    }
		    else{
			   if(POGate(i)->GetWireValue(1, j)==1){
	    			cout << "1";
			   }
			   else cout << "X";
		    }
	    }
	    cout << endl;
    }
    return;
}

void CIRCUIT::ScheduleAllPIs()
{
    for (unsigned i = 0;i < No_PI();i++) {
        ScheduleFanout(PIGate(i));
    }
    return;
}



//***************************************************************************//
void CIRCUIT::packXinput(unsigned int patNum) {

    cout << "Run Parallel Logic simulation" << endl;
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    while(!Pattern.eof()){ 
	for(pattern_idx=0; pattern_idx<patNum; pattern_idx++){
	    if(!Pattern.eof()){ 
	        ++pattern_num;
	        Pattern.ReadNextPattern(pattern_idx);
	    }
	    else break;
	}
	ScheduleAllPIs();
	ParallelLogicSim();
	PrintParallelIOs(pattern_idx);
    }



}

void CIRCUIT::simulator(string filename) {

    // cout << "generating compiled code" << endl;

    fstream fout ;  
    fout.open(filename, ios::out);

    fout << "#include <iostream>\n";
    fout << "#include <ctime>\n";
    fout << "#include <bitset>\n";
    fout << "#include <string>\n";
    fout << "#include <fstream>\n";

    fout << "using namespace std;\n";
    fout << "const unsigned PatternNum = 16;\n\n";

    fout << "void evaluate();\n";
    fout << "void printIO(unsigned idx);\n\n";



    for(int i=0; i<No_Gate(); i++) {
        // "bitset<PatternNum> G_G1[2];"
        fout << "bitset<PatternNum> " << Gate(i)->GetName() << "[" << 2 << "];" << endl;
    }
    fout << "bitset<PatternNum> temp;" << endl;

    fout << "ofstream fout(\"" << "filename" << "\",ios::out);" << endl;

    fout << "\n\nint main() {\n";


    // prepare input !!
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    while(!Pattern.eof()) { 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++){
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.MyReadNextPattern(pattern_idx);
            }
            else break;
	    }

        for (int i = 0; i < No_PI(); i++) {
            fout << PIGate(i)->GetName() << "[0] = " << (int)(PIGate(i)->GetValue1().to_ulong()) << ";" << endl;
            fout << PIGate(i)->GetName() << "[1] = " << (int)(PIGate(i)->GetValue2().to_ulong()) << ";" <<  endl;
        }

        fout << "\nevaluate();\n";
        fout << "printIO(" << pattern_idx << ");\n\n"; 

        MyScheduleAllPIs();
        // // PrintParallelIOs(pattern_idx);
    }


    fout << "fout.close();\n";
    fout << "return 0;\n\n";

    // end of main
    fout << "\n}\n\n";



    fout << "void evaluate() {\n\n";
    MyParallelLogicSim(fout);
    fout << "}\n\n";



    fout << "void printIO(unsigned idx) {\n";
    fout << "for (unsigned j=0; j<idx; j++) {\n";
    
    for(int i=0; i<No_PI(); i++) {

        fout << "    if(" << PIGate(i)->GetName() << "[0][j]==0)\n";
        fout << "    {\n";
        fout << "        if(" << PIGate(i)->GetName() << "[1][j]==1)\n";
        fout << "            fout<<\"F\";\n";
        fout << "        else\n";
        fout << "            fout<<\"0\";\n";
        fout << "    }\n";
        fout << "    else\n";
        fout << "    {\n";
        fout << "        if(" << PIGate(i)->GetName() << "[1][j]==1)\n";
        fout << "            fout<<\"1\";\n";
        fout << "        else\n";
        fout << "            fout<<\"2\";\n";
        fout << "    }\n";
    }


    fout << "fout<<\" \";\n";

    for(int i=0; i<No_PO(); i++) {

        fout << "    if(" << POGate(i)->GetName() << "[0][j]==0)\n";
        fout << "    {\n";
        fout << "        if(" << POGate(i)->GetName() << "[1][j]==1)\n";
        fout << "            fout<<\"F\";\n";
        fout << "        else\n";
        fout << "            fout<<\"0\";\n";
        fout << "    }\n";
        fout << "    else\n";
        fout << "    {\n";
        fout << "        if(" << POGate(i)->GetName() << "[1][j]==1)\n";
        fout << "            fout<<\"1\";\n";
        fout << "        else\n";
        fout << "            fout<<\"2\";\n";
        fout << "    }\n";
    }

    fout << "fout << endl;\n";


    fout << "}\n";
    fout << "}\n";


    fout.close();
}


void CIRCUIT::MyParallelLogicSim() {

    static unsigned evalCnt = 0;
    GATE* gptr;
    for (unsigned i = 0;i <= MaxLevel;i++) {

        while (!Queue[i].empty()) {
            gptr = Queue[i].front();
            Queue[i].pop_front();
            gptr->ResetFlag(SCHEDULED);
            MyParallelEvaluate(gptr);
            evalCnt++;
            // cout << "*";
        }
    }
    cout << "eval count: " << evalCnt << endl;
    return;
}



//Evaluate parallel value of gptr
void CIRCUIT::MyParallelEvaluate(GATEPTR gptr)
{
    // cout << "evaluating " << gptr->GetName() << endl;
    register unsigned i;
    bitset<PatternNum> new_value1(gptr->Fanin(0)->GetValue1());
    bitset<PatternNum> new_value2(gptr->Fanin(0)->GetValue2());
    switch(gptr->GetFunction()) {
        case G_AND:
        case G_NAND:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 &= gptr->Fanin(i)->GetValue1();
                new_value2 &= gptr->Fanin(i)->GetValue2();
            }
            break;
        case G_OR:
        case G_NOR:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 |= gptr->Fanin(i)->GetValue1();
                new_value2 |= gptr->Fanin(i)->GetValue2();
            }
            break;
        default: break;
    } 
    //swap new_value1 and new_value2 to avoid unknown value masked
    if (gptr->Is_Inversion()) {
        new_value1.flip(); new_value2.flip();
        bitset<PatternNum> value(new_value1);
        new_value1 = new_value2; new_value2 = value;
    }
    // do it anyway !!
    // if (gptr->GetValue1() != new_value1 || gptr->GetValue2() != new_value2) {
        gptr->SetValue1(new_value1);
        gptr->SetValue2(new_value2);
        ScheduleFanout(gptr);
    // }
    return;
}




void CIRCUIT::MyParallelLogicSim(fstream &fout) {

    static unsigned evalCnt = 0;
    GATE* gptr;
    for (unsigned i = 0;i <= MaxLevel;i++) {

        while (!Queue[i].empty()) {
            gptr = Queue[i].front();
            Queue[i].pop_front();
            gptr->ResetFlag(SCHEDULED);
            MyParallelEvaluate(gptr, fout);
            evalCnt++;
            // cout << "*";
        }
    }
    cout << "eval count: " << evalCnt << endl;
    return;
}


//Evaluate parallel value of gptr
void CIRCUIT::MyParallelEvaluate(GATEPTR gptr, fstream &fout)
{
    // cout << "evaluating " << gptr->GetName() << endl;
    register unsigned i;
    bitset<PatternNum> new_value1(gptr->Fanin(0)->GetValue1());
    fout << gptr->GetName() << "[0] = " << gptr->Fanin(0)->GetName() << "[0];" << endl;
    bitset<PatternNum> new_value2(gptr->Fanin(0)->GetValue2());
    fout << gptr->GetName() << "[1] = " << gptr->Fanin(0)->GetName() << "[1];" << endl;
    switch(gptr->GetFunction()) {
        case G_AND:
        case G_NAND:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 &= gptr->Fanin(i)->GetValue1();
                fout << gptr->GetName() << "[0] &= " << gptr->Fanin(i)->GetName() << "[0];" << endl;
                new_value2 &= gptr->Fanin(i)->GetValue2();
                fout << gptr->GetName() << "[1] &= " << gptr->Fanin(i)->GetName() << "[1];" << endl;
            }
            break;
        case G_OR:
        case G_NOR:
            for (i = 1; i < gptr->No_Fanin(); ++i) {
                new_value1 |= gptr->Fanin(i)->GetValue1();
                fout << gptr->GetName() << "[0] |= " << gptr->Fanin(i)->GetName() << "[0];" << endl;
                new_value2 |= gptr->Fanin(i)->GetValue2();
                fout << gptr->GetName() << "[1] |= " << gptr->Fanin(i)->GetName() << "[1];" << endl;
            }
            break;
        default: break;
    } 
    //swap new_value1 and new_value2 to avoid unknown value masked
    if (gptr->Is_Inversion()) {
        new_value1.flip(); new_value2.flip();
        bitset<PatternNum> value(new_value1);
        fout << "temp = " << gptr->GetName() << "[0];\n";
        new_value1 = new_value2; new_value2 = value;
        fout << gptr->GetName() << "[0] = ~" << gptr->GetName() << "[1];\n";
        fout << gptr->GetName() << "[1] = " << "~temp;\n";
    }
    // do it anyway !!
    // if (gptr->GetValue1() != new_value1 || gptr->GetValue2() != new_value2) {
        gptr->SetValue1(new_value1);
        gptr->SetValue2(new_value2);
        ScheduleFanout(gptr);
    // }
    return;
}



// ~~Event-driven Parallel Pattern Logic simulation~~
void CIRCUIT::MyParallelLogicSimVectors()
{
    cout << "Run Parallel Logic simulation" << endl;
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    while(!Pattern.eof()){ 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++){
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.ReadNextPattern(pattern_idx);
            }
            else break;
        }
        MyScheduleAllPIs();
        MyParallelLogicSim();
        // PrintParallelIOs(pattern_idx);
    }
}

void CIRCUIT::MyScheduleAllPIs()
{
    for (unsigned i = 0;i < No_PI();i++) {
        MyScheduleFanout(PIGate(i));
    }
    return;
}




// ~~Event-driven Parallel Pattern Logic simulation~~
void CIRCUIT::MyParallelLogicSimVectors(fstream &fout)
{
    cout << "Run Parallel Logic simulation" << endl;
    unsigned pattern_num(0);
    unsigned pattern_idx(0);
    while(!Pattern.eof()){ 
        for(pattern_idx=0; pattern_idx<PatternNum; pattern_idx++){
            if(!Pattern.eof()){ 
                ++pattern_num;
                Pattern.ReadNextPattern(pattern_idx);
            }
            else break;
        }
        MyScheduleAllPIs();
        MyParallelLogicSim();
        // PrintParallelIOs(pattern_idx);
    }
}

void CIRCUIT::MyScheduleAllPIs(fstream &fout)
{
    for (unsigned i = 0;i < No_PI();i++) {
        MyScheduleFanout(PIGate(i));
    }
    return;
}
