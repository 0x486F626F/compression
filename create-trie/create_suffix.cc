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

	ifstream in ("corpus");

	string s;

	while(in >> s){
		vector<string> words;
		
		while(s != "."&& !in.eof()){
			words.push_back(s);
			in >> s;
		}
		int len = words.size();
		for(int i = 0; i <= len; i++){
			ostringstream out;
			for(int j = 0; j < i; j++){
				out << words[j];
				if(j != i - 1) out << " ";
			}
			string item = out.str();
			if(item != "") {
				if(COMMENTS) cout << "\"" << item << "\"" << endl;
				Global->insert(item);
			}
		}
	} 

	return Global;
}
