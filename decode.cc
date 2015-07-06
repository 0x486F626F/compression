#include "decode.h"
#include "wordclass.h"
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

// LEVEL OF OUTPUT COMMENTS:
bool COMMENT = true;


// ********************* TO BE PROVIDED (the following are temporary implementations) ***************

// searches for the word of length len, with the revealed characters given by revealed,
// and the word at index in the list of possible candidates
string get_word(string s, queue <unsigned int> revealed, unsigned int rank){
        return " DECODED GLOBAL,";
}

// returns the word stored at index in the local dictionary
string searchDecodeLocal(int index){
        return " DECODED LOCAL,";
}


// **************************************************************************************





// converts first number from in to an int
int convertToInt(istringstream & in){
        // length of number in binary, obtained by reading all 0s at the start
        int i = 1;
        char c;
        while(in >> c && c == '0') i++;

        // the number in binary
        stringstream num;
        while(i > 0){
                in >> c;
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


// decodes the first word from in, given that it
// was encoded using the global dictionary
"decode.cc" 328L, 7429C                                                                                                                                                                  1,1           Top
