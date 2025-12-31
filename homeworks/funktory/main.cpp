#include <iostream>
#include <vector>
#include "funktory.h"
using namespace std;


int main(int argc, char ** argv) {
    vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto s = for_each(v.begin(), v.end(), ftor1{5, 2});
    cout << *++s.vysledek.begin()<<endl;
    cout << *find_if(v.begin(), v.end(), ftor2{3})<<endl;
    for_each(v.begin(), v.end(), ftor3{13});
    cout << *v.begin()<<endl;
    vector<int> v2 {1, 2,  4, 5, 6, 7, 8, 9, 10};
    auto s2 = for_each(v2.begin(), v2.end(), ftor4{});
    cout << s2.vysledek<<endl;
    return 0;
}