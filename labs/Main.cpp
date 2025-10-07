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
// mozne vstupy:
// Po98adsf asdf 98 7f f8.?..?
// 
// sadf.
// .
// .
// 
// 78
// asdf dasf
// 78 sadf7 8asdf.
// .
// adsf.dsafa8
// 9999999999999999999999
// 
// konec vstupu
//

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
void process(char c) {

}

//parse input
// parsuje std::cin nebo fstream, vnitøek fce nerozlisuje je tam istream&
void fce(istream& s) {
	char c;
	string word;
	for (;;) {
		c = s.get();
		s >> word;
		if (s.fail())
			return;
		process(c);
	}
}



//print output
//vytiskne hodnoty ProcessText
void printoutput() {

}


//vytvori ProcessedText
//nacte vstup
// zavola vytisteni outputu
int main() {
	ProcessedText x;
	return 0;
}