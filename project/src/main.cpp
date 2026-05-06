#include <iostream>
#include "kontejner.h"
using namespace std;
int main(int argc, char ** argv){
    RollingMatrix<double> k(1,2);
    cout << k.rows() <<" " <<k.capacity()<<" "<<k.cols()<< endl;
    k.push_column({1});
    k.push_column({2});
    k.push_column({3});
    cout << k.at(0,1) << endl;
    return 0;
}