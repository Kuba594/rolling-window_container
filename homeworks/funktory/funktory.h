#ifndef CPP_HOMEWORKS_FUNKTORY_H
#define CPP_HOMEWORKS_FUNKTORY_H
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>


class ftor1 {
public:
    ftor1(int greater_than, int increment);
    void operator() (int& x);
    std::multiset<int> vysledek;
private:
    int limit_;
    int greater_than_;
};

class ftor2 {
public:
    ftor2(int limit);
    bool operator() (int& x);
private:
    int limit_;
    bool initialized_ {false};
    int prev_{0};
};

class ftor3 {
public:
    ftor3(int min_val, int max_val, int n) ;

    void operator() (int& x);
private:
    int size_{1};
    int n_;
    int min_;
    int max_;
};

class ftor4 {
public:
    void operator() (int& x);
    int vysledek{0};
private:
    int dira_{0};
    int prev_{0};
    bool initialized_{false};
};

class ftor5 {
public:
    ftor5(int middle);
    void operator() (int& x);
    int lower_sum;
    int upper_sum;
private:
    int middle_;
    int counter_{0};
};

#endif //CPP_HOMEWORKS_FUNKTORY_H