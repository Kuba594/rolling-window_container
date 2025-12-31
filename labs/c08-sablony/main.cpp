#include <iostream>
#include <ostream>

#include "zlomek.h"
int main() {
    Zlomek<int> z(1,2);
    Zlomek<int> w(2,3);
    Zlomek<int> x = z+w;
    x.print();

    DA<Zlomek<int>> c{z};
    DA<Zlomek<int>> d{w};
    DA<Zlomek<int>> e = c+d;
    e.print();
    return 0;
}