#include <memory>
#include <vector>
#include <algorithm>
#define size_max 1024

/*A dictionary is represented as a trie and an array. Each leaf of the trie (which is an string from the dictionary) is double connected to exactly 
one element of the list (array). The index of the element is the code of the string in the dictionary.
*/

class Trie {
		public:
                std::vector<std::shared_ptr<Trie> > children;
				Trie* parent;
				std::string character;
				int index;
        
		
                Trie(Trie* parent_trie = NULL) {
						
						for(int i=0; i<128; i++)
						{
							children.push_back(std::shared_ptr<Trie>());
						}
                        //children.assign(128, NULL); //entire ASCII:  0-127 characters								
						parent = parent_trie;
						index = -1; 
						character.assign(""); 
                }

                /**
                 * Returns trie node of the given prefix.
                 */
                Trie* find(std::string s) {
						char c;
                        Trie* current = this;
						Trie* child = NULL;
                        for (int i = 0; i < s.length(); i++) {
                                c = s[i];
                                child = current->children[c].get();
								if (child != NULL) {
										current = child;
                                } else {
                                        return NULL;
                                }
                        }
                        return current;
                }
	/*
	* given a string s, it is inserted in the trie and it returns the new leaf
	*/
		Trie* insert(std::string s) {
				Trie* current = this;
				for (int i = 0; i < s.length(); i++) {
                    char c = s[i];	
					Trie* child = current->children[c].get();
					if (child != NULL) {
										current = child;
					} else {
						std::shared_ptr<Trie> new_child = std::shared_ptr<Trie>(new Trie(current));
						new_child->character = c;
						new_child->index = c;
						current->children[c] = new_child;
						current = new_child.get();
					}
				}
				return current;
		}
        

};

//////////////////////////////////////////////////////////////////////////////////

class Element{
	public:
			Trie* trie_leaf;
			int freq;
			
			Element(){
				freq = 0;
				trie_leaf = NULL;
			}
};

/////////////////////////////////////////////////////////////////////////////////

class Dictionary{//!!!
	public:
			std::shared_ptr<Trie> trie;
			std::vector<std::shared_ptr<Element> > list;
			int lastID; 
			
			Dictionary(){
				this->lastID = 0;
				this->trie = std::shared_ptr<Trie>(new Trie());
				for(int i=0; i<128; i++,this->lastID++){
					std::shared_ptr<Trie> t(new Trie(this->trie.get()));
					t->character = (char)i;
					t->index = this->lastID;
					this->trie->children[i] = t;
					std::shared_ptr<Element> e(new Element());
					e->trie_leaf = t.get();
					list.push_back(e);
				}
				
			}
			~Dictionary()
			{
			}
			
			  /**
                 * Inserts the given string in the dictionary: inserts the word s in the trie and returns the new leaf
				 then makes the doubles arrows between the new leaf and the last element in the list vector 
				 This function assumes that the string is missing in the trie
                 */
                void insert(std::string s){
					if(lastID < size_max){
						Trie* new_leaf = this->trie->insert(s); //inserts in the trie and returns the leaf
						std::shared_ptr<Element> e(new Element());
						e->trie_leaf = new_leaf;
						new_leaf->index = this->lastID; //the new leaf->index is the position in the vector	
						this->list.push_back(e); // the element of the list points towards the new leaf						
						this->lastID++;
					}
					else{
						std::cout<<"size-max reached!\n";
					}
				}
			                /**
                 * Returns the index(from the linked list) of the corresponding string s (in the trie) or -1 if the string is missing.
                 */
                int return_index(std::string s) {
                        Trie* t = this->trie->find(s);
                        if (t != NULL && this->list[t->index] != NULL){
								//this->list[t->index]->freq++;
								return t->index;
						}
						else return -1;
                }
				
				std::string return_string(int id){
					std::string s("");
					Trie* node = NULL;
					if (id < this->lastID) {
						node = this->list[id]->trie_leaf;
						s.append(node->character); //read the last character of the string from the trie
						while(node->parent != NULL){
							node = node->parent;
							s.append(node->character); //append the next character (in the reverse order going up the trie)
						}
						std::reverse(s.begin(), s.end());
						return s;
					}
					return "";
				}
				
				/*swaps two items in the array
				*/

			void swap(int i, int j){
				
				int index = this->list[j]->trie_leaf->index;
				this->list[j]->trie_leaf->index = this->list[i]->trie_leaf->index;
				this->list[i]->trie_leaf->index = index;
				this->list[i].swap(this->list[j]);
				
				//Element* elem1 = this->list[i];
				//Element* elem2 = this->list[j];
				//Element* temp = new Element();
				//temp = elem1;
				//elem1 = elem2;
				//elem2 = temp;
			}
			
			/*move to the front the element at position index in the array
			*/
			void movetofront(int index){
				int i;
				std::shared_ptr<Element> temp = this->list[index];
				if (index > 0)
				{
					for(i = index - 1; i >= 0; i--)
					{
						this->list[i+1] = this->list[i];
						this->list[i+1]->trie_leaf->index = i+1;
					}
					this->list[0] = temp;
					this->list[0]->trie_leaf->index = 0;
				}
			}
};


Dictionary* makeGlobal();
void encode_text(Dictionary* D, const char* s);
void decode_text(Dictionary* global);
