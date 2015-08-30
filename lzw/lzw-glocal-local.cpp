#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>  
#include <math.h>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

//Dictionaries will be divided into 3 parts: encoded with 2^3, 2^6, 2^10 bits
//try first with size_max = 2^10 and the dictionary parts will be 0 - 2^3, 2^3 - 2^6, 2^6 - 2^10
//then later with 2^32 = 4294967296 and dictionary parts: 0 - 2^6, 2^6 - 2^16, 2^16 - 2^32
#define size_max 1024 // try later 2^10 = 4294967296
#include "lzw-global-local.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Make the global dictionary from file output with path s, read from file output and use LZW to build the global dictionary
 * */
Dictionary* makeGlobal (){
    int i, j, index;
    char K;
	std::ostringstream wK;
    Dictionary* global = new Dictionary();
    std::ifstream input;
	std::ofstream output;
	input.open("input-global.txt");
	output.open("output-global.txt");

	/*
	* Construct the dictionary using LZW without outputing encoding the text 
	*/	bool found = false;
		std::string w("");
		index = -1;
        K = input.get();
		while(!input.eof() && K){
			w = wK.str(); //we need w for increasing freq, once w was found and the next substring wK is not found
			wK << K;

			index = global->return_index(wK.str());
			if (index > -1){ //if the word is already in the dictionary, keep on searching for longer string contained in dictionary
				K = input.get();
				found = true;//we need this for increasing frequencies (we won't increas freq for w, if w not found and wK not found
				//only increase freq of w, if w found and wK not found
            }
            else{
            	if (found) //we want to increase frequency not of every substring found, but only of the longest found (which is w here)
            	{
            		int id_prev = global->return_index(w); //w is at least a letter (so it must be in global, thus id_prev > -1)
            		global->list[id_prev]->freq++;
            	}
				global->insert(wK.str()); //if the word is not in the dictionary, insert it
				wK.str("");
				wK << K;
				K = input.get();
				found = false;
            }
        }

/*
* Once the dictionary has all the entries, sort decreasingly the entries by their frequencies
* (BubbleSort)
*/
	for(i = 0; i < global->lastID - 1; i++){
		for(j = 0; j < global->lastID - 1; j++){
					if(global->list[j]->freq < global->list[j+1]->freq){
						global->swap(j,j+1);
					}
		}
	}
			
    /*outputs global dictionary in a file output*/  
        for (i = 0; i < global->lastID; i++) {
			output << i << "," << global->list[i]->freq << "," << global->return_string(i) << "\n";
		}
  
	input.close();
	output.close();
	return global;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<char> binary(int index){

	std::vector<char> bit, temp;//max size 10 //later 32
	int i, number_digits;

	if (index < pow(2,3)){ //3 later becomes 6
		temp.push_back('0');
		number_digits = 3;
	}
	else{
		if(index < pow(2,6) ){ //6 later becomes 16
			temp.push_back('1');
			temp.push_back('0');
			number_digits = 6;
		}
		else{
			temp.push_back('1');
			temp.push_back('1');
			number_digits = 10; //10 later become 32
		}
	}

	for(i=number_digits-1; i>=0; i--)
	{
		//temp.push_back(index >> i & 1);
		temp.push_back(index >> i & 1? '1' : '0');
	}

	return temp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/*once we have D_G in a file, given a text from a file f, encode using LZW and create D_L*/

void encode_text(Dictionary* global, const char* s){
	int idglobal, idlocal;
    bool fromglobal;
	std::ifstream input;
	std::ofstream output_local;
	std::ofstream output_modified_global;
	std::ofstream output;
	Dictionary* local = new Dictionary();
	std::ostringstream wK;
	char K;
	std::string w;
	input.open(s); //for reading
	output.open("output-compress.txt");// for writing the compressed text
	output_local.open("output-local.txt");// for writing the local dictionary
	output_modified_global.open("output-modified-global.txt");// for writing the modified global dictionary
	std::vector<char> bit;
	int prev_id = 0; //something > -1
	//K = input.get();
	fromglobal = true;

	while((K = input.get()) != EOF){
		w = wK.str();
		wK << K;
		std::cout << "w:'" << w << "'" << std::endl;
		std::cout << "wK:'" << wK.str() << "'" << std::endl;

		idglobal = global->return_index(wK.str());//search for wK in the global dictionary
		if (idglobal > -1 && prev_id > -1){//if wK found in global dictionary
			//K = input.get(); //read next character
			fromglobal = true; //last found was from global
		}
		else{ //if wK not in global dictionary
			idlocal = local->return_index(wK.str());//search for wK in local dictionary
			if (idlocal > -1){//if wK found in local dictionary
				//K = input.get(); //try to find the longest contained in local
				fromglobal = false; //last found not from global, but from local
			}
			else{//if wK not in local dictionary and not in global
				//increase the frequency of previous found string (w) and movetofront if found in local and only movetofront if found in global
				//and in parallel encode(w) from the global or from the local (depending on fromglobal value)
				if(fromglobal){//means w was found in global
					prev_id = global->return_index(w);
					//encode prev_id in 3, 6 or 10 bits (depending on its value)
					output << '0'; //the encoding is done using global dictionary
					bit = binary(prev_id);
					for(int i=0; i<bit.size(); i++)
					{
						output << bit[i];
					}
					//movetofront w
					global->movetofront(prev_id);
				}
				else{//w was found in local
					prev_id = local->return_index(w);
					//encode prev_id in 3, 6 or 10 bits (depending on its value)
					output << '1'; //the encoding is done using local dictionary
					bit = binary(prev_id);
					for(int i=0; i<bit.size(); i++)
					{
						output << bit[i];
					}
					//increase the frequency of w and movetofront w
					local->list[prev_id]->freq++;
					local->movetofront(prev_id);
				}

				local->insert(wK.str());
				wK.str("");
				wK << K;
				//K = input.get(); //K reads the next letter
			}
		}
	}


	if(fromglobal){//means w was found in global
		//encode prev_id in 3, 6 or 10 bits (depending on its value)
		output << '0'; //the encoding is done using global dictionary
		bit = binary(idglobal);
		for(int i=0; i<bit.size(); i++)
		{
			output << bit[i];
		}
		//movetofront w
		global->movetofront(idglobal);
	}
	else{//w was found in local
		//encode prev_id in 3, 6 or 10 bits (depending on its value)
		output << '1'; //the encoding is done using local dictionary
		bit = binary(idlocal);
		for(int i=0; i<bit.size(); i++)
		{
			output << bit[i];
		}
		//increase the frequency of w and movetofront w
		local->list[idlocal]->freq++;
		local->movetofront(idlocal);
	}

	//output local dictionary in output_local
	for (int i = 0; i < local->lastID; i++) {
				output_local << i << "," << local->list[i]->freq << "," << local->return_string(i) << "\n";
			}
	for (int i = 0; i < global->lastID; i++) {
					output_modified_global << i << "," << global->list[i]->freq << "," << global->return_string(i) << "\n";
				}

	input.close();
	output.close();
	output_local.close();
	output_modified_global.close();

}
 
///////////////////////////////////////////////////////////////////////////////////////

/*decode the code using the 2 dictionaries*/
void decode_text(Dictionary* global){
    int bit, decimal = 0, number_digits;
	char first,second,third;
	std::ifstream input;
	input.open("input-decompress.txt");
	std::ofstream output;
	output.open("output-decompress.txt");
	std::ofstream output_local_decompress;
	output_local_decompress.open("output-local-decompress.txt");
	std::ofstream output_global_decompress;
	output_global_decompress.open("output-global-decompress.txt");
	Dictionary* local = new Dictionary();
    bool t = true, firsttimeglobal = true;
    std::string decoded_prev(""), decoded(""), decoded_to_insert("");
	int c = 0;

	    while((first = input.get()) != EOF){//!input.eof()){
           // first = input.get();//the first character tells us which dictionary to use (0 - global, 1 - local)
            // the second and third character: if 0 then read next 3 bits and decode, if 10 - read next 6bits, if 11 - read next 10 bits
            second = input.get();
            if(second == '1')
            {
                third = input.get();
                if(third == '0')
                {
                    number_digits = 6;//later change to 16;
                }
                else
                {
                    number_digits = 10;//later change to 32;
                }
            }
            else 
            {
                number_digits = 3;//later change to 6;
            }
            //read next number_digits characters and find its decimal meaning
            int temp = number_digits;
            decimal = 0;
            for(int i = 0; i < number_digits; i++)
              {
                bit = input.get();
                decimal += (bit - '0') * pow(2,temp - 1);
                temp--;
              }
            c++;
            //now decompress 'decimal' from global or local

            if (first == '0'){// from global dictionary        
                decoded = global->return_string(decimal);
				//output << decoded;
				global->movetofront(decimal);
				decoded_to_insert = decoded_prev.append(decoded.substr(0,1));
				if (!firsttimeglobal)
					local->insert(decoded_to_insert);
				firsttimeglobal = false;
            }
            else // from local dictionary (then we have to construct in parallel the local dictionary)
            {
            	if (t) //if first time reading from local(since we have no previous searched here)
                {
                	decoded = local->return_string(decimal);
                	decoded_to_insert = decoded_prev.append(decoded.substr(0,1));
                	local->insert(decoded_to_insert);
                    decoded_prev = decoded;
                    t = false;
                }
                else //not the first time using local(means we have a s_prev searched here)
                {
                    if(decimal == local->lastID)//if the index to search in local is empty (has no string associated): s = s_prev + s_prev[0]
                    {
                    	//then the string at previous index in local is copied at current index and the first character its copied at the end
                    	//ex: we need to decode 130 which has no string yet and at 129 its ABC then 130 becomes ABCA
                        //s = s_prev;
                    	decoded_to_insert = decoded.append(decoded_prev.substr(0,1));
						local->insert(decoded_to_insert);
                    }
                    else//if the index to search has a string associated
                    {
                        decoded = local->return_string(decimal);
                        decoded_to_insert = decoded_prev.append(decoded.substr(0,1));
                        local->insert(decoded_to_insert);//insert in local s_prev + s[0]
                    }
                }
                local->list[decimal]->freq++;
                local->movetofront(decimal);
                //prev_fromglobal = false;
            }
            output << decoded;
            decoded_prev = decoded;
        }


	    ///output local dictionary created during decompressing in output_local_decompress
	    for (int i = 0; i < local->lastID; i++) {
	    					output_local_decompress << i << "," << local->list[i]->freq << "," << local->return_string(i) << "\n";
	    				}

	    ///output global dictionary modified during decompressing in output_global_decompress
	    for (int i = 0; i < global->lastID; i++) {
	    					output_global_decompress << i << "," << global->list[i]->freq << "," << global->return_string(i) << "\n";
	    				}


	input.close();
    output.close();
    output_local_decompress.close();
    output_global_decompress.close();

}


