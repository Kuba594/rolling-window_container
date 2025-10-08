#include <print>
#include <iostream>
#include <fstream>
using namespace std;

struct ProcessedText {
	int charcout = 0;
	int rowcount = 0;
	int wordcount = 0;
	int sentencecount = 0;
	int numbercount = 0;
	int numbersum = 0;
};



class InputProcesser {
private:
	ProcessedText pt;
	void process(char c);
	void processnumber();
public:
	void fce(istream& s);
	void printoutput();

};

//process input
// z funkce parse input dostane chary a tady kontrola co to je
// inkrementace ProcessText, referenci
// slovo - nemezera->mezera, nemezera->konecradku, nemezera->tab, konec vety
// veta - .,!,?
// radka - jednoznacne
// znak - jasne
// cislo - muze byt kdekoliv posloupnost znaku
void InputProcesser::process(char c) {
	pt.charcout++;
	switch (c) {
		case '\n':
			pt.rowcount++;
			break;
		case '.':
			pt.sentencecount++;
			break;
		case '?':
			pt.sentencecount++;
			break;
		case '!':
			pt.sentencecount++;
			break;

	}

}

//parse input
// parsuje std::cin nebo fstream, vnitrek fce nerozlisuje je tam istream&
void InputProcesser::fce(istream& s) {
	char c;
	string word;
	for (;;) {
		c = s.get();
		//s >> word;
		if (s.fail()) {
			return;
		}
		process(c);
	}
}



//print output
//vytiskne hodnoty ProcessText
void InputProcesser::printoutput() {
	println("Chars: {}", pt.charcout);
	println("Rows: {}", pt.rowcount);
	println("Words: {}", pt.wordcount);
	println("Sentences: {}", pt.sentencecount);
	println("Numbers: {}", pt.numbercount);
	println("Sum: {}", pt.numbersum);

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