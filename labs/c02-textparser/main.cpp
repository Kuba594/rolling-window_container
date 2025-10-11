#include <print>
#include <iostream>
#include <fstream>
using namespace std;

constexpr char EOL = '\n';
constexpr char DOT = '.';
constexpr char QUESTION = '?';
constexpr char EXCLAMATION = '!';

struct ProcessedText {
	int charcount = 0;
	int rowcount = 0;
	int wordcount = 0;
	int sentencecount = 0;
	int numbercount = 0;
	int numbersum = 0;
};



class InputProcesser {
private:
	bool empty_row_ = true;
	bool empty_sentence_ = true;
	bool is_word_ = false;
	string word_;
	ProcessedText pt_;
	void process(char c);
	void process_word(const string& word);
	void process_number(const string& word);
public:
	void fce(istream& s);
	void printoutput();

};

void InputProcesser::process_number(const string &word) {
	++pt_.numbercount;
	int num = stoi(word);
	pt_.numbersum += num;
}


//process word
void InputProcesser::process_word(const string& word) {
	if (isdigit(word[0])) {
		process_number(word);
		return;
	}
	++pt_.wordcount;
}

//process input
void InputProcesser::process(char c) {
	++pt_.charcount;
	if (isalnum(c)) {
		empty_row_ = false;
		empty_sentence_ = false;
		is_word_ = true;
		word_ += c;
		return;
	}
	if (is_word_) {
		process_word(word_);
		is_word_ = false;
		word_.clear();
	}
	switch (c) {
		case EOL:
			if (!empty_row_)
				++pt_.rowcount;
			empty_row_ = true;
			break;
		case DOT:
		case QUESTION:
		case EXCLAMATION:
			if (!empty_sentence_)
				++pt_.sentencecount;
			empty_sentence_ = true;
			break;
		default:
			break;
	}

}

//parse input
// parsuje std::cin nebo fstream, vnitrek fce nerozlisuje je tam istream&
void InputProcesser::fce(istream& s) {
	char c;
	for (;;) {
		c = s.get();
		if (s.fail()) {
			if (empty_row_)
				return;
			++pt_.rowcount;
			if (is_word_) {
				process_word(word_);
				is_word_ = false;
				word_.clear();
			}
			return;
		}
		process(c);
	}
}



//print output
//vytiskne hodnoty ProcessText
void InputProcesser::printoutput() {
	println("Chars: {}", pt_.charcount);
	println("Rows: {}", pt_.rowcount);
	println("Words: {}", pt_.wordcount);
	println("Sentences: {}", pt_.sentencecount);
	println("Numbers: {}", pt_.numbercount);
	println("Sum: {}", pt_.numbersum);

}


//vytvori ProcessedText
//nacte vstup
// zavola vytisteni outputu
int main() {
	InputProcesser x;
	ifstream f;
	f.open( "c02-textparser/input.txt");
	if( ! f.good()) {
		return 1;
	}
	x.fce(f);
	x.printoutput();

	return 0;
}