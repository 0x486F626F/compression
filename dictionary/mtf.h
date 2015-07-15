#ifndef __MTF_H__
#define __MTF_H__

#include <vector>
#include <string>

class mtf {
	private:
		std::vector <std::string> dict;
	public:
		void insert(const std::string &word);
		unsigned int index(const std::string &word);
		std::string word(const unsigned int index);
};

#endif
