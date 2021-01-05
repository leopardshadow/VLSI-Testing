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
    option.enroll("check_point", GetLongOpt::NoValue,
            "hw4-a: check-point theorem", 0);
    option.enroll("bridging", GetLongOpt::NoValue,
            "hw4-b: generates bridging faults", 0);
    option.enroll("bridging_fsim", GetLongOpt::NoValue,
            "hw5-c: bridging faults simulation", 0);
    option.enroll("hw6_c", GetLongOpt::NoValue,
            "hw6-c: ATPG tracing for a designated fault", 0);
    option.enroll("random_pattern", GetLongOpt::NoValue,
            "hw6-d: random pattern generation before ATPG", 0);
    option.enroll("bridging_atpg", GetLongOpt::NoValue,
            "hw6-e: ATPG for bridging faults", 0);



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
    // else if (option.retrieve("check_point")) {
    //     // Circuit.MarkOutputGate();
    //     Circuit.GenerateAllFaultList();
    //     Circuit.GenerateAllCPSAaultList();
    // }
    else if (option.retrieve("bridging")) {

        Circuit.GenerateBridgingFaults();

        if (option.retrieve("output")) {
            Circuit.PrintBridgingFaults(option.retrieve("output"));
        }
    }
    else if (option.retrieve("bridging_fsim")) {

        cout << "\ngenerates bridging faults\n\n";
        Circuit.GenerateBridgingFaults();

        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();
        Circuit.InitPattern(option.retrieve("input"));

        cout << "\nbridging faults simulation\n\n";
        Circuit.BFaultSimVectors();
        
    }
    else if (option.retrieve("hw6_c")) {

        // for(int i=0; i<Circuit.No_Gate(); i++) {
        //     cout << i << " " << Circuit.Gate(i)->GetName() << endl;
        // }
        // cout << "\n\n\n";

        // net17 stuck-at-0
        // cout << "s-a-0 fault at " << Circuit.Gate(11)->GetName() << endl;
        // Circuit.setFault(Circuit.Gate(11), Circuit.Gate(11), S0);

        // n60 stuck-at-1
        cout << "s-a-1 fault at " << Circuit.Gate(9)->GetName() << endl;
        Circuit.setFault(Circuit.Gate(9), Circuit.Gate(9), S1);

        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();

        if (option.retrieve("fsim")) {
            //stuck-at fault simulator
            Circuit.InitPattern(option.retrieve("input"));
            Circuit.FaultSimVectors();
        }
        else {
            Circuit.Atpg();
        }
    }
    else if (option.retrieve("random_pattern")) {
        cout << "random pattern!!!!!\n\n";
        Circuit.GenerateAllFaultList();
        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();
        Circuit.RandomPattern();
        Circuit.Atpg();
    }
    else if (option.retrieve("bridging_atpg")) {
        Circuit.GenerateBridgingFaults();
        Circuit.SortFaninByLevel();
        Circuit.MarkOutputGate();
        Circuit.BridgingAtpg();
    }
    else {
        
        Circuit.GenerateAllFaultList();

        if (option.retrieve("check_point")) {
            Circuit.GenerateAllCPSAaultList();
        }
        else {
        }
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
