#include <iostream>
#include "Pole.h"
#include "myexpt.h"
int main() {
    Pole<int> p;
    p.push_back(1);
    p.push_back(2);
    std::cout << p[1] << std::endl;
    try {
        p.at(1);
    }
    catch (const myexpt& e) {
        std::cout << e.what() << e.getIndex() << e.getSize() << std::endl;
    }
    return 0;
}