#include "create_suffix.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <omp.h>

const std::string corpus = "all_corpus";
long long longest = 0;
	std::vector <std::string> long_phrase;


trie* readCorpus() {
	trie* global = new trie;
	std::vector <std::vector <std::string> > phrases;
	std::vector <std::string> phrase;
	std::string word;
	std::ifstream fin(corpus.c_str());
	while (fin >> word) {
		if (word != "." && phrase.size() < 40) phrase.push_back(word);
		else {
			phrases.push_back(phrase);
			if (phrase.size() > longest) longest = phrase.size(), long_phrase = phrase;
			phrase.clear();
		}
	}
	std::cerr << phrases.size() << " phrases read" << std::endl;
	std::cerr << longest << std::endl;
	for (int i = 0; i < long_phrase.size(); i ++)
		std::cerr << long_phrase[i] << " ";
	std::cerr << std::endl;

//#pragma omp parallel for
	for (unsigned int k = 0; k < phrases.size(); k ++) {
		std::stringstream strout;
		std::vector <std::string> phrase = phrases[k];
		for (int i = 0; i < phrase.size(); i ++) {
			strout << phrase[i];
			global->insert(strout.str());
			if (i + 1 < phrase.size()) strout << " ";
		}
	}
	return global;
}
