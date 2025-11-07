#include "frequencies.h"

using namespace std;
int main(int argc, char ** argv) {
    frequencies f;
    f.process_word("HELLO");
    f.process_word("HELLO");
    f.process_word("HELLO");
    f.process_word("HELLO");
    f.process_word("HELLO");
    f.process_word("HELL");
    f.process_word("HELL");
    f.process_word("HELL");
    f.process_word("HELLI");
    f.process_word("HELLI");
    f.process_word("HELLI");
    f.reformat_stored_frequencies();
    f.return_results(6);
    return 0;
}