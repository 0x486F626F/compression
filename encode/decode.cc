#include "decode.h"
#include "wordclass.h"
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// LEVEL OF OUTPUT COMMENTS:
bool COMMENT = true;


// ********************* TO BE PROVIDED (the following are temporary implementations) ***************

// returns the word stored at index in the local dictionary
string searchDecodeLocal(int index){
        return " DECODED LOCAL,";
}


// **************************************************************************************





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
		if(t2 == 27) c = ' '; 
		else c = t2 + 'a' - 1;

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

	int globalIndex = convertToInt(in);

	if(COMMENT) cout << "len : " << index << " , globalIndex : " << globalIndex << " , guessed: " << guessed.str() << endl; 

	return GlobalSuffixTrie->get_word(guessed.str(), revealedQueue, globalIndex);
}


// decodes the first word from in, given that it
// was encoded using the local dictionary
string decodeLocal (istringstream & in) {
        int index = convertToInt(in);
	return searchDecodeLocal(index);
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
		if(t == 27) c = ' ';
		else c = t + 'a' - 1; 
		res << c;
		len--;
	}
	
	return res.str();
}


// decodes the first phrase in the binary string in
string decodePhrase(istringstream & in, trie * GlobalSuffixTrie){
	// the number of word groups in this phrase
	int numGroups = convertToInt(in);

	// the result
	ostringstream res;

	// decodes word group by word group
	while(numGroups > 0){
		// reads in the first bit
		char c;
		in >> c;

		// if the first bit is 0, this word group is encoded
		// with the global dictionary scheme
		if(c == '0') res << decodeGlobal(in, GlobalSuffixTrie);
		else {  // if the first bit is 1
			c = in.peek();

			// if the first 2 bits are 10 then this word group is encoded
                	// with the local dictionary scheme
			if(c == '0') res << decodeLocal(in);
			else {  // if the first 2 bits are 11
				in >> c;
				c = in.peek();

				// if the first 3 bits are 110 then this word group is encoded
                        	// with the standard scheme
				if(c == '0'){
					in >> c;
					res << decodeNormal(in);
				} else {
					// if the first 3 bits are 111 then this word group is encoded
                                	// with the local dictionary scheme
					in.putback('1');
					res << decodeLocal(in);
				} // if
			} // if
		} // if

		numGroups--;
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

	// reads char-by-char
        while(read.get(c)){
		// outputs all non-space and non-period chars
        	if(c != ' ' && c != '.') res << c;
		else {  // reads the next number from in
			n = convertToInt(in) - 1;

			// adds this number of spaces
			while(n > 0){
				res << ' ';
				n--;
			} // while
			// if c is a period, outputs it; otherwise c is a space (so 
			// has already been outputted)
			if(c == '.') res << c;
		} // else
        } // while

	// adds spaces at end
	n = convertToInt(in) - 1;
        while(n > 0){
	        res << ' ';
                n--;
        } // while


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
			res << decodePhrase(in, GlobalSuffixTrie);
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

	// recovers upper-case letters and commas
	result = unsimplify(result,in);
	
	return result;
}
