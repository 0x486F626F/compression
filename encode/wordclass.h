#ifndef __WORDCLASS_H__
#define __WORDCLASS_H__
#include <vector>
#include <string>

class CompressedWords{

  public:
        std::string words;
        std::vector<int> revealedChars;
	int numLetters;
        std::string compressedString;
        float ratio;
        bool usesLocalDict;
	std::string encodingScheme;
        CompressedWords();
};

class CompressedPhrase {

  public:
        int numberSplits;
        int * splits;
        std::vector<CompressedWords *> WordsSet;
        float totalRatio;
        ~CompressedPhrase();

};


std::string convertToBinary(int n);

#endif
