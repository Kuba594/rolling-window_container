#include <fstream>
#include <string>
#include <vector>
#include<iostream>
#include "Counting.h"
using namespace std;
int main(int argc, char ** argv) {

	vector<string> arg( argv+1, argv+argc);
	InputProcesser x;
	if (arg.empty()) {
		x.fce(cin);
		x.printoutput();
		return 0;
	}
	for (auto&& a : arg) {
		ifstream f;
		f.open(a);
		if( !f.good()) {
			return 1;
		}
		x.fce(f);
	}
	x.printoutput();
	return 0;
}