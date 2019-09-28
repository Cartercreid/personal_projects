#ifndef mRNA_h
#define mRNA_h
#include <iostream>
#include <string>
#include <vector>
using namespace std; 

//trie node for the trie tree
struct trie_node {
    char bp;
    trie_node* next[4];
    vector <string> RNA_ID;  
};
//solution table will be to recover our solutions effectively
struct solution_list{
    vector <float> percent_hit;
    vector <string> ID;
    vector <int> count;
    //change vector<vector<trie_node*>> to just a vector of trie_nodes and independently recover the solution if prompted to print the solution
    //vector <vector<trie_node*> > solution_start;
    vector <trie_node*> solution_start;
    //solution_list* next;
};

//add sequence will take a tree, and add a sequence to it.
void add_sequence(trie_node* root, string input, string sequence_name);
void add_recursive(trie_node* cursor, string input, string sequence_name, unsigned int index);
void add_child(trie_node* cursor, trie_node* child);
//check sequence will check if a sequence is in the tree and return all values greater than the 
//accuracy threshold. It will return the solution in the form of a solution table
solution_list* check_sequence(trie_node* root, string input, bool summary);
void outer_check_recursive(trie_node* current_node, int hit_thresh, string input, int problem_space, solution_list* solution, int node_counter);
bool check_recursive(trie_node* current_node, int hit_thresh, string input, int index, int problem_space, int miss, solution_list* solution, trie_node* start_node, int node_counter);
void add_solution(solution_list* solution, string input, int problem_space, int miss, trie_node* start_node, int node_counter);
bool not_in_colated_solution(string RNA_ID, solution_list* c_solution);
solution_list* collate_solution(solution_list* solution);
void print_sequence_match(trie_node* start, string input, string sequence_name);
bool import_data(trie_node* tree_root, string fn);
bool imort_human_flu_PB2(trie_node* new_tree);
bool imort_human_flu(trie_node* new_tree);
string import_sequence(string fn);
void change_percent_threshold();
void print_c_solution(solution_list* solution);
solution_list* BLAST(trie_node* data_tree, string location);
void sort_c_solution(solution_list* solution);
bool UI();
trie_node* get_next_node(trie_node* start, string sequence_name);
//init root initiallizes a special trie node with bp char '5' whic stands for 5' end of the mRNA. Really, though this is just for a 
//convenient starting point for the tree
trie_node* init_root();

//init node initializes a new node in the tree with a specific bp and RNA_ID
trie_node* init_node(char bp, string RNA_ID);

//init solution_list
solution_list* init_solution_list(float hit, string ID);

//terminate checks the current and next 6 nodes to see if they are all A's, if yes, then the tree is terminated and function
//returns true, else it returns false
bool terminate();

//checks the nodes children to see if they have a child contianing the next bp
trie_node* check_children(trie_node* current_node, char bp_to_search_for);

//determines the maximum number of bp misses before check fails
int determine_threshold(string search_string);

void visualize_sequence_search();
trie_node* load_cat_flu();

#endif