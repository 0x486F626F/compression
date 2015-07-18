#include "decode.h"
#include "wordclass.h"
#include "mtf.h"
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// LEVEL OF OUTPUT COMMENTS:
bool COMMENT = false;


// converts first number from in to an int
int convertToInt(istringstream & in){
	// length of number in binary, obtained by reading all 0s at the start
	int i = 1;
	char c;
	while(in >> c && c == '0') i++;
	// the number in binary
	stringstream num;
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

        return total;
}

// decodes the first word from in, given that it
// was encoded using the global dictionary
string decodeGlobal (istringstream & in, trie * GlobalSuffixTrie) {
	// did this encoding use the last letter of the prev word?
	bool usedLast = false;

	// encoding to be added before guesses (if !usedLast) this is ""
	// otherwise it is lastLetter + " "
        string start = "";

	// determines if usedLast is true or false based on first 2 bits
	// (if they are "10" then true, else false)
	char c = in.peek();
	if(c == '1'){
		in >> c;
		c = in.peek();
		if(c == '0'){
			in >> c;
			usedLast = true;

			// decodes the last letter
			int t = convertToInt(in);
	                if(ENCODINGCHARS == 1) c = intToChar(t);
        	        else {
                	        if(t == 27) c = ' ';
                        	else c = t + 'a' - 1;
                	} // eles
			ostringstream o;
			o << c << " ";
			start = o.str();
			if(COMMENT) cout << "first letter " << c << endl;
		} else {
			in.putback(c);
		} // else
	} // if

	// the number of revealed chars
	int numReveals = convertToInt(in);

	if(COMMENT) cout << "NUM REVEALS : " << numReveals << endl;

	// set of indices of non-revealed characters 
	queue< unsigned int > revealedQueue;

	// guessed word (non-revealed chars are _)
	ostringstream guessed;

	// current index in the word
	int index = -1;

	// decodes all revealed chars and determines their indices
	while(numReveals > 0) {

		// position difference from current value of index
		int t1 = convertToInt(in);
		// the revealed char, as an int
		int t2 = convertToInt(in);

		for(int j = index + 1; j < index + t1; j++){
			revealedQueue.push(j);
			guessed << '_';
		}

		index += t1;

		// determines the revealed char
		char c;
		if(ENCODINGCHARS == 1) c = intToChar(t2);
		else {
			if(t2 == 27) c = ' '; 
			else c = t2 + 'a' - 1;
		}

		guessed << c;

		if(COMMENT) cout << "char : " << c << ", pos diff : " << t1 << " , current index : " << index << endl;

		numReveals--;
	} 

	// position difference to the end of the word
	int t1 = convertToInt(in);

        for(int j = index + 1; j < index + t1; j++){
                revealedQueue.push(j);
        	guessed << '_';
       	}

	// adds last position difference to index, to get the length of the word
	index += t1;

	int globalIndex = convertToInt(in)-1;

	string guessedWord = start + guessed.str();

	if(COMMENT) cout << "len : " << index << " , rank : " << globalIndex << " , guessed: " << guessedWord << endl; 
	return GlobalSuffixTrie->get_word(guessedWord, revealedQueue, globalIndex,usedLast);
}


// decodes the first word from in, given that it
// was encoded using the local dictionary
string decodeLocal (istringstream & in, mtf * localDictionary) {
        int index = convertToInt(in) - 1;
	if(COMMENT) cout << "searching local at index " << index << endl;
	return localDictionary->word(index);
}


// decodes the first word from in, given that it
// was compressed with the standard scheme
string decodeNormal(istringstream & in) {
	// the length of the word
	int len = convertToInt(in);

	if(COMMENT) cout << "LEN : " << len << endl;

	// the result
	ostringstream res;

	// decodes char-by-char
	while(len > 0){
		int t = convertToInt(in);
		char c;
		if(ENCODINGCHARS == 1)  c = intToChar(t);
		else {
			if(t == 27) c = ' ';
			else c = t + 'a' - 1; 
		}
		res << c;
		len--;
	}
	
	return res.str();
}


// decodes the first phrase in the binary string in
string decodePhrase(istringstream & in, trie * GlobalSuffixTrie, mtf * localDictionary){

	// the number of word groups in this phrase
	int numGroups = convertToInt(in);

	if(COMMENT) cout << "# word groups : " << numGroups << endl;

	// the result
	ostringstream res;

	// word groups (to be inserted in the local dict)
	queue<string> words;

	// decodes word group by word group
	while(numGroups > 0){
		// reads in the first bit
		char c;
		in >> c;

		string result;

		// if the first bit is 0, this word group is encoded
		// with the global dictionary scheme
		if(c == '0') result = decodeGlobal(in, GlobalSuffixTrie);
		else {  // if the first bit is 1
			c = in.peek();

			// if the first 2 bits are 10 then this word group is encoded
                	// with the local dictionary scheme
			if(c == '0') result = decodeLocal(in, localDictionary);
			else {  // if the first 2 bits are 11
				in >> c;
				c = in.peek();

				// if the first 3 bits are 110 then this word group is encoded
                        	// with the standard scheme
				if(c == '0'){
					in >> c;
					result = decodeNormal(in);
				} else {
					// if the first 3 bits are 111 then this word group is encoded
                                	// with the local dictionary scheme
					in.putback('1');
					result = decodeLocal(in, localDictionary);
				} // if
			} // if
		} // if

		// adds result to word groups
		words.push(result);

		res << result;


		numGroups--;
		if(numGroups != 0) res << " ";
	} // while

	// inserts word groups in the local dictionary
	while(!words.empty()){
		// current word group
		string group = words.front();
		words.pop();

                if(COMMENT) cout << "local inserting \"" << group << "\"" << endl;
                localDictionary->insert(group,NULL);

                istringstream tmpIn(group.c_str());
                string tm;
                while(tmpIn >> tm){
                        if(tm != group){
                                if(COMMENT) cout << "local inserting \"" << tm << "\"" << endl;
                                localDictionary->insert(tm,NULL);
                        } // if
                } // while
	} // while

	return res.str();
}


// recovers spaces from simplified using the bits from in
// and returns the result
string addSpaces(string simplified, istringstream & in){
        istringstream read (simplified);
        char c;
	int n;

        ostringstream res;

	// reads the # spaces at start from in
        n = convertToInt(in) - 1;
        // adds this number of spaces
        while(n > 0){
                res << ' ';
        	n--;
        } // while

	// was the last char a period?
	bool period = false;

	// reads char-by-char
        while(read.get(c)){
		// outputs all non-space and non-period chars
        	if(c != ' ' && c != '.') {
			res << c;
			period = false;
		} else {  // reads the next number (# spaces before a perido if c == '.') from in
			n = convertToInt(in) - 1;
			// adds this number of spaces
			while(n > 0){
				res << ' ';
				n--;
			} // while
			// if c is a period, outputs it and the # spaces after it; otherwise c is a space (so 
			// has already been outputted)
			if(c == '.') {
				period = true;
				res << c;
				// reads the next number from in
	                        n = convertToInt(in) - 1;
        	                // adds this number of spaces
                	        while(n > 0){
                        	        res << ' ';
                                	n--;
				} // while
			} else period = false;
		} // else
        } // while

	if(!period){
		// adds spaces at end
		n = convertToInt(in) - 1;
	        while(n > 0){
		        res << ' ';
                	n--;
	        } // while
	}

        return res.str();
}


// recovers upper case letters and commas from simplified using 
// the bit vector from in and returns the result
string unsimplify(string simplified, istringstream & in){
	istringstream read (simplified);
	char c, n;

	ostringstream res;

	// reads char-by-char
	while(read.get(c)){
		// reads next bit from in
		in >> n;
		// if bit is 0, does nothing
		if(n == '0'){
			res << c;
		} else if(c == ' '){
			// if bit is 1 and c is a space, reads next bit
			in >> n;
			if(n == '0') { // if "10" -> comma
				res << ',';
			} else res << '.';
		} else {
			// if bit is 1 and c is not a space, converts c to upper case
			char temp = c - 'a' + 'A';
			res << temp;
		} // if
	} // while
	
	return res.str();
}


// decodes the entire binary string from in
string decodeText(istringstream & in, trie * GlobalSuffixTrie){
	// local dictionary
	mtf * localDictionary = new mtf;

	// the result
	ostringstream res;

	// gets the first bit
	char c;
	in >> c;
	// if bit is '1', text ends with '.'; otherwise it does not
	bool dot = (c == '1');

	// indicates if this is the initial iteration of the while loop
	bool start = true;

	// decodes phrase by phrase
	while(in >> c){
		char t = in.peek();
		// if reading bit vector
		if(c == '1' && t == '0') {
			in >> t;
			break;
		} else {// if reading phrase
			if(start){
				start = false;
			} else res << '.';
			in.putback(c);
			res << decodePhrase(in, GlobalSuffixTrie, localDictionary);
		} // if
	} // while

	// adds . if needed at the end
	if(dot) res << '.';

	// simplified result
	string simplified = res.str();

        if(COMMENT) cout << "SIMPLIFIED : \"" << simplified << "\"" << endl;

	// recovers multiple spaces
	string result = addSpaces(simplified,in);
	if(COMMENT) cout << "ADDED SPACES : \"" << result << "\"" << endl;

	// decodes the rest (a bit vector) with RLE
	string rleRest = rleDecode(in);
	istringstream in2(rleRest.c_str());

	// recovers upper-case letters and commas
	result = unsimplify(result,in2);
	
	delete localDictionary;

	return result;
}
