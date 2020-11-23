#include <iostream>
#include <vector>
#include "circuit.h"
#include "gate.h"
#include "bridgingFaults.h"
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