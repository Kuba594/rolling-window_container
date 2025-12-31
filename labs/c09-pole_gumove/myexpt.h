#ifndef CPP_LABS_MYEXPT_H
#define CPP_LABS_MYEXPT_H

#include <stdexcept>;
class myexpt : public std::out_of_range {
public:
    myexpt(int index, int size): out_of_range("Index out of range"), index_(index), size_(size) {}
    int getIndex() const { return index_; }
    int getSize() const { return size_; }
private:
    int index_;
    int size_;
};


#endif //CPP_LABS_MYEXPT_H