#ifndef CPP_HOMEWORKS_TABLE_H
#define CPP_HOMEWORKS_TABLE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

//polymorfismus s hodnotami
class AbstractVal {
public:
    virtual void print(std::ostream& os) const = 0;
    virtual bool operator< (const AbstractVal& rhs) const =0;
};
//sablonovaná třída
template<typename T>
class ConcreteVal : public AbstractVal {
private:
    T val_;
public:
    ConcreteVal(T&& val);
    void print(std::ostream& os) const override;
    bool operator< (const AbstractVal& rhs) const override;
};
template<typename T>
ConcreteVal<T>::ConcreteVal(T&& val):val_(std::move(val)){}

template<typename T>
void ConcreteVal<T>::print(std::ostream & os) const {os << val_;}

template<typename T>
bool ConcreteVal<T>::operator<(const AbstractVal& rhs) const {
    return val_ < static_cast<const ConcreteVal<T>&>(rhs).val_;
}

//pro jednodussi vypisování
std::ostream& operator<<(std::ostream& os, const AbstractVal& r);



//třída obsahuje řádek a má operátor porovnání
using u_ptr = std::unique_ptr<AbstractVal>;
using vect_ptr = std::vector<u_ptr> ;
class radek {
private:
    vect_ptr radek_;
    char separator_{' '};

public:
    void print_line(std::ostream& os=std::cout) const;
    radek(vect_ptr&& radek, char separator = ' ');
    const AbstractVal& operator[](size_t i) const {return *radek_[i-1];} //minus 1 protože se počítá od 1 a ne 0
};

std::ostream& operator<<(std::ostream& os, const radek& r);



//třída celé tabulky
enum class Type{STRING, INT, ELSE};
class Table {
private:
    std::vector<radek> table_;
    char separator_;
    std::unordered_map<int,Type> map_;
    std::vector<int> sort_order_;
    size_t column_count_{0};
    size_t row_count_{0};
public:
    Table(std::vector<int>&& order, std::unordered_map<int, Type>&& map ,char separator=' ');
    void add_radek(const std::string& radek);
    void print_table(std::ostream& os=std::cout) const;
    void process_thing(vect_ptr& v, std::string& tmp, int column);
    void sort_table();
    bool compare_radek(const radek& a, const radek& b) const;
};


#endif //CPP_HOMEWORKS_TABLE_H