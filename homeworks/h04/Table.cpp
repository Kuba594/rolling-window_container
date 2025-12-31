#include "Table.h"
#include <algorithm>
#include <vector>

using namespace std;


//deklarace třídy radek
void radek::print_line(std::ostream &os) const {
    bool first = true;
    for (auto && r : radek_) {
        if (!first) os << separator_;
        os << *r;
        first = false;
    }
}
radek::radek(vect_ptr &&radek, char separator) : radek_(std::move(radek)), separator_(separator) {}

ostream &operator<<(ostream &os, const radek &r) {
    r.print_line(os);
    return os;
}
std::ostream& operator<<(std::ostream& os, const AbstractVal& av) {
    av.print(os);
    return os;
}



//deklarace třídy table
Table::Table(vector<int> &&order, unordered_map<int, Type>&& map, char separator) : separator_(separator), map_(std::move(map)) , sort_order_(std::move(order)) {
}

void Table::sort_table() {
    std::sort(table_.begin(), table_.end(),
        [this](const radek& a, const radek& b){
            return compare_radek(a, b);
        }
    );
}

bool Table::compare_radek(const radek& a, const radek& b) const {
    for (auto && col: sort_order_) {
        const AbstractVal& va = a[col];
        const AbstractVal& vb = b[col];
        if (va < vb) return true;
        if (vb < va) return false;
    }
    return false;
}

void Table::add_radek(const std::string &radek) {
    ++row_count_;
    std::string tmp;
    vect_ptr v;
    size_t column = 0;
    for (char c: radek) {
        if (c == separator_) {
            process_thing(v, tmp, ++column);
            tmp.clear();
        } else {
            tmp += c;
        }
    }
    if (column_count_ == 0) {
        column_count_ = column;
    }
    if (column_count_ != column) {
        throw std::runtime_error("error: radka "+to_string(row_count_)+" , sloupec "+ to_string(column_count_) +" - málo sloupců");
    }
    process_thing(v, tmp, ++column);

    table_.emplace_back(std::move(v), separator_);

}

void Table::process_thing(vect_ptr &v, std::string &tmp, int column) {
    auto t = map_.find(column);
    if (t == map_.end() || t->second == Type::STRING) {
        u_ptr ptr = make_unique<ConcreteVal<std::string>>(std::move(tmp));
        v.push_back(std::move(ptr));
        return;
    }
    if (t->second == Type::INT) {
        try {
            int val = std::stoi(tmp);
            v.push_back(std::make_unique<ConcreteVal<int>>(std::move(val)));
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("error: radka "+to_string(row_count_)+" , sloupec "+ to_string(column_count_) +" - mělo být číslo ale je "+tmp);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("error: radka "+to_string(row_count_)+" , sloupec "+ to_string(column_count_) +" - moc velký int");
        }
        return;
    }
}

void Table::print_table(std::ostream &os) const {
    bool first = true;
    for (auto &&r: table_) {
        if (!first) os << '\n';
        os << r;
        first = false;
    }
}
