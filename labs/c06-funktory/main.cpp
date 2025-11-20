#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
using namespace std;

class ftor1 {
public:
    ftor1(int greater_than, int increment): greater_than_(greater_than), limit_(increment) {}
    void operator() (int& x) {
        if( x > greater_than_) {
            vysledek.insert(x+limit_);
        }
    }
    multiset<int> vysledek;
private:
    int limit_;
    int greater_than_;

};

class ftor2 {
public:
    ftor2(int limit): limit_(limit) {}
    bool operator() (int& x) {
        if (!initialized_) {
            prev_ = x;
            initialized_ = true;
            return false;
        }
        bool return_value = (x > limit_+prev_) || (x < prev_ - limit_);
        prev_ = x;
        return return_value;
    }
private:
    int limit_;
    bool initialized_ {false};
    int prev_;
};

class ftor3 {
public:
    ftor3(int n): n_(n){}
    void operator() (int& x) {
        x += size_ * n_;
        ++size_;
    }
private:
    int size_{1};
    int n_;
};

class ftor4 {
public:
    void operator() (int& x) {
        if (!initialized_) {
            prev_ = x;
            initialized_ = true;
            return;
        }
        if (vetsi_dira) {
            vysledek = x;
        }
        if (x - prev_>dira_) {
            vetsi_dira = true;
            dira_ = x - prev_;
        }
        else {
            vetsi_dira = false;
        }
        prev_ = x;
    }
    int vysledek;
private:
    int dira_{0};
    int prev_;
    bool initialized_{false};
    bool vetsi_dira{false};
};

class ftor5 {
public:
    ftor5(int middle): middle_(middle) {}
    void operator() (int& x) {
        ++counter_;
        if (counter_ < middle_) {
            lower_sum += x;
        }
    }
    int lower_sum;
    int upper_sum;
private:
    int middle_;
    int counter_{0};
};

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