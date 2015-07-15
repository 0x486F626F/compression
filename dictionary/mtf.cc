#include "mtf.h"

#include <algorithm>
#include <iostream>

void mtf::insert(const std::string &word) {
	std::vector <std::string>::iterator it = std::find(dict.begin(), dict.end(), word);
	if (it != dict.end()) dict.erase(it);
	dict.insert(dict.begin(), word);
}

unsigned int mtf::index(const std::string &word) {
	std::vector <std::string>::iterator it = std::find(dict.begin(), dict.end(), word);
	if (it == dict.end()) return 0xffffffff;
	return it - dict.begin();
}

std::string mtf::word(const unsigned int index) {
	if (index < dict.size()) return dict[index];
	return "OUT_OF_RANGE";
}
