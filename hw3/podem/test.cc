#include <iostream>
#include <ctime>
#include <bitset>
#include <string>
#include <fstream>
using namespace std;
const unsigned PatternNum = 16;

void evaluate();
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
bitset<PatternNum> temp
ofstream fout("filename",ios::out);


int main() {
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

}void evaluate() {

net17[0] = G1[0];
net17[1] = G1[1];
net17[0] &= G3[0];
net17[1] &= G3[1];
temp = G1[0];
G1[0] = ~G1[1];
G1[1] = ~temp;
n60[0] = G2[0];
n60[1] = G2[1];
n60[0] |= G5[0];
n60[1] |= G5[1];
temp = G2[0];
G2[0] = ~G2[1];
G2[1] = ~temp;
net14[0] = G3[0];
net14[1] = G3[1];
net14[0] &= G4[0];
net14[1] &= G4[1];
temp = G3[0];
G3[0] = ~G3[1];
G3[1] = ~temp;
net18[0] = net14[0];
net18[1] = net14[1];
net18[0] &= G2[0];
net18[1] &= G2[1];
temp = net14[0];
net14[0] = ~net14[1];
net14[1] = ~temp;
net25[0] = net14[0];
net25[1] = net14[1];
temp = net14[0];
net14[0] = ~net14[1];
net14[1] = ~temp;
G16[0] = net17[0];
G16[1] = net17[1];
G16[0] &= net18[0];
G16[1] &= net18[1];
temp = net17[0];
net17[0] = ~net17[1];
net17[1] = ~temp;
G17[0] = n60[0];
G17[1] = n60[1];
G17[0] |= net25[0];
G17[1] |= net25[1];
temp = n60[0];
n60[0] = ~n60[1];
n60[1] = ~temp;
PO_G16[0] = G16[0];
PO_G16[1] = G16[1];
PO_G17[0] = G17[0];
PO_G17[1] = G17[1];
}

