#ifndef __WORDCLASS_H__
#define __WORDCLASS_H__
#include <vector>
#include <string>
#include <sstream>



// 0 = use normal encoding scheme (a = 1, b = 2, ... z = 26, ' ' = 27)
// 1 = use frequency - encoding scheme
extern int ENCODINGCHARS;

class CompressedWords{

  public:
        std::string words;
        std::vector<int> revealedChars;
	int numLetters;
        std::string compressedString;
        float ratio;
	bool usedLast;
        bool usesLocalDict;
	std::string encodingScheme;
	char prevLetter;
        CompressedWords();
	CompressedWords(const CompressedWords& cp);
};

class CompressedPhrase {

  public:
        int numberSplits;
        int * splits;
        std::vector<CompressedWords *> WordsSet;
        float totalRatio;
        ~CompressedPhrase();

};


std::string convertToBinary(int n,bool addOne = true);

std::string convertToBinaryLong(long long n,bool addOne = true);

std::string rleEncode(std::istringstream & in);

std::string rleDecode(std::istringstream &in);

int charToInt(char c);

char intToChar (int n);

#endif
