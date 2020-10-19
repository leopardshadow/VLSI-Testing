
#include <iostream>
#include <map>
#include <bitset>

using namespace std;

map<string, int> encode;
map<int, string> decode;


int l_and(int a, int b) {
	return a & b;
}

int l_or(int a, int b) {
	return a | b;
}

int l_inv(int a) {
	// 00 -> 11
	// 01 -> 01
	// 10 -> 01
	// 11 -> 00

	// int b = (~a) & 3;
	// if(b==2)
	// 	return 1;
	// else
	// 	return b; 

	bool u = a >> 1,
		 l = a & 1,
		 r1 = (~u) & (~l),
		 r0 = (u & l);


	return ~r0;
}

int l_nand(int a, int b) {
	return l_inv( l_and(a, b) );
}

int l_nor(int a, int b) {
	return l_inv( l_or(a, b) );
}

// void test_logic(string func, int (*f)(int, int) ) {
// 	cout << "=== " << func << " ===" << endl;
// 	cout << "   0 1 X X" << endl;
// 	for(int i=0; i<4; i++) {
// 		cout << s[v[i]] << ": ";
// 		for(int j=0; j<4; j++) {
// 			cout << s[(*f)(v[i], v[j])] << " ";
// 		}
// 		cout << endl;
// 	}
// 	cout << endl;
// }


int main() {

	encode["0"] = 0;  // 00
	encode["1"] = 3;  // 11
	encode["X"] = 1;  // 01

	decode[0] = "0";
	decode[1] = "X";
	decode[2] = "X";
	decode[3] = "1";

	// for(int i=0; i<4; i++) {
	// 	cout << "inverse for " << decode[i] << " is " << l_inv(i) << endl;
	// }
	// cout << endl;

	for(int i=0; i<4; i++) {

		cout << 
	}


	/*
		00 (0) -> 0 
		01 (1) -> 1
		10 (2) -> X
		11 (3) -> X
	 */



	// test_logic("and", l_and);
	// test_logic("or", l_or);
	// test_logic("nand", l_nand);
	// test_logic("nor", l_nor);

	return 0;
}
