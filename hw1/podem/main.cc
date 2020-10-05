#include <iostream>
#include <ctime>
#include "circuit.h"
#include "GetLongOpt.h"
#include "ReadPattern.h"

#include <map>
#include <deque>
#include <list>

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


void getPiPo() {

    pi =  getGateNumberByName[piStr];
    po =  getGateNumberByName[poStr];
}




// enum GSTATE {START, END, PATH, NON};

class GInfo {
public:
    size_t cnt, fanin;
    bool active;
    deque<string> pathQ;

    GInfo() {}

    GInfo (GATE* g) {

        cnt = 0;
        active = false;
        
        switch (g->GetFunction()) //G_PI, G_PO, G_PPI, G_PPO, G_NOT, G_AND, G_NAND, G_OR, G_NOR, G_DFF, G_BUF, G_BAD
        {
            case G_PI:
                fanin = 0;
                break;

            case G_PO:
                fanin = 9999999999999;
                break;

            default:
                fanin = g->No_Fanin();
                break;
        }
    }

};


map<GATE*, GInfo> gate2count;
deque<GATE*> bfsQueue;

void initBFSfromStart() {

    FOR(Circuit.No_Gate()) {
        gate2count[Circuit.Gate(i)] = GInfo(Circuit.Gate(i));
    }

    gate2count[Circuit.Gate(pi)].cnt = 1;
    gate2count[Circuit.Gate(pi)].pathQ.push_front(Circuit.Gate(pi)->GetName());

    FOR(Circuit.No_PI()) {
        bfsQueue.push_back(Circuit.PIGate(i));
    }
}

void initBFSfromEnd() {

    deque <GATE*> tempQ;

    tempQ.push_back(Circuit.Gate(po));

    while(! tempQ.empty()) {
        
        GATE* g = tempQ.front();
        tempQ.pop_front();

        FOR(g->No_Fanin()) {
            if(gate2count[g->Fanin(i)].active)
                continue;
            gate2count[g->Fanin(i)].active = true;
            tempQ.push_back(g->Fanin(i));
        }
    }
}
size_t ttt = 0;
void searchPathCountBFS() {

    initBFSfromStart();

    initBFSfromEnd();

    while(! bfsQueue.empty()) {

        GATE* g = bfsQueue.front();
        cout << "Now: " << ttt++ << " " << g->GetName() << endl;
        bfsQueue.pop_front();

        FOR(g->No_Fanout()) {
                    
            
            gate2count[g->Fanout(i)].fanin -= 1;

            if(gate2count[g].active) {

                gate2count[g->Fanout(i)].cnt += gate2count[g].cnt;

                for(size_t qi=0; qi<gate2count[g].pathQ.size(); qi++) {
                    // gate2count[g->Fanout(i)].pathQ.push_back(gate2count[g].pathQ[qi]+ " "+g->Fanout(i)->GetName());
                    // gate2count[g->Fanout(i)].pathQ.push_back("");
                }

            }

            if(gate2count[g->Fanout(i)].fanin == 0) {
                bfsQueue.push_back(g->Fanout(i));
            }
   
        }
        
        gate2count[g].pathQ.clear();
    }

    // cout << "print path\n-------" << endl;
    // cout << gate2count[Circuit.Gate(po)].pathQ.size() << endl;
    // FOR(gate2count[Circuit.Gate(po)].pathQ.size()) {
    //     // cout << gate2count[Circuit.Gate(po)].pathQ[i] << endl;
    // }
    // cout << "The paths from " << piStr << " to " << poStr << ": " << gate2count[Circuit.Gate(po)].pathQ.size() << endl;
    cout << "The paths from " << piStr << " to " << poStr << ": " << gate2count[Circuit.Gate(po)].cnt << endl;
} 

size_t layer = 0;

void searchPathDFS(GATE *g, string &s) {
    if(g->GetName() == poStr) {
        totCount ++;
        cout << "--\n";
        cout << totCount << " " << layer << endl;
        cout << "--\n\n";
        // cout << s << " " << poStr << endl;
        return;
    }
    FOR(g->No_Fanout()) {
        if(gate2count[g->Fanout(i)].active) {
            s = s + g->GetName() + " ";
            cout << (layer++) << " ";
            searchPathDFS(g->Fanout(i), s);
            size_t rmLen = g->GetName().length() + 1;
            s = s.substr(0, s.size()-rmLen);
            layer--;
            // cout << (layer--) << " ";
        }
    }
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


        // find the specific output with designated string        
        FOR(Circuit.No_Gate()) {
            getGateNumberByName[Circuit.Gate(i)->GetName()] = i;
        }

        getPiPo();


        // BFS search
        searchPathCountBFS();

        // clock_t time_end1;
        // time_end1 = clock();
        // cout << "total CPU time = " << double(time_end1 - time_init)/CLOCKS_PER_SEC << endl;

        // DFS search
        // string pathStr = "";
        // // initBFSfromStart();
        // initBFSfromEnd();

        // clock_t time_end2;
        // time_end2 = clock();
        // cout << "total CPU time = " << double(time_end2 - time_init)/CLOCKS_PER_SEC << endl;

        // // getchar();
        // gate2count[Circuit.Gate(po)].active = true;
        // searchPathDFS(Circuit.Gate(pi), pathStr);
        // cout << "The paths from " << piStr << " to " << poStr << ": " << totCount << endl;
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
