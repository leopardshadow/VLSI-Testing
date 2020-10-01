#include <iostream>
#include <ctime>
#include "circuit.h"
#include "GetLongOpt.h"
#include "ReadPattern.h"

#include <map>
#include <deque>

using namespace std;

// All defined in readcircuit.l
extern char* yytext;
extern FILE *yyin;
extern CIRCUIT Circuit;
extern int yyparse (void);
extern bool ParseError;

extern void Interactive();

GetLongOpt option;


/***********************************************/
#define debug(x) cerr << #x << ": " << (x) << endl;
#define FOR(x) for(size_t i=0; i<(x); i++)

string piStr, poStr;
size_t pi, po;
size_t totCount = 0;

map<string, int> getGateNumberByName;

enum GSTATE {START, END, PATH, NON};

struct GInfo {
    GSTATE gstate;
    GInfo() {
        gstate = NON;
    }
    int toStr() {
        return gstate;
    }
};

vector<GInfo> ginfos;



void getPiPo() {

    pi =  getGateNumberByName[piStr];
    po =  getGateNumberByName[poStr];

    debug(pi);
    debug(po);

    // FOR(Circuit.No_Gate()) {
    //     if(Circuit.Gate(i)->GetName() == piStr) {
    //         pi = i;
    //         debug(pi);
    //     }
    // }  
    // FOR(Circuit.No_Gate()) {
    //     if(Circuit.Gate(i)->GetName() == poStr) {
    //         po = i;
    //         debug(po);
    //     }
    // }
}


// void searchPathDFS(GATE *g, string s) {
//     if(g->GetName() == poStr) {
//         totCount ++;
//         cout << s << totCount << endl;
//         return;
//     }
//     FOR(g->No_Fanout()) {
//         searchPathDFS(g->Fanout(i), s+g->GetName()+" ");
//     }
// }

map<GATE*, size_t> gate2count;
deque<GATE*> bfsQueue;

void searchPathCountDFS() {
    FOR(Circuit.No_PI()) {
        gate2count[Circuit.PIGate(i)] = 0;
    }
    gate2count[Circuit.Gate(pi)] = 1;

    // FOR(Circuit.No_PO()) {
    //     gate2count[Circuit.POGate(i)] = 0;
    // }
    // gate2count[Circuit.Gate(po)] = 0;

    // FOR()


    FOR(Circuit.No_PI()) {
        bfsQueue.push_back(Circuit.PIGate(i));
    }

    cout << "=====" << endl;
    while(! bfsQueue.empty()) {
        GATE* g = bfsQueue.front();
        bfsQueue.pop_front();
        FOR(g->No_Fanout()) {

            cout << g->Fanout(i)->GetName() << " ";
        }
        cout << endl;
        // gate2count[g]
    }
    cout << "-----" << endl;

    // FOR(Circuit.No_Gate()) {
    //     debug(i);
    //     debug(Circuit.Gate(i)->GetName());
    //     debug(gate2count[Circuit.Gate(i)]);
    //     cout << endl;
    // }

} 

/***********************************************/


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

    /***********************************************/
    option.enroll("start", GetLongOpt::MandatoryValue,
            "start point of a circuit", 0);
    option.enroll("end", GetLongOpt::MandatoryValue,
            "end point of a circuit", 0);
    option.enroll("path", GetLongOpt::NoValue,
            "output all possible path with given pi & po", 0);
    /***********************************************/

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
    /***********************************************/
    else if (option.retrieve("path")) {

        piStr = option.retrieve("start");
        poStr = option.retrieve("end");

        

        
        FOR(Circuit.No_Gate()) {
            getGateNumberByName[Circuit.Gate(i)->GetName()] = i;
        }

        
        // FOR(Circuit.No_Gate()) {
        //     debug(i);
        //     debug(Circuit.Gate(i)->GetName())
        //     cout << "\n";
        // }
        

        // debug(piStr);
        // debug(poStr);

        getPiPo();


        


        // // init all gates states with value NON
        // FOR(Circuit.No_Gate()) {
        //     ginfos.push_back(GInfo());
        // }
        // // specify inpu and output
        // ginfos.at(pi).gstate = START;
        // ginfos.at(po).gstate = END;  

        // // FOR(Circuit.No_Gate()) {
        // //     cout << Circuit.Gate(i)->GetName() << " " << ginfos.at(i).gstate << endl;
        // // }

        // searchPathCountDFS(Circuit.Gate(pi), "");

        searchPathCountDFS();

        cout << "The paths from " << piStr << " to " << poStr << ": " << totCount << endl;
    }
    /***********************************************/
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
