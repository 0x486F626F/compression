#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <sstream>
#include "encode.h"
#include "wordclass.h"

using namespace std;

// LEVEL OF OUTPUT COMMENTS:
bool REPORT = true;  // very detailed
bool SUMMARY = true; // some details
bool END = true;     // few details


// ********************* TO BE PROVIDED (the following are temporary implementations) ***************


// searches local dict for text, returns index and then
// updates local dict
// if not found, returns -1
int searchLocalDict(string text){
	return 16025;
}



// **********************************************************************************************





vector<int> position;
vector<int> combination;
vector< vector<int> > combinationList;
vector< vector<int> > combinationLetters;

// recursively adds combinations to the result, stored in combinationList
void recursiveCombinations(int offset, int k, bool splits) {
        if (k == 0) {
		if(splits) combinationList.push_back(combination);
		else combinationLetters.push_back(combination);
                return;
        }
	unsigned int end = position.size() - k;
        for (unsigned int i = offset; i <= end; ++i) {
            combination.push_back(position[i]);
            recursiveCombinations(i+1, k-1, splits);
            combination.pop_back();
        }
}

// finds all combinations of k items chosen from n items
// and stores the result in combinationList
void findAllCombinations(int n, int k, bool splits = true){

	position.clear();
	combination.clear();
	for (int i = 0; i < n; ++i) { 
		position.push_back(i+1); 
	}
        recursiveCombinations(0, k, splits);
}


// returns the compressed string for
// text if the standard (char-by-char) compression scheme is used
string normalCompression(string text){
	const char * word = text.c_str();
	unsigned int len = text.length();

        // compressed string
        string compressed = "110" + convertToBinary(len);

	// adds compressed characters to compressed
	for(unsigned int i = 0; i < len; i++){
		int c = word[i] - 'a' + 1;
		if(word[i] == ' ') c = 27; 
		string t = convertToBinary(c);
		if(REPORT) cout << "c : " << c << " -> " << t << endl;
		compressed += t;
	}
	if(REPORT) cout << "NORMAL : "<< compressed << endl;
	return compressed;
}


// searches for text in the local dictionary and returns
// the corresponding CompressedWords *
// normalLen is the length of the compressed string under the standard scheme
CompressedWords * tryLocalDict(string text, int normalLen){
	// compressed version of this phrase using local dict
        CompressedWords * bestWord = new CompressedWords;

        bestWord->words = text;

        // determines index using local Dictionary
        int localRes = searchLocalDict(text);

        string localCompressed = "";
        float localRatio = -1.0;

	// if text is found in local dictionary, determine localCompressed
        if(localRes != -1){
		// "1" indicates local dict is used
        	localCompressed = "1" + convertToBinary(localRes);
                int localLen = localCompressed.length();
                localRatio = 100.0 * localLen / normalLen; 
                if(REPORT) cout << "LOCAL (FOR \"" << text << "\"): localRes = " << localRes << " , localCompressed = " << localCompressed
                                << " , localLen = " << localLen << " , localRatio = " << fixed << setw(7) << setprecision(3) << localRatio << endl;
		else if(SUMMARY) cout <<  "LOCAL (FOR " << text << "): localRes = " << localRes << " , localCompressed = " << localCompressed
				<< " , localRatio = " << fixed << setw(7) << setprecision(3) << localRatio << endl;
        } else if (REPORT || SUMMARY) cout << "NOT FOUND IN LOCAL DICTIONARY" << endl;
        bestWord->compressedString = localCompressed;
        bestWord->ratio = localRatio;
        bestWord->usesLocalDict = (localRes != -1);
	return bestWord;
}


// tries all possible of revealed - chars combinations (with the global dictionary) for text
// and returns the best combination, with its winning ratio
// normalLen is the length of the compressed string for text using standard scheme
CompressedWords * tryAllLetters(string text, int normalLen, trie * GlobalSuffixTrie) {
	// the result
        CompressedWords * bestWord = new CompressedWords;

        bestWord->words = text;

	// length of text
	int len = text.length();	

	// text as an array
	const char * words = text.c_str();

	// for every possible number of revealed chars (ranging from 1 to len)
        for(int q = 1; q <= len; q++){
		if(REPORT) cout << "   FINDING LETTERS ; len: " << len << ", q: " << q << endl;
		// finds all combinations of choosing q items from len items, and stores it in
		// combinationLetters
                findAllCombinations(len, q, false);

		// for each combination
                for(vector< vector<int> >::iterator itera = combinationLetters.begin(); itera != combinationLetters.end(); itera++){
			if(REPORT) cout << "     Letters: " << endl << "     ";

                        // positions of revealed chars in ascending order
                        vector <int> currentRevealed;
			queue <unsigned int> currentRevealedQueue;

			// stores the chars corresponding to the positions given by the 
			// current combination in currentRevealed
			// and indices of non-revealed chars in currentRevealedQueue
			int ind = 0;
                        for(vector <int>::iterator iterat = (*itera).begin(); iterat != (*itera).end(); iterat++){
				for(int j = ind; j < (*iterat)-1; j++){
					currentRevealedQueue.push(j);
				}
				ind = (*iterat);
				if(REPORT) cout << words[(*iterat)-1] << " , ";
                                currentRevealed.push_back((*iterat)-1);
                        }
			if(REPORT) cout << endl;

                        for(int j = ind; j < len; j++){
                        	currentRevealedQueue.push(j);
			}
			// the index given by searching for text in the suffix tree using currentRevealed
                        int globalRes = GlobalSuffixTrie->get_rank(text, currentRevealedQueue);

			// if text is not found in global dictionary, done (return bestWord, with ratio -1)
			if(globalRes == -1) {
				if(REPORT) cout << text << " NOT FOUND in global dictionary => DONE " << endl;
				return bestWord;
			}

			// the string storing the compressed result for this combination
			// first stores the # of reveals
                        string globalCompressed = convertToBinary(q); 
			if(REPORT) cout << " # reveals : " << globalCompressed << " (" << q << ")"<< endl;

			// previous position guessed
                        int prev = 0;

                        // stores <position difference (relative to prev),char> in compressed string
			// for each revealed char
                        for(vector<int>::iterator IT = currentRevealed.begin(); IT != currentRevealed.end(); IT++){
				// current revealed char, stored as a number between 1 - 27
                        	char revealChar = words[(*IT)];
                                int revealInt;
                                if(revealChar == ' ') revealInt = 27; 
                                else revealInt = revealChar - 'a' + 1;

				string t1 = convertToBinary((*IT) - prev + 1);
				string t2 = convertToBinary(revealInt);

				if(REPORT) cout << "  adding " << t1 << " (" << (*IT) - prev + 1 << ") + " << t2 << " (" << revealInt << ")" << endl;
				// adds position difference and char to string
                                globalCompressed = globalCompressed + t1 + t2;
                                prev = (*IT) + 1;
                        } // for

			// adds the difference to the end of phrase and globalRes to the end of the string, and "0" 
			// to the start of the string to indicate the global dicitionary is used

			string t1 = convertToBinary(len - prev + 1);
			string t2 = convertToBinary(globalRes);

			if(REPORT) cout << "  end: " << t1 << " (" << len - prev + 1 << ") + index " << t2 << " (" << globalRes << ")" << endl;
                        globalCompressed = "0" + globalCompressed + t1 + t2; 

			// length of the compressed string
                        int globalLen = globalCompressed.length();
			// ratio
                        float globalRatio = 100.0 * globalLen / normalLen;

			if(REPORT) cout << "GLOBAL: globalRes = " << globalRes << " , globalCompressed = " << globalCompressed << endl << "  normalLen = " << normalLen 
					<< " , globalLen = " << globalLen << " , globalRatio = " << fixed << setw(7) << setprecision(3) << globalRatio << endl;

			// if the ratio for this combination is better than the best ratio so far, or 
			// this is the first combination of revealed chars tried, stores this combination
			// in bestWord
                        if(bestWord->ratio > globalRatio || bestWord->ratio == -1) {
                        	if(REPORT) cout << "OLD RATIO: " << bestWord->ratio << " worse than new ratio: "<< globalRatio << endl;
                                bestWord->compressedString = globalCompressed;
                                bestWord->ratio = globalRatio;
                                bestWord->usesLocalDict = false;
                                bestWord->revealedChars = currentRevealed;
                        } // if
                } // for

                combinationLetters.clear();
	} // for

	// prints best combination
	if(SUMMARY) {
		cout << "***" << endl << "BEST REVEAL FOR \"" << text << "\": " << bestWord->compressedString << " (ratio " << bestWord->ratio << "); guess : ";
		for(int i = 0; i < len; i++){
			if(bestWord->revealedChars.size() != 0 && bestWord->revealedChars[0] == i){
				cout << words[i];
				bestWord->revealedChars.erase(bestWord->revealedChars.begin());
			} else cout << "_ ";
		} 
		cout << endl << "***" << endl;
	}
	return bestWord;
}


// simplifies text (makes it all lower case, and removes commas)
// return (simplified text, bitVector) pair
pair <string,string> simplifyText(string text){
	string bitVector = "";

	istringstream in (text);

	ostringstream out;

	char c;
	// has there been any letter since the last period?
	bool lastPeriod = true;

	// reads text char-by-char
	while(in.get(c)){
		if(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && lastPeriod) lastPeriod = false;

		// indicates comma is "10"
		if(c == ',') {
			bitVector += "10";
			out << ' ';
		} else if(c >= 'A' && c <= 'Z') {
			// indicates upper case with "1"
			bitVector += "1";
			char t = c - 'A' + 'a';
			out << t;
		} else if(c == '.'){
			if(!lastPeriod) {
				// if first period, does nothing
				lastPeriod = true;
	                        bitVector += "0";
        	                out << c;
			} else {
				// if this is a period in a series of periods (with no letter between)
				// indicates it by "11", and transforms it into a space
				bitVector += "11";
				out << ' ';
			} // else
		} else {
			bitVector += "0";
			out << c;
		} // else
	} // while

	string simplified = out.str();

	if(REPORT) cout << "SIMPLIFIED \"" << text << "\" to \"" << simplified << "\" with bit vector " << bitVector << endl;

	pair <string, string> p;
	p.first = simplified;
	p.second = bitVector;

	return p;
}


// removes multiple spaces in text 
// returns (simplified text, set of # of spaces) pair
pair <string,string> removeSpaces(string text){
        string bits = "";

        istringstream in (text);

        ostringstream out;

        char c;
	
	// last char was space
	bool space = false;
	int numSpaces = 0;
        // reads text char-by-char
        while(in.get(c)){
                if(c != ' ' && ! space) {
			// if c is not a space and prev char was not a space, outputs c
			out << c;
			// if c is a period, stores the number of spaces before it, 0
			if(c == '.') bits += convertToBinary(1);
		} else if(c != ' ' && space) {
			// if c is not a space, but last char was a space
			space = false;

			// if c is a period, no space is outputted; otherwise outputs 1 space
			if (c == '.') out << c;
			else out << ' ' << c;

			// stores # of spaces
			bits += convertToBinary(numSpaces + 1);
			numSpaces = 0;
		} else { 
			// if c is a space, increments # spaces
			space = true;
			numSpaces++;
		} // if
        } // while

	// stores # of spaces at the end
	if(space) {
		bits += convertToBinary(numSpaces + 1);
	} else bits += convertToBinary(1);

        string simplified = out.str();

        if(REPORT) cout << "SIMPLIFIED SPACES: \"" << text << "\" to \"" << simplified << "\" with bits " << bits << endl;

        pair <string, string> p;
        p.first = simplified;
        p.second = bits;

        return p;
}



// returns the binary string for the best compression of text
string bestCompression (string text, trie * GlobalSuffixTrie){
	// final binary string, for text
	string finalRes = "";

	// simplifies text
	pair <string, string> p = simplifyText(text);
	text = p.first;
	string bits = p.second;

	// removes extra spaces from text
	p = removeSpaces(text);
        text = p.first;
        string bitVector = p.second + bits;

	int n = text.length();
	const char * T = text.c_str();

	// start of phrase in T
	int s = 0;

	// the last index in T that holds a letter (not '.')
	int bound;

	// indicates whether text ends with '.' ("1") or not;
	// in the latter case, indicates whether text ends with ' ' ("01") or not ("00")
	string dot;
	if(T[n-1] == '.') {
		bound = n - 1;
		dot = "1";
	} else {
		bound = n;
		dot = "0";
	}


	// compresses text, phrase by phrase	
	while (s < bound) {
        	//find a phrase P
		// current position in P
	        int t = 0;
		// # of words in P
        	int m = 1;

		// starting indices of the words in P
		int w [ 1 + (n - s + 1) / 2] ;	

		// finds the index of the end of the phrase (either '.' or the EOF)
		// and stores it in temp
		int temp = s;

		// finds the end of this phrase
		while(T[temp] != '.' && temp < bound) {
			temp++;
		}

		// stores the phrase (without '.') in P and
		// the starting indices of the words in w
		char P[temp - s];

		while (s < temp) {
			// first word in P starts at position 0
			w[1] = 0; 

	                P[t] = T[s];

        	        if (P[t] == ' '){
	                	m++; //counts the words
        	        	w[m] = t + 1; // next word starts at position t+1
		        }
	                t++;
        	        s++;
		} // while


		s++;

		// best compressedPhrase for this phrase (initialized to case where numSplits = 0)
		CompressedPhrase * best = new CompressedPhrase;
		// current compressedPhrase for this combination of splits and revealed chars
		CompressedPhrase * current = NULL;

	 	// ************** CASE 1: numSplits = 0 
		best->numberSplits = 0;
		best->splits = NULL;
                best->totalRatio = 100;

                // copy of the phrase (without '.')
                char words[t+1];

                if(REPORT) cout << "ENTIRE PHRASE (numSplits = 0): " << endl; 

                for(int q = 0; q < t; q++){
                	words[q] = P[q];
			if(REPORT) cout << words[q];
                }
                words[t] = '\0';
                if(REPORT) cout << endl;

		// the entire phrase as a string
                string thisWord(words);

		// the length of the compressed string when using the standard compression scheme
		string normalComp = normalCompression(thisWord);
                int normalLen = normalComp.length();

		// the compressed word using local dictionary
		CompressedWords * bestWord = tryLocalDict(thisWord,normalLen);

		// the best revealed-chars combination, gotten from the global dictionary
		CompressedWords * bestGlobal = tryAllLetters(thisWord,normalLen, GlobalSuffixTrie);

		// stores the better of bestGlobal and bestWord in bestWord, deletes the other
		if(bestGlobal->ratio != -1 && bestGlobal->ratio < 100 && (bestGlobal->ratio < bestWord->ratio || bestWord->ratio == -1)){
			if(REPORT) cout << "Global ratio " << bestGlobal->ratio << " < local ratio " << bestWord->ratio << endl;
			delete bestWord;
			bestWord = bestGlobal;
		} else if (bestWord->ratio != -1 && bestWord->ratio < 100 && (bestGlobal->ratio >= bestWord->ratio || bestGlobal->ratio == -1)){
			if(REPORT) cout << "Global ratio " << bestGlobal->ratio << " >= local ratio " << bestWord->ratio << endl;
			delete bestGlobal;
		} else {
			// if not found in either dictionary
			delete bestGlobal;
			if(REPORT) {
				if(bestWord->ratio == -1 && bestGlobal->ratio == -1) cout << "NOT FOUND IN EITHER DICTIONARY" << endl;
				else cout << "NORMAL SCHEME IS BETTER: Global ratio " << bestGlobal->ratio 
					<< ", local ratio " << bestWord->ratio << " , normal: " << normalLen << endl;
			}
			bestWord->ratio = 100.0;
			bestWord->compressedString = normalComp;
		}

                best->WordsSet.push_back(bestWord);
                best->totalRatio = bestWord->ratio;

		if(SUMMARY && bestWord->ratio == 100) cout << "***" << endl << "COMPRESSED WORD FOR \"" 
							<< thisWord << "\" : " << bestWord->compressedString << " (normal) 100" << endl << "***" << endl;
		else if(SUMMARY) cout << "***" << endl << "COMPRESSED WORD FOR \"" << thisWord << "\" : " << bestWord->compressedString << " (" 
				<< (bestWord->usesLocalDict ? "local" : "global") << ") " << bestWord->ratio << endl << "***" << endl;

		// ************** CASE 2: 1 <= numSplits <= m-1
		// tries to split the phrase p in all possible ways
		for(int numSplits = 1; numSplits <= m - 1; numSplits++){
			if(REPORT || SUMMARY) cout << "NUMSPLITS: " << numSplits << endl;

			// creates a list of all possible combinations of numSplits chosen from the m-1 splits
			findAllCombinations(m - 1, numSplits);

			// the indices of the current splits
			int splits[numSplits];

			// tries every combination of numSplits splits
			for(vector< vector<int> >::iterator it = combinationList.begin(); it != combinationList.end(); it++){
				if(REPORT) cout << "  ";

				// current split index
				int j = 0;
				// stores the indices of the splits
				for(vector<int>::iterator iter = (*it).begin(); iter!=(*it).end(); iter++){
					if(REPORT) cout << (*iter) << " , ";
					splits[j] = w[(*iter) + 1] - 1;
					j++;
				}
				if(REPORT) cout << endl;

				// compressed phrase for this combination
				current = new CompressedPhrase;

				current->splits = new int[numSplits];
				current->totalRatio = 0;

				// stores number of splits + positions of splits in current
				current->numberSplits = numSplits;
				for(int l = 0; l < numSplits; l++){
					(current->splits)[l] = splits[l];
				}

				// tries to compress each set of words
				for(int k = 0; k <= numSplits; k++){
					// length of current group of words
					int len;
					if(k == 0) len = splits[0];
					else if(k == numSplits) len = t - splits[numSplits-1] - 1;
					else len = splits[k] - splits[k-1] - 1;

					// current set of words
					char words[len+1];

					// prints current words
					if(REPORT || SUMMARY) cout << "CURRENT WORD # " << k << " (of " << numSplits << ") : ";

					// stores current set of words in words
					for(int q = 0; q < len; q++){
						if(k == 0) words [q] = P[q];
						else if(k == numSplits) words[q] = P[q + splits[numSplits-1] + 1];
						else words[q] = P[splits[k-1]+1+q];
						if(REPORT || SUMMARY) cout << words[q];
					}
					words[len] = '\0';
					if(REPORT || SUMMARY) cout << endl;

	                                string thisWord(words);

			                // the compressed string and its length when using the standard compression scheme
			                string normalComp = normalCompression(thisWord);
			                int normalLen = normalComp.length();

			                // the compressed word using local dictionary
			                CompressedWords * bestWord = tryLocalDict(thisWord,normalLen);

			                // the best revealed-chars combination, gotten from the global dictionary
			                CompressedWords * bestGlobal = tryAllLetters(thisWord,normalLen, GlobalSuffixTrie);

			                // stores the better of bestGlobal and bestWord in bestWord, deletes the other
			                if(bestGlobal->ratio != -1 && bestGlobal->ratio < 100 && (bestGlobal->ratio < bestWord->ratio || bestWord->ratio == -1)){
			                        if(REPORT) cout << "Global ratio " << bestGlobal->ratio << " < local ratio " << bestWord->ratio << endl;
			                        delete bestWord;
                        			bestWord = bestGlobal;
			                } else if (bestWord->ratio != -1 && bestWord->ratio < 100 && (bestGlobal->ratio >= bestWord->ratio || bestGlobal->ratio == -1)){
                        			if(REPORT) cout << "Global ratio " << bestGlobal->ratio << " >= local ratio " << bestWord->ratio << endl;
			                        delete bestGlobal;
			                } else {
                        			// if not found in either dictionary
			                        delete bestGlobal;
                        			if(REPORT) {
			                                if(bestWord->ratio == -1 && bestGlobal->ratio == -1) cout << "NOT FOUND IN EITHER DICTIONARY" << endl;
                        			        else cout << "NORMAL SCHEME IS BETTER: Global ratio " << bestGlobal->ratio 
								<< ", local ratio " << bestWord->ratio << " , normal: " << normalLen << endl;
                        			}
                        			bestWord->ratio = 100.0;
                        			bestWord->compressedString = normalComp;
                			}
					// adds bestWord to current
	                                current->WordsSet.push_back(bestWord);
	                                current->totalRatio += bestWord->ratio;
	
        			        if(SUMMARY && bestWord->ratio == 100) cout << "***" << endl << "COMPRESSED WORD FOR \"" << thisWord << "\" : " 
										<< bestWord->compressedString << " (normal) 100" << endl << "***" << endl;
                			else if(SUMMARY) cout << "***" << endl << "COMPRESSED WORD FOR \"" << thisWord << "\" : " << bestWord->compressedString 
							<< " (" << (bestWord->usesLocalDict ? "local" : "global") << ") " << bestWord->ratio << endl << "***" << endl;

				} // for
				if(REPORT) cout << "TOTAL RATIO for these splits: " << current->totalRatio << " ; best TOTAL : " << best->totalRatio << endl;
                                if(REPORT) cout << "AVG RATIO for these splits: " << current->totalRatio / (1 + current->numberSplits) << " ; best AVG : " 
					<< best->totalRatio / (1 + best->numberSplits) << endl;

				// if current has better ratio than best, stores current in best; else deletes current
				if(current->totalRatio  / (1+current->numberSplits) < best->totalRatio / (1 + best->numberSplits)) {
					delete best;
					best = current;
					current = NULL;
					if(REPORT) cout << "BEST REPLACED BY CURRENT" << endl;
				} else {
					delete current;
					current = NULL;
				}

			} // for

			combinationList.clear();
		} // for

		if(SUMMARY || REPORT) cout << "THE BEST FOR THIS PHRASE: avgRatio = " << best->totalRatio / (1 + best->numberSplits) << " (total " 
					<< best->totalRatio << " / (1 + numSplits " << best->numberSplits << ")) " << endl;

		if(SUMMARY || END) cout << "CONFIG : \""; 

		/// ******************* APPEND COMPRESSED PHRASE TO RESULT *********
		// adds # of word groups in this phrase
		finalRes = finalRes + convertToBinary(best->numberSplits + 1); 


		// adds each compressed string for the groups of words 
		for (vector<CompressedWords *>::iterator ITERAT = best->WordsSet.begin(); ITERAT != best->WordsSet.end(); ITERAT++){
			finalRes = finalRes + (*ITERAT)->compressedString;
			if(SUMMARY || END) cout << (*ITERAT)->words << "|" ;
		}
		if(SUMMARY || END) cout << "\"" << endl;

		delete best;


		combinationList.clear();
		combinationLetters.clear();

	} // while

	// adds dot indicator, and "10" (indicates end of phrases) + bitVector
	return dot + finalRes + "10" + bitVector;
}


