#include "funktory.h"

ftor1::ftor1(int greater_than, int increment): greater_than_(greater_than), limit_(increment) {}

void ftor1::operator() (int& x) {
    if( x > greater_than_) {
        vysledek.insert(x+limit_);
    }
}

ftor2::ftor2(int limit): limit_(limit) {}

bool ftor2::operator() (int& x) {
    if (!initialized_) {
        prev_ = x;
        initialized_ = true;
        return false;
    }
    bool return_value = (x >= limit_+prev_) || (x <= prev_ - limit_);
    prev_ = x;
    return return_value;
}



ftor3::ftor3(int min_val, int max_val, int n): min_(min_val), max_(max_val), n_(n) {}
void ftor3::operator() (int& x)  {
    if (x >= min_ && x <= max_) {
        x += size_ * n_;
        ++size_;
    }
}



void ftor4::operator() (int& x) {
    if (!initialized_) {
        prev_ = x;
        initialized_ = true;
        return;
    }
    if (x - prev_ > dira_) {
        dira_ = x - prev_;
        vysledek = x;
    }

    prev_ = x;
}

ftor5::ftor5(int n): n_(n), half_(n / 2) {}
void ftor5::operator() (int& x) {
    if (idx_ < half_) {
        vysledek.emplace_back(x*x);
    }
    else if (idx_ >= (n_ - half_)) {
        int offset = n_ - half_;
        vysledek[idx_ - offset] += x * x;
    }
    idx_++;
}