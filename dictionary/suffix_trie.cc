#include "suffix_trie.h"

#include <set>

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
	if (!current) current = new trie_node();
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

int trie::get_rank(std::string s, std::queue <unsigned int> revealed) {
	s = std::string(s.rbegin(), s.rend());
	std::queue <std::pair <trie_node*, std::string> > q;
	q.push(std::make_pair(root, ""));
	std::set <std::pair <unsigned int, std::string> > matched_words;
	while (!q.empty()) {
		trie_node* current = q.front().first;
		std::string word = q.front().second, next_word;
		q.pop();
		if (word.size() == revealed.front()) {
			revealed.pop();
			for (int i = 0; i < 27; i ++) 
				if (current->child[i]) {
					next_word = word + char(i == 26 ? ' ' : 'a' + i);
					if (next_word.size() == s.size()) matched_words.insert(std::make_pair(current->count, next_word));
					else q.push(std::make_pair(current->child[i], next_word));
				}
		}
		else {
			unsigned int idx = s[word.size()] == ' ' ? 26 : s[word.size()] - 'a';
			if (current->child[idx]) {
				next_word = word + s[word.size()];
					if (next_word.size() == s.size()) matched_words.insert(std::make_pair(current->count, next_word));
				else q.push(std::make_pair(current->child[idx], next_word));
			}
		}
	}

	int rank = 0;
	while (matched_words.size()) {
		if (matched_words.begin()->second == s) return rank;
		else rank ++, matched_words.erase(matched_words.begin());
	}
	return -1;
}

std::string trie::get_word(std::string s, std::queue <unsigned int> revealed, unsigned int rank) {
	s = std::string(s.rbegin(), s.rend());
	std::queue <std::pair <trie_node*, std::string> > q;
	q.push(std::make_pair(root, ""));
	std::set <std::pair <unsigned int, std::string> > matched_words;
	while (!q.empty()) {
		trie_node* current = q.front().first;
		std::string word = q.front().second, next_word;
		q.pop();
		if (word.size() == revealed.front()) {
			revealed.pop();
			for (int i = 0; i < 27; i ++) 
				if (current->child[i]) {
					next_word = word + char(i == 26 ? ' ' : 'a' + i);
					if (next_word.size() == s.size()) matched_words.insert(std::make_pair(current->count, next_word));
					else q.push(std::make_pair(current->child[i], next_word));
				}
		}
		else {
			unsigned int idx = s[word.size()] == ' ' ? 26 : s[word.size()] - 'a';
			if (current->child[idx]) {
				next_word = word + s[word.size()];
					if (next_word.size() == s.size()) matched_words.insert(std::make_pair(current->count, next_word));
				else q.push(std::make_pair(current->child[idx], next_word));
			}
		}
	}
	for (std::set <std::pair <unsigned int, std::string> >::iterator i = matched_words.begin(); i != matched_words.end(); i ++, rank --)
		if (rank == 0) return std::string(i->second.rbegin(), i->second.rend());
	return "NOT_FOUND";
}
