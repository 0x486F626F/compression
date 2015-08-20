#include "wordclass.h"
#include <map>

#include <iostream>

using namespace std;


CompressedWords::CompressedWords() {
	words = "";
        usesLocalDict = false;
	usedLast = false;
        ratio = -1.0;
	numLetters = 0;
	prevLetter = '!';
}

// copy constructor
CompressedWords::CompressedWords(const CompressedWords& cp):
	words(cp.words),revealedChars(cp.revealedChars),numLetters(cp.numLetters),
	compressedString(cp.compressedString),ratio(cp.ratio),usedLast(cp.usedLast),
	usesLocalDict(cp.usesLocalDict),encodingScheme(cp.encodingScheme), prevLetter(cp.prevLetter) {}

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
string convertToBinary(int n, bool addOne) {
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

        // appends 1 (if addOne is true) and log n - 1 zeros to the start of r
        if(addOne) r = "1" + r;
        for(int i = 1; i < len; i++) { r = "0" + r;}

        return r;
}

// same as above, for long long
string convertToBinaryLong(long long n, bool addOne) {
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

        // appends 1 (if addOne is true) and log n - 1 zeros to the start of r
        if(addOne) r = "1" + r;
        for(int i = 1; i < len; i++) { r = "0" + r;}

        return r;
}


// encodes entire binary string from in using RLE
string rleEncode(istringstream & in){
	// initial bit
        char initial = in.peek();
        string result = (initial == '0') ? "0" : "1";

	// reads bit-by-bit
        while(in >> initial){
                char c;
                int count = 1;
		// adds # of repeated bits - 1 zeros
                while(in >> c && c == initial){
                        count++;
                }
                string bin = convertToBinary(count,false);
                result += bin;
		in.putback(c);
        }
	return result;
}

// decodes entire binary string from in using RLE
string rleDecode(istringstream &in){
	char first;
	in >> first;

	string result;

	while(1){
	        // length of number in binary, obtained by reading all 0s at the start
        	int i = 1;
	        char c;
        	while(in >> c && c == '0') i++;

	        // exits if no more input
		if(in.eof()) break;	

                // the number in binary
	        stringstream num;
		num << c;
		i--;
        	while(i > 0){
                	in >> c;
	                num << c;
        	        i--;
	        }

        	// the desired number in binary
	        int binary;
        	num >> binary;
	        // converts binary to int
        	int factor = 1;
	        // the result
        	int total = 0;

	        while (binary != 0) {
        	        total += (binary % 10) * factor;
                	binary /= 10;
	                factor *= 2;
        	}

		// adds first total times
		while(total > 0){
			result += first;
			total--;
		}
		if(first == '0') first = '1';
		else first = '0';
	}
	return result;
}

// initializes the map possibleChars to map
// chars in order of frequencies
void getMap(map<char,int> & possibleChars){
	possibleChars['e'] = 1;
        possibleChars['t'] = 2;
        possibleChars['a'] = 3;
        possibleChars['o'] = 4;
        possibleChars['i'] = 5;
        possibleChars['n'] = 6;
        possibleChars['s'] = 7;
        possibleChars['h'] = 8;
        possibleChars['r'] = 9;
        possibleChars['d'] = 10;
        possibleChars['l'] = 11;
        possibleChars[' '] = 12;
        possibleChars['c'] = 13;
        possibleChars['u'] = 14;
        possibleChars['m'] = 15;
        possibleChars['w'] = 16;
        possibleChars['f'] = 17;
        possibleChars['g'] = 18;
        possibleChars['y'] = 19;
        possibleChars['p'] = 20;
        possibleChars['b'] = 21;
        possibleChars['v'] = 22;
        possibleChars['k'] = 23;
        possibleChars['j'] = 24;
        possibleChars['x'] = 25;
        possibleChars['q'] = 26;
        possibleChars['z'] = 27;
        possibleChars['.'] = 28;
}

// returns c mapped to an integer
int charToInt(char c){
        map<char,int> possibleChars;
	getMap(possibleChars);
	return possibleChars[c];
}

// returns the char corresponding to n
char intToChar (int n){
        map<char,int> possibleChars;
	getMap(possibleChars);
	for( map<char,int>::iterator it = possibleChars.begin(); it!= possibleChars.end(); it++){
		if(it->second == n) return it->first;
	}
	return '\0';
}
