#include "wordclass.h"
#include "encode.h"
#include "decode.h"
#include "create_suffix.h"
#include <iostream>
#include <sstream>

using namespace std;

int main (){

	trie * GlobalSuffixTrie = readCorpus();
	bool quit = false;
	string command = "";

	while(!quit){
		command = "";
		while(command != "encode" && command != "decode" && command != "quit"){
			cout << "Enter a command (encode, decode, quit) : ";
			cin >> command;
		} // while

		if(command == "encode") {

			cout << "Enter the text to be compressed : ";
		        string s;
			getline(cin,s);
			getline(cin,s);
		        cout << "RESULT : " << endl << bestCompression(s,GlobalSuffixTrie) << endl;
		} else if(command == "decode") {
			cout << "Enter the binary string to be decoded : ";
			string s;
			cin >> s;

			istringstream in (s);

			cout << "RESULT : " << endl << "\"" << decodeText(in,GlobalSuffixTrie) << "\"" << endl;
		} else quit = true;
	} // while
	delete GlobalSuffixTrie;

}

