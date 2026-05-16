#include "ui.h"
#include <iostream>
//main launching the UI
int main() {
    UI repl;
    try {
        repl.run();
    } catch (const std::exception& e) {
        std::cerr << "PROBLEM: " << e.what() << "\n";
        return 1;
    }
    return 0;
}