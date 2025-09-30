#include <print>
#include <vector>
#include <string>
using namespace std;

class Myclass {
public:
    Myclass() {}
    Myclass( int x, int from, int to) : x_( x), from_( from), to_( to) {}
    void doit();
private:
    int from_ = 1;
    int to_ = 10;
    int x_;
    int internal_fnc() { return ++x_; }
};

void Myclass::doit() {
    for ( int i = from_; i <= to_; ++i) {
        println( "{:2} x {} = {:2}", i, x_, i*x_);
    }
  }
void argument(int number, int from, int to) {
    Myclass m { number, from, to};
    m.doit();
}
void process_arguments(const vector<string>& a) {
    int from = 1;
    int to = 10;
    for (int i = 1; i < a.size(); ++i) {
        if (a[i] == "nasobilka") {
            continue;
        }
        if (a[i] == "-f" && i+1 < a.size()) {
            from = stoi(a[++i]);
            continue;
        }
        if (a[i] == "-t" && i+1 < a.size()) {
            to = stoi(a[++i]);
            continue;
        }
        int x = stoi( a[i]);
        argument(x, from, to);
    }
}
int main(int argc, char ** argv)
{
    vector<string> arg( argv, argv+argc);
    if (arg.size() < 1) {
        println( "No arguments");
        return 1;
    }
    process_arguments(arg);

    return 0;
}