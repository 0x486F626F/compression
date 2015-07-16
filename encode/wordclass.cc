#include "wordclass.h"

using namespace std;

CompressedWords::CompressedWords() {
        words = "";
        usesLocalDict = false;
        ratio = -1.0;
        numLetters = 0;
}


// CompressedPhrase desctructor
CompressedPhrase::~CompressedPhrase(){
        // deallocates memory for all word CompressedWords in WordsSet
        for(vector<CompressedWords *>::iterator it = WordsSet.begin(); it != WordsSet.end(); it++){
                delete *it;
        }
        // deallocates memory for splits array
        delete [] splits;
        WordsSet.clear();
}

// converts the number n to a binary string
// by appending log n - 1 zeros and a 1 to the left of
// n conveted to binary
string convertToBinary(int n) {
        // the result
        string r = "";
        while(n!=0) {
                string add;
                if(n % 2 == 0) add = "0";
                else add = "1";
                r = add + r;
                n /= 2;
        }
        // the length of n converted to binary
        int len = r.length();

        // appends 1 and log n - 1 zeros to the start of r
        r = "1" + r;
        for(int i = 1; i < len; i++) { r = "0" + r;}

        return r; 
}
