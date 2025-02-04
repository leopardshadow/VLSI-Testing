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



//***************************************************************************//


#include <cstdlib>
#include <ctime>
#include <cmath>


int realRandom(int size) {
    // return int((rand()/(RAND_MAX + 1.0))*size);
    const unsigned long long A = 1664525,
                   C = 1013904223,
                   M = (unsigned long long)(pow(2, 32));
    static unsigned long long num = rand() % M;
    num = (A * num + C) % M;
    return  int(size*num/double(M+1));
}

void genPattern(string filename, size_t num, bool unknown) {

    ofstream fout;
    fout.open(filename, ios::out);
    if (!fout) {
        cout << "cannot create output file " << filename << endl;
        exit(-1);
    }

    for(size_t i=0; i<Circuit.No_PI(); i++) {
        fout << "PI " << Circuit.PIGate(i)->GetName() << " ";
    }
    fout << endl;

    for(size_t n=0; n<num; n++) {
        for(size_t i=0; i<Circuit.No_PI(); i++) {
            if (unknown) {
                int rr = realRandom(3);
                if(rr < 2) {
                    fout << rr;
                }
                else {
                    fout << "X";
                }
            }
            else {
                fout << realRandom(2);
            }
        }
        fout << endl;
    }
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
    //***********************************************************************//
    option.enroll("pattern", GetLongOpt::NoValue,
            "generate test patterns ~", 0);
    option.enroll("num", GetLongOpt::MandatoryValue,
            "number of test patterns to generate", 0);
    option.enroll("num", GetLongOpt::MandatoryValue,
            "number of test patterns to generate", 0);
    option.enroll("output", GetLongOpt::MandatoryValue,
            "output file name", 0);
    option.enroll("unknown", GetLongOpt::NoValue,
            "output file name", 0);
    option.enroll("mod_logicsim", GetLongOpt::NoValue,
            "run logic simulation using bin.op~", 0);


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
    //***********************************************************************//
    else if (option.retrieve("pattern")) {

        string filename;
        size_t num = 0;
        bool unknown;

        srand( time(NULL) );

        if (option.retrieve("num")) {
            cout << atoi(option.retrieve("num")) << endl;
            num = atoi(option.retrieve("num"));
        }
        if (option.retrieve("output")) {
            cout << option.retrieve("output") << endl;
            filename = option.retrieve("output");
        }
        unknown = option.retrieve("unknown");
        cout << (option.retrieve("unknown") ? "w/" : "w/o") << " unknown" << endl;
        genPattern(filename, num, unknown);
    }
    else if (option.retrieve("mod_logicsim")) {
        Circuit.InitPattern(option.retrieve("input"));
        Circuit.LogicSimVectors();
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
