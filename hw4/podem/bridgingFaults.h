#ifndef BRIDGING_FAULTS_H
#define BRIDGING_FAULTS_H

#include "gate.h"

enum BFAULT_TYPE {AND, OR};

class BRIDGING_FAULT
{
    private:
        BFAULT_TYPE FType;
        GATE* gptr1;
        GATE* gptr2;
    public:
        BRIDGING_FAULT(BFAULT_TYPE FType, GATE* gptr1, GATE* gptr2) {
            this->FType = FType;
            this->gptr1 = gptr1;
            this->gptr2 = gptr2;
        }
        string output() {
            string out = "";
            out = out + "(" + gptr1->GetName() + ", " + gptr2->GetName() + ", ";
            out = out + (FType ? "OR" : "AND") + ")";
            return out;
        }
};



#endif
