#ifndef BRIDGING_FAULTS_H
#define BRIDGING_FAULTS_H

#include "gate.h"
#include "fault.h"

enum BFAULT_TYPE {AND, OR};

class BRIDGING_FAULT : public FAULT
{
    private:
        BFAULT_TYPE FType;
        VALUE Value;
        GATE* Input;
        GATE* Output; //record output gate for branch fault
        GATE* gptr1;
        GATE* gptr2;
        bool Branch; //fault is on branch
        unsigned EqvFaultNum; //equivalent fault number (includes itself)
        FAULT_STATUS Status;

    public:
    //***** needs modification on constructor
        BRIDGING_FAULT(BFAULT_TYPE FType, GATE* gptr1, GATE* gptr2):
            FAULT(gptr1, gptr2, S0) {
            // Value(S0), Input(gptr1),Output(gptr2), Branch(false), EqvFaultNum(1), Status(UNKNOWN) {
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
        // VALUE GetValue() { return Value; }

        VALUE GetValue() {
            if(FType == AND) {
                return S0; 
            }
            if(FType == OR ) { 
                return S1;
            }
        }
        // GATE* GetInputGate() { return Input; }
        GATE* GetInputGate1() { return gptr1; }
        GATE* GetInputGate2() { return gptr2; }
        BFAULT_TYPE getFaultType() { return FType; };
        // GATE* GetOutputGate() { return Output; }
        // void SetBranch(bool b) { Branch = b; }
        // bool Is_Branch() { return Branch; }
        // void SetEqvFaultNum(unsigned n) { EqvFaultNum = n; }
        // void IncEqvFaultNum() { ++EqvFaultNum; }
        // unsigned GetEqvFaultNum() { return EqvFaultNum; }
        // void SetStatus(FAULT_STATUS status) { Status = status; }
        // FAULT_STATUS GetStatus() { return Status; }

};



#endif
