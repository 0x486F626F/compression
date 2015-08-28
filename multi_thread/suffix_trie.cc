#include "suffix_trie.h"

#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>

long long trie_size = 0;

word_counter::word_counter(const unsigned long long &freq, const std::string &word): freq(freq), word(word) {}

bool word_counter::operator < (const word_counter &other) const {
	if (freq == other.freq) return word < other.word;
	return freq > other.freq;
}

trie_node::trie_node() {
	count = 0;
	for (int i = 0; i < 27; i ++)
		child[i] = NULL;
}

trie_node::~trie_node() {
	for (int i = 0; i < 27; i ++)
		if (child[i]) delete child[i];
}

trie::trie(): root(NULL) {}

trie::~trie() { delete root; }

void trie::_insert(trie_node* &current, std::string s) {
	if (!current) current = new trie_node(), trie_size ++;
	current->count ++;
	if (s.size()) {
		unsigned int idx = s[0] == ' ' ? 26 : s[0] - 'a';
		_insert(current->child[idx], s.substr(1, s.size() - 1));
	}
}

void trie::insert(const std::string &s) {
	_insert(root, std::string(s.rbegin(), s.rend()));
}

bool trie::approx_match(std::string s, const unsigned int max_mismatch) {
	s = std::string(s.rbegin(), s.rend());
	std::queue <std::pair <trie_node*, std::pair <unsigned int, unsigned int> > > q;
	q.push(std::make_pair(root, std::make_pair(0, max_mismatch)));
	while (!q.empty()) {
		trie_node* current = q.front().first;
		unsigned int next_char_idx = q.front().second.first;
		unsigned int mismatch = q.front().second.second;
		q.pop();
		if (next_char_idx < s.size()) {
			unsigned int idx = s[next_char_idx] == ' ' ? 26 : s[next_char_idx] - 'a';
			if (current->child[idx]) 
				q.push(std::make_pair(current->child[idx], std::make_pair(next_char_idx + 1, mismatch)));
			if (mismatch) {
				for (int i = 0; i < 26; i ++)
					if (s[next_char_idx] - 'a' != i && current->child[i])
						q.push(std::make_pair(current->child[i], std::make_pair(next_char_idx + 1, mismatch - 1)));
				if (s[next_char_idx] != ' ' && current->child[26])
					q.push(std::make_pair(current->child[26], std::make_pair(next_char_idx + 1, mismatch - 1)));
			}
		}
		else if (current->count) return true;
	}
	return false;
}

std::set <word_counter> trie::_match_words(std::string s, std::queue <unsigned int> dontcare, bool usedLast) {
	std::queue <std::pair <trie_node*, std::string> > q;
	std::set <word_counter> matched_words;
	q.push(std::make_pair(root, ""));
	std::stack <unsigned int> reverse_dontcare;

	if(!usedLast) while (dontcare.size()) reverse_dontcare.push(s.size() - dontcare.front() - 1), dontcare.pop();
	else while (dontcare.size()) reverse_dontcare.push(s.size() - dontcare.front() - 3), dontcare.pop();

	while (!q.empty()) {
		trie_node* current = q.front().first;
		std::string word = q.front().second, next_word;
		q.pop();
		while (reverse_dontcare.size() && word.size() > reverse_dontcare.top()) reverse_dontcare.pop();
		if (reverse_dontcare.size() && word.size() == reverse_dontcare.top()) {
			for (int i = 0; i < 27; i ++) 
				if (current->child[i]) {
					next_word = word + char(i == 26 ? ' ' : 'a' + i);
					if (next_word.size() == s.size()) 
						matched_words.insert(word_counter(current->count, std::string(next_word.rbegin(), next_word.rend())));
					else q.push(std::make_pair(current->child[i], next_word));
				}
		}
		else {
			unsigned int idx = s[word.size()] == ' ' ? 26 : s[word.size()] - 'a';
			if (current->child[idx]) {
				next_word = word + s[word.size()];
				if (next_word.size() == s.size()) 
						matched_words.insert(word_counter(current->count, std::string(next_word.rbegin(), next_word.rend())));
				else q.push(std::make_pair(current->child[idx], next_word));
			}
		}
	}
	return matched_words;
}

long long trie::get_rank(std::string s, std::queue <unsigned int> dontcare, bool usedLast) {
	s = std::string(s.rbegin(), s.rend());
	std::set <word_counter> matched_words = _match_words(s, dontcare,usedLast);
	long long rank = 0;
	while (matched_words.size()) {
		if (matched_words.begin()->word == std::string(s.rbegin(), s.rend())) return rank;
		else rank ++, matched_words.erase(matched_words.begin());
	}
	return -1;
}

std::string trie::get_word(std::string s, std::queue <unsigned int> dontcare, unsigned long long rank, bool usedLast) {
	s = std::string(s.rbegin(), s.rend());
	std::set <word_counter> matched_words = _match_words(s, dontcare, usedLast);

	for (std::set <word_counter>::iterator i = matched_words.begin(); i != matched_words.end(); i ++, rank --) {
		if (rank == 0) {
			std::string currWord = i->word;
			std::string res;
			if(usedLast) res = currWord.substr(2);
			else res = currWord;
			return res;
		}
	}
	return "NOT_FOUND";
}

/*
unsigned long long numStar;
int number;
void trie::_load(std::istream & in, trie_node* &current) {
	std::string line = "";
	while(line == "" && !in.eof()) std::getline(in, line);
        std::istringstream currFreq(line.c_str());
        if(in.eof()) return;

std::cout << "at " << line << std::endl;
	if(numStar != 0) {
		numStar--;
		return;
	} else if(currFreq.peek() == '*') {
		std::string s;
		currFreq >> s >> numStar;
		numStar--;
		return;
	}

	unsigned long long freq;
	currFreq >> freq;

	if(!current){
		current = new trie_node();
		current->count = freq;
	}

	for(int k = 0; k < 27; k++){
		_load(in,current->child[k]);
number++;
	}
	
}

void trie::load() {
	std::ifstream in ("SuffixTrie");
	numStar = 0; number = 0;
	_load(in,root);
std::cout << "number " << number << std::endl;
}
*/

/*
bool LastStar;
int numStars;


int kl;
void trie::_traverse(std::ostream & out, trie_node* &current){
        if (!current) {
kl++;        	//out << "*" << std::endl;
if(kl == 1000000) {kl = 0; std::cout << "at *" << std::endl;}		
		if(!LastStar){
                        LastStar = true;
                        numStars = 1;
                } else numStars++;

		return;
	} else if(LastStar) {
        	out << "* " << numStars << std::endl;
                numStars = 0;
                LastStar = false;
	}
        out << current->count << std::endl;
        for(int k = 0; k < 27; k++) {
		_traverse(out, current->child[k]);
        }
}

void trie::traverse_trie(){

kl=0;	std::ofstream out ("SuffixTrie");

	LastStar = false;
	numStars = 0;

	_traverse(out,root);
}
*/
