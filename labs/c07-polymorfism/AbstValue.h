//
// Created by Jakub Michalski on 25.11.2025.
//

#ifndef CPP_LABS_ABSTVALUE_H
#define CPP_LABS_ABSTVALUE_H
#include <iostream>
#include <memory>




class AbstValue {
public:
    virtual void print() = 0;
    virtual std::unique_ptr<AbstValue> clone() = 0;
};

using Valptr = std::unique_ptr<AbstValue>;

class IntValue : public AbstValue {
public:
    IntValue(int value) : value_(value) {}
    virtual void print() override { std::cout<<value_; }
    virtual Valptr clone() override { return std::make_unique<IntValue>(value_); }
private:
    int value_;
};
class StringValue : public AbstValue {
public:
    StringValue(std::string value) : value_(std::move(value)) {}
    virtual void print() override { std::cout<<value_; }
    virtual Valptr clone() override { return std::make_unique<StringValue>(value_); }
private:
    std::string value_;
};

#endif //CPP_LABS_ABSTVALUE_H