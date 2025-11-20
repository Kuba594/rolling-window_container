#include <iostream>
#include "makro.h"
using namespace std;

int main(int argc, char ** argv) {
    string s = "AHOJ";
    string d = "CAU";
    makro m(std::move(s), std::move(d));
    m.print_definition();
    cout << m;
    return 0;
}