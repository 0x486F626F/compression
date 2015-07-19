//this program will extract meaningful text from stdin and print the text in a format
//that matches how the suffix trie works(lowercase with single spaces and period is also seperate by single spaces)
//ex "The Man, Is F@100%^& tall." gets converted to "the man is tall . "
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool checkWord(string word){
	//word len>1 and word has no special chars inbetween it
	if(word.length()==0){
		return false;
	}
	for(int i=0; i<word.length();i++){
		if(word[i]<97||word[i]>122){
			return false;
		}
	}
	return true;
}

//idea keep writing to file rather than using stdout
int main(){
	string word,prevWord;
	char ch;
	char temp;
	while(cin.get(ch)){
		if(ch>='A'&&ch<='Z'){
			temp=ch+32;
			word+=(ch+32);//adds lower case to string
		}
		else if((ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||ch=='@'||ch=='$'||ch=='#'||
				ch=='+'||ch=='<'||ch=='>'||ch=='='||ch=='/'){
			word+=ch;
		}
		else if((ch>=0&&ch<=31)||ch==127){ //gets rid of special characters
			continue;
		}
		else if(ch==' '){
			if(checkWord(word)){
				cout<<word<<" ";
				prevWord=word;
				word="";
			}
			else{
				word=""; //clears the invalid word
			}
		}
		else if(ch=='|'||ch==','||ch==':'||ch==96||ch==39||ch==34||
				ch=='{'||ch=='}'||ch=='['||ch==']'||ch=='('||ch==')'||ch=='-'){
			continue;
		}
		else if(ch==';'||ch=='.'||ch=='!'||ch=='?'){
			if(checkWord(word)){//valid phrase if we include word
				cout<<word<<" . ";
				prevWord="";
				word="";
			}
			else if(!checkWord(word)&&prevWord.length()>0){ //valid phrase if we dont incldude word
				word="";
				prevWord="";
				cout<<". ";
			}
			else{ //not a valid phrase as no previous word
				word="";
			}
		}
	}
	if(word!=""){
		if(checkWord(word)){
			cout<<word<<" . ";
		}
	}
	else if(prevWord!=""){
		cout<<". ";
	}
}	
