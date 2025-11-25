#include <iostream>
#include "AbstValue.h"
#include "Seznam.h"
using namespace std;
int main(){
    Seznam s;
    s.add(make_unique<IntValue>(10));
    s.add(make_unique<StringValue>("AHOOj"));
    s.print();
    Seznam s2;
    s2 = s;
    s2.print();
    return 0;
}