#ifndef COUNTING_H_
#define COUNTING_H_

struct ProcessedText {
	int charcount { 0 };
	int rowcount { 0 };
	int wordcount { 0 };
	int sentencecount { 0 };
	int numbercount { 0 };
	int numbersum { 0 };
};



class InputProcesser {
private:
	bool empty_row_ = true;
	bool empty_sentence_ = true;
	bool is_word_ = false;
	std::string word_;
	ProcessedText pt_;
	void process(char c);
	void process_word(const std::string& word);
	void process_number(const std::string& word);
public:
	void fce(std::istream& s);
	void printoutput();

};
#endif