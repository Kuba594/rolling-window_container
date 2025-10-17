#include <print>
#include <iostream>
#include <string>

#include "Counting.h"
using namespace std;

constexpr char EOL = '\n';
constexpr char DOT = '.';
constexpr char QUESTION = '?';
constexpr char EXCLAMATION = '!';



void InputProcesser::process_number(const string& word) {
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
	empty_sentence_ = false;
	++pt_.wordcount;
}

//process input
void InputProcesser::process(char c) {
	++pt_.charcount;
	if (isalnum(c)) {
		empty_row_ = false;
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
void InputProcesser::fce(istream& s) {
	char c;
	for (;;) {
		c = s.get();
		if (s.fail()) {
			if (empty_row_)
				return;
			++pt_.rowcount;
			empty_row_ = true;
			empty_sentence_ = true;
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
void InputProcesser::printoutput() {
	println("znaku: {}", pt_.charcount);
	println("slov: {}", pt_.wordcount);
	println("vet: {}", pt_.sentencecount);
	println("radku: {}", pt_.rowcount);
	println("cisel: {}", pt_.numbercount);
	println("soucet: {}", pt_.numbersum);
}