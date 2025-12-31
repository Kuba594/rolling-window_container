#include <iostream>
#include <vector>
#include "funktory.h"
using namespace std;


int main(int argc, char ** argv) {
    vector<int> v1 = {10, 2, 8, 20, 1};
    int X = 5, Y = 100;
    ftor1 f1(X, Y);
    f1 = for_each(v1.begin(), v1.end(), f1);
    for(auto i : f1.vysledek) cout << i << " ";
    cout << endl;

    vector<int> v2 = {1, 2, 3, 10, 11};
    int N = 5;
    ftor2 f2(N);
    auto it = find_if(v2.begin(), v2.end(), f2);
    if (it != v2.end()) {
        cout <<  *it;
    } else {
        cout << "Nenalezeno.";
    }
    cout<<endl;


    vector<int> v3 = {5, 15, 20, 100, 25};
    ftor3 f3(10, 30, 10);
    for_each(v3.begin(), v3.end(), f3);
    for(auto i : v3) cout << i << " ";
    cout << endl;


    vector<int> v4 = {10, 12, 50, 52};
    ftor4 f4;
    f4 = for_each(v4.begin(), v4.end(), f4);
    cout <<  f4.vysledek << endl;


    vector<int> v5 = {1, 2, 999, 3, 4};
    ftor5 f5(v5.size());
    f5 = for_each(v5.begin(), v5.end(), f5);
    for(auto i : f5.vysledek) cout << i << " ";
    cout << endl;

    return 0;
}