#include <iostream>
#include <ctime>
#include <bitset>
#include <string>
#include <fstream>
using namespace std;
const unsigned PatternNum = 16;

void evaluate();
void printIO(unsigned idx);

bitset<PatternNum> G1[2];
bitset<PatternNum> G2[2];
bitset<PatternNum> G3[2];
bitset<PatternNum> G4[2];
bitset<PatternNum> G5[2];
bitset<PatternNum> PO_G16[2];
bitset<PatternNum> G16[2];
bitset<PatternNum> PO_G17[2];
bitset<PatternNum> G17[2];
bitset<PatternNum> n60[2];
bitset<PatternNum> net14[2];
bitset<PatternNum> net17[2];
bitset<PatternNum> net25[2];
bitset<PatternNum> net18[2];
bitset<PatternNum> temp;
ofstream fout("c17.out",ios::out);


int main() {
clock_t time_init, time_end;
time_init = clock();
G1[0] = 16191;
G1[1] = 15914;
G2[0] = 63350;
G2[1] = 55122;
G3[0] = 34769;
G3[1] = 1984;
G4[0] = 10055;
G4[1] = 9280;
G5[0] = 57273;
G5[1] = 49584;

evaluate();
printIO(16);

G1[0] = 16378;
G1[1] = 15930;
G2[0] = 63482;
G2[1] = 55104;
G3[0] = 34790;
G3[1] = 1894;
G4[0] = 10015;
G4[1] = 9245;
G5[0] = 57304;
G5[1] = 49544;

evaluate();
printIO(8);

time_end = clock();
cout << "Total CPU Time = " << double(time_end - time_init)/CLOCKS_PER_SEC << endl;
fout.close();
return 0;


}

void evaluate() {

net17[0] = G1[0];
net17[1] = G1[1];
net17[0] &= G3[0];
net17[1] &= G3[1];
temp = net17[0];
net17[0] = ~net17[1];
net17[1] = ~temp;
n60[0] = G2[0];
n60[1] = G2[1];
n60[0] |= G5[0];
n60[1] |= G5[1];
temp = n60[0];
n60[0] = ~n60[1];
n60[1] = ~temp;
net14[0] = G3[0];
net14[1] = G3[1];
net14[0] &= G4[0];
net14[1] &= G4[1];
temp = net14[0];
net14[0] = ~net14[1];
net14[1] = ~temp;
net18[0] = net14[0];
net18[1] = net14[1];
net18[0] &= G2[0];
net18[1] &= G2[1];
temp = net18[0];
net18[0] = ~net18[1];
net18[1] = ~temp;
net25[0] = net14[0];
net25[1] = net14[1];
temp = net25[0];
net25[0] = ~net25[1];
net25[1] = ~temp;
G16[0] = net17[0];
G16[1] = net17[1];
G16[0] &= net18[0];
G16[1] &= net18[1];
temp = G16[0];
G16[0] = ~G16[1];
G16[1] = ~temp;
G17[0] = n60[0];
G17[1] = n60[1];
G17[0] |= net25[0];
G17[1] |= net25[1];
temp = G17[0];
G17[0] = ~G17[1];
G17[1] = ~temp;
PO_G16[0] = G16[0];
PO_G16[1] = G16[1];
PO_G17[0] = G17[0];
PO_G17[1] = G17[1];
}

void printIO(unsigned idx) {
for (unsigned j=0; j<idx; j++) {
    if(G1[0][j]==0)
    {
        if(G1[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G1[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(G2[0][j]==0)
    {
        if(G2[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G2[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(G3[0][j]==0)
    {
        if(G3[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G3[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(G4[0][j]==0)
    {
        if(G4[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G4[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(G5[0][j]==0)
    {
        if(G5[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(G5[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
fout<<" ";
    if(PO_G16[0][j]==0)
    {
        if(PO_G16[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(PO_G16[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
    if(PO_G17[0][j]==0)
    {
        if(PO_G17[1][j]==1)
            fout<<"F";
        else
            fout<<"0";
    }
    else
    {
        if(PO_G17[1][j]==1)
            fout<<"1";
        else
            fout<<"2";
    }
fout << endl;
}
}
