#include "wordclass.h"
#include "encode.h"
#include "decode.h"
#include "create_suffix.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int USELASTLETTER = 0;
int ENCODINGCHARS = 0;

// converts first number from in to an int
int convertInt(istringstream & in, bool addOne = true){
        // length of number in binary, obtained by reading all 0s at the start
        int i = 1;
        char c;
        while(in >> c && c == '0') i++;
        // the number in binary
        stringstream num;
        if(!addOne) in.putback(c);

        while(i > 0 && in >> c){
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


int main (){
	trie * GlobalSuffixTrie = readCorpus();
//cout << "READ" << endl;
//	GlobalSuffixTrie->traverse_trie();
	bool quit = false;
	string command = "";

	while(!quit){
		command = "";
		while(command != "encode" && command != "decode" && command != "quit"){
			cout << "Enter a command (encode, decode, quit) : ";
			cin >> command;
		} // while

		if(command == "encode") {
			string inputType = "";

                        while(inputType != "f" && inputType != "n"){
                                cout << "Do you want to use normal encoding scheme for chars, or use the frequencies of letters (n/f)?";
                                cin >> inputType;
                        }
                        if(inputType == "n") ENCODINGCHARS = 0;
                        else ENCODINGCHARS = 1;

                        inputType = "";

			while(inputType != "f" && inputType != "k"){
				cout << "Input from file \"input\" or from keyboard (f/k)? ";
				cin >> inputType;
			}	

			string Result;

			if(inputType == "k"){

				cout << "Enter the text to be compressed : ";
			        string s;
				getline(cin,s);
				getline(cin,s);
			        Result = bestCompression(s,GlobalSuffixTrie);
			} else {
				cout << "Read the file \"input\"" << endl;
				ifstream in("input");                                
				stringstream buffer;
				buffer << in.rdbuf();
				Result = bestCompression(buffer.str(),GlobalSuffixTrie);
			}
                        cout << "RESULT : " << endl << Result << endl;
                        ofstream out ("output");
                        out << Result; 

		} else if(command == "decode") {
			string inputType = "";

                        while(inputType != "f" && inputType != "n"){
                                cout << "Do you want to use normal encoding scheme for chars, or use the frequencies of letters (n/f)?";
                                cin >> inputType;
                        }
                        if(inputType == "n") ENCODINGCHARS = 0;
                        else ENCODINGCHARS = 1;

                        inputType = "";

                        while(inputType != "f" && inputType != "k"){
                                cout << "Input from file \"output\" or from keyboard (f/k)? ";
                                cin >> inputType;
                        }
                        
                        string Result;

                        if(inputType == "k"){
                        	cout << "Enter the binary string to be decoded : ";
                                string s;
                        	istringstream in (s);
                                Result = decodeText(in,GlobalSuffixTrie);
                        } else {
                                cout << "Read the file \"output\"" << endl;
                                ifstream inp("output");
                                stringstream buffer;
                                buffer << inp.rdbuf();
				istringstream in(buffer.str());

                                Result = decodeText(in,GlobalSuffixTrie);
                        }
                        cout << "RESULT : " << endl << "\"" << Result << "\"" << endl;
                        ofstream out ("message");
                        out << Result;

		} else quit = true;
	} // while
	delete GlobalSuffixTrie;
}

