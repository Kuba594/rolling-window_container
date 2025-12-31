#include <iostream>
#include <ostream>

#include "stable_vector.hpp"
using namespace std;

int main() {
    stable_vector<int> v;
    v.push_back(1);
    cout << v.at(0) << endl;
    try {
        cout << v.at(1) << endl;
    }
    catch (stable_vector_exception& e) {
        cout << e.what() << endl;
        cout<<e.get_index()<<endl;
    }
    return 0;
}