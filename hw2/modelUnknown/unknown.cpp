
#include <iostream>
using namespace std;

//              0    1    2    3
//             00   01   10   11
string s[4] = {"0", "X", "X", "1"};



int l_and(int a, int b) {
	return (a&b) | ((a|b)&2);
}

int l_or(int a, int b) {
	return (a|b);
}

int l_inv(int a) {
	return ((~a)&1) | (a&2);
}

int l_nand(int a, int b) {
	return l_inv( l_and(a, b) );
}

int l_nor(int a, int b) {
	return l_inv( l_or(a, b) );
}

void test_logic(string func, int (*f)(int, int) ) {
	cout << "=== " << func << " ===" << endl;
	cout << "   0 1 X X" << endl;
	for(int i=0; i<4; i++) {
		cout << s[i] << ": ";
		for(int j=0; j<4; j++) {
			cout << s[(*f)(i, j)] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


int main() {


	/*
		00 (0) -> 0 
		01 (1) -> 1
		10 (2) -> X
		11 (3) -> X
	 */

	test_logic("and", l_and);
	test_logic("or", l_or);
	test_logic("nand", l_nand);
	test_logic("nor", l_nor);

	return 0;
}
