#include <iostream>
#include <string>
#include <vector>
#include "makroprocessor.h"
using namespace std;

int main(int argc, char ** argv) {
    vector<string> arg( argv+1, argv+argc);

    makroprocessor x;
    x.process_arg(arg);
    x.read_input();
    return 0;
}