#include <iostream>
#include <string>
#include <vector>
#include "api.h"
#include "Table.h"
using namespace std;
int main(int argc, char ** argv) {
    vector<string> arg( argv+1, argv+argc);
    api a;
    try {
        a.process_arguments(arg);
        a.read_input();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}