#include "wordclass.h"
#include "encode.h"
#include "decode.h"
#include <iostream>
#include <sstream>

using namespace std;

int main (){

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
                        cout << bestCompression(s) << endl;
                } else if(command == "decode") {
                        cout << "Enter the binary string to be decoded : ";
                        string s;
                        cin >> s;

                        istringstream in (s);

                        cout << "RESULT : \"" << decodeText(in) << "\"" << endl;
                } else quit = true;
        } // while
}
