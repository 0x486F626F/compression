#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <sstream>
#include "encode.h"
#include "wordclass.h"

using namespace std;


// LEVEL OF OUTPUT COMMENTS:
bool REPORT = true;  // very detailed
bool SUMMARY = true; // some details
bool END = true;     // few details


// ********************* TO BE PROVIDED (the following are temporary implementations) ***************


// searches local dict for text, returns index and then
// updates local dict
// if not found, returns -1
int searchLocalDict(string text){
        return 16025;
}

int c2 = 69;
// searches suffix tree for text using revealed (positions of revealed chars)
// and returns the pair (# of matches, match #)
// then updates the occurance count (in the suffix tree)
// if not found returns -1
int get_rank(string text, queue <unsigned int> revealed){
        int p = c2;
        c2--;
        if(c2 == 3) c2 = 140;
        return p;

}


// **********************************************************************************************







vector<int> position;
vector<int> combination;
vector< vector<int> > combinationList;
vector< vector<int> > combinationLetters;

// recursively adds combinations to the result, stored in combinationList
void recursiveCombinations(int offset, int k, bool splits) {
        if (k == 0) {
                if(splits) combinationList.push_back(combination);
                else combinationLetters.push_back(combination);
                return;
        }
        unsigned int end = position.size() - k;
        for (unsigned int i = offset; i <= end; ++i) {
            combination.push_back(position[i]);
            recursiveCombinations(i+1, k-1, splits);
            combination.pop_back();
        }
}

"encode.cc" 672L, 24099C                                                                                                                                                                 1,1           Top
