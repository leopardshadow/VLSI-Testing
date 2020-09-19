#include <iostream>
#include <ctime>
#include "circuit.h"
#include "GetLongOpt.h"
#include "ReadPattern.h"
using namespace std;

// All defined in readcircuit.l
extern char* yytext;
extern FILE *yyin;
extern CIRCUIT Circuit;
extern int yyparse (void);
extern bool ParseError;

extern void Interactive();

GetLongOpt option;



bool matchGateType(GATE *gate, GATEFUNC gateFunc) {
    return gate->GetFunction() == gateFunc;
}

size_t gateCountOfFunc(GATEFUNC gateFunc) {
    size_t cnt = 0;
    for(size_t i=0; i<Circuit.No_Gate(); i++) {
        if(matchGateType(Circuit.Gate(i), gateFunc))
            cnt += 1;
    }
    return cnt;
}

double avgFanoutsOfFunc(GATEFUNC gateFunc) {
    size_t gateCnt = 0, fanoutCnt = 0;
    for(size_t i=0; i<Circuit.No_Gate(); i++) {
        if(matchGateType(Circuit.Gate(i), gateFunc)) {
            gateCnt += 1;
            fanoutCnt += Circuit.Gate(i)->No_Fanout();
        }
    }
    return 1. * fanoutCnt / gateCnt;
}

int SetupOption(int argc, char ** argv)
{
    option.usage("[options] input_circuit_file");
    option.enroll("help", GetLongOpt::NoValue,
            "print this help summary", 0);
    option.enroll("logicsim", GetLongOpt::NoValue,
            "run logic simulation", 0);
    option.enroll("plogicsim", GetLongOpt::NoValue,
            "run parallel logic simulation", 0);
    option.enroll("fsim", GetLongOpt::NoValue,
            "run stuck-at fault simulation", 0);
    option.enroll("stfsim", GetLongOpt::NoValue,
            "run single pattern single transition-fault simulation", 0);
    option.enroll("transition", GetLongOpt::NoValue,
            "run transition-fault ATPG", 0);
    option.enroll("input", GetLongOpt::MandatoryValue,
            "set the input pattern file", 0);
    option.enroll("output", GetLongOpt::MandatoryValue,
            "set the output pattern file", 0);
    option.enroll("bt", GetLongOpt::OptionalValue,
            "set the backtrack limit", 0);
    option.enroll("ass0", GetLongOpt::NoValue,
            "assignment 0", 0);
    int optind = option.parse(argc, argv);
    if ( optind < 1 ) { exit(0); }
    if ( option.retrieve("help") ) {
        option.usage();
        exit(0);
    }
    return optind;
}

int main(int argc, char ** argv)
{
    int optind = SetupOption(argc, argv);
    clock_t time_init, time_end;
    time_init = clock();
    //Setup File
    if (optind < argc) {
        if ((yyin = fopen(argv[optind], "r")) == NULL) {
            cout << "Can't open circuit file: " << argv[optind] << endl;
            exit( -1);
        }
        else {
            string circuit_name = argv[optind];
            string::size_type idx = circuit_name.rfind('/');
            if (idx != string::npos) { circuit_name = circuit_name.substr(idx+1); }
            idx = circuit_name.find(".bench");
            if (idx != string::npos) { circuit_name = circuit_name.substr(0,idx); }
            Circuit.SetName(circuit_name);
        }
    }
    else {
        cout << "Input circuit file missing" << endl;
        option.usage();
        return -1;
    }
    cout << "Start parsing input file\n";
    yyparse();
    if (ParseError) {
        cerr << "Please correct error and try Again.\n";
        return -1;
    }
    fclose(yyin);
    Circuit.FanoutList();
    Circuit.SetupIO_ID();
    Circuit.Levelize();
    Circuit.Check_Levelization();
    Circuit.InitializeQueue();

    if (option.retrieve("logicsim")) {
        //logic simulator
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.LogicSimVectors();
    }
    else if (option.retrieve("plogicsim")) {
        //parallel logic simulator
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.ParallelLogicSimVectors();
    }
    else if (option.retrieve("stfsim")) {
        //single pattern single transition-fault simulation
        Circuit.MarkOutputGate();
        Circuit.GenerateAllTFaultList();
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.TFaultSimVectors();
    }
    else if (option.retrieve("transition")) {
        Circuit.MarkOutputGate();
        Circuit.GenerateAllTFaultList();
        Circuit.SortFaninByLevel();
        if (option.retrieve("bt")) {
            Circuit.SetBackTrackLimit(atoi(option.retrieve("bt")));
        }
        Circuit.TFAtpg();
    }
    else if (option.retrieve("ass0")) {

        // Number of inputs
        cout << "Number of inputs: " << Circuit.No_PI() << endl;

        // Number of outputs
        cout << "Number of outputs: " << Circuit.No_PO() << endl;

        // Total number of gates including inverter, or, nor, and, nand
        cout << "Total number of gates: " << Circuit.No_Gate() << endl;

        // Number of gates for each type
        cout << " - Number of gates inverter: " << gateCountOfFunc(G_NOT) << endl;
        cout << " - Number of gates or: " << gateCountOfFunc(G_OR) << endl;
        cout << " - Number of gates nor: " << gateCountOfFunc(G_NOR) << endl;
        cout << " - Number of gates and: " << gateCountOfFunc(G_AND) << endl;
        cout << " - Number of gates nand: " << gateCountOfFunc(G_NAND) << endl;

        // Number of flip-flops (if available)
        cout << " - Number of dff: " << gateCountOfFunc(G_DFF) << endl;

        // Total number of signal nets.
        // A net is any wire connecting between (PI, PPI, gate outputs) to (PO, PPO and gate inputs). Note that a gate connecting to different gates will be counted as different nets.
        // TBD

        // Number of branch nets
        // TBD

        // Number of stem nets
        // TBD

        // Average number of fanouts of each gate (all types)
        cout << " - Average number of fanouts  of gates inverter: " << avgFanoutsOfFunc(G_NOT) << endl;
        cout << " - Average number of fanouts  of gates or: " << avgFanoutsOfFunc(G_OR) << endl;
        cout << " - Average number of fanouts  of gates nor: " << avgFanoutsOfFunc(G_NOR) << endl;
        cout << " - Average number of fanouts  of gates and: " << avgFanoutsOfFunc(G_AND) << endl;
        cout << " - Average number of fanouts  of gates nand: " << avgFanoutsOfFunc(G_NAND) << endl;
    }
    
    else {
        Circuit.GenerateAllFaultList();
        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();
        if (option.retrieve("fsim")) {
            //stuck-at fault simulator
            Circuit.InitPattern(option.retrieve("input"));
            Circuit.FaultSimVectors();
        }

        else {
            if (option.retrieve("bt")) {
                Circuit.SetBackTrackLimit(atoi(option.retrieve("bt")));
            }
            //stuck-at fualt ATPG
            Circuit.Atpg();
        }
    }
    time_end = clock();
    cout << "total CPU time = " << double(time_end - time_init)/CLOCKS_PER_SEC << endl;
    cout << endl;
    return 0;
}
