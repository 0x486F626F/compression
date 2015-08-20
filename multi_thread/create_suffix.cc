#include "create_suffix.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <omp.h>

const std::string corpus = "all_corpus";

trie* readCorpus() {
	trie* global = new trie;
	std::vector <std::vector <std::string> > phrases;
	std::vector <std::string> phrase;
	std::string word;
	std::ifstream fin(corpus.c_str());
	while (fin >> word) {
		if (word != ".") phrase.push_back(word);
		else {
			phrases.push_back(phrase);
			phrase.clear();
		}
	}
	std::cerr << phrases.size() << " phrases read" << std::endl;

#pragma omp parallel for
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
