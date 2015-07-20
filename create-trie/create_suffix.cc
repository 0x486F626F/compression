#include "create_suffix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

bool COMMENTS = false;


// given a corpus in a 
trie * readCorpus(){
	trie * Global = new trie;

string name = "preProsCorpus";
for (int k = 0; k  <= 55; k++){
	ostringstream thisName;
	thisName << name << k;
	string file = thisName.str();

	ifstream in (file);

	string s;

	while(in >> s){
		// reads all words in current phrase to the vector words
		vector<string> words;
		
		while(s != "."&& !in.eof()){
			words.push_back(s);
			in >> s;
		}

		// adds all word-prefixes of current phrase to the suffix trie
		int len = words.size();
		for(int i = 0; i <= len; i++){
			// current string to be inserted
			ostringstream out;
			for(int j = 0; j < i; j++){
				out << words[j];
				if(j != i - 1) out << " ";
			}
			string item = out.str();

			// inserts item in suffix trie
			if(item != "") {
				if(COMMENTS) cout << "\"" << item << "\"" << endl;
				Global->insert(item);
			} // if
		} // for
	} // while

}
	return Global;
}
