#ifndef __WORDCLASS_H__
#define __WORDCLASS_H__
#include <vector>
#include <string>

class CompressedWords{

  public:
        std::string words;
        std::vector <int> revealedChars;
        std::string compressedString;
        float ratio;
        bool usesLocalDict;
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
