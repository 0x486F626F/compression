#include "mtf.h"
#include <algorithm>
#include <iostream>

mtf::~mtf(){
	std::vector< std::pair <std::string, CompressedWords*> >::iterator it;
        for(it = dict.begin(); it != dict.end(); it++){
                delete it->second;
        }

}


void mtf::insert(const std::string &word,CompressedWords * cp) {
	std::vector< std::pair <std::string, CompressedWords*> >::iterator it;
        for(it = dict.begin(); it != dict.end(); it++){
                if(it->first == word && (cp == NULL || it->second->prevLetter == cp->prevLetter)) break;
        }

	if (it != dict.end()) {
		delete it->second;
		dict.erase(it);
	}
	
	// adds new entry at the start
	std::pair<std::string, CompressedWords*> p;
	p.first = word;
	p.second = cp;

	dict.insert(dict.begin(), p);
}

unsigned long long mtf::index(const std::string &word) {
        std::vector< std::pair <std::string, CompressedWords*> >::iterator it;
	for(it = dict.begin(); it != dict.end(); it++){
		if(it->first == word) break;
	}

	if (it == dict.end()) return 0xffffffff;
	return it - dict.begin();
}

std::string mtf::word(const unsigned long long index) {
	if (index < dict.size()) return dict[index].first;
	return "OUT_OF_RANGE";
}

// returns the associated CompressedWord with word
CompressedWords* mtf::findBest(const std::string &word, char lastLetter){
	// searches for word
        std::vector< std::pair <std::string, CompressedWords*> >::iterator it;
        for(it = dict.begin(); it != dict.end(); it++){
                if(it->first == word && it->second->prevLetter == lastLetter) break;
        }

        if (it == dict.end()) return NULL;
        return it->second;

}
