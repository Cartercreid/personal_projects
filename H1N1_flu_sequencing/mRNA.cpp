// mRNA.cpp
//

#include "mRNA.h"
#include "load_cat.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;
//as much as I hate to have this... but global variabls definitions:
////////////////////////////////////////////////////////////////////
float percent_threshold = 90; //90 is for the mRNA_test cases, 95 is a reasonable staring point for larger strings
////////////////////////////////////////////////////////////////////
//add sequence will take a tree, and add a sequence to it.

/*add sequence takes a tree root and adds a sequence to it*/
void add_sequence(trie_node* root, string input, string sequence_name){
    unsigned int index = 0;
    trie_node* cursor = new trie_node;
    cursor = root;
    //cursor = check_children(root, input[index]);
    //if (cursor == NULL){
        
    //}
    return add_recursive(cursor, input, sequence_name, index);
}
/*add recursive walks along a previously established sequence until new sequence deviates, then adds new nodes till
new sequence is complete*/
void add_recursive(trie_node* cursor, string input, string sequence_name, unsigned int index){
    if (index == input.length()){
        return;
    }
    trie_node* child = check_children(cursor, input[index]);
    //if no children match current bp
    if (child == NULL){
        child = init_node(input[index], sequence_name);
        add_child(cursor, child);
    }
    else{
        //this is where I should add sequence name to current node
        child->RNA_ID.push_back(sequence_name);
    }
    index++;
    add_recursive(child, input, sequence_name, index);
    return;
}
//adds child node into next available opening
/*add a child to the node*/
void add_child(trie_node* cursor, trie_node* child){
    for (int i = 0; i < 4; i++){
        if (cursor->next[i] == NULL){
            cursor->next[i] = child;
            return;
        }
    }
}
//check sequence will check if a sequence is in the tree and return all values greater than the 
//accuracy threshold. It will return the solution in the form of a solution table
//if summary is true, it will return a summary table, otherwise it will return the raw tabel
solution_list* check_sequence(trie_node* root, string input, bool summary){
    int node_counter = 0;
    int hit_thresh = determine_threshold(input);
    int problem_space = input.length();
    solution_list* solution = new solution_list;
    
    outer_check_recursive(root, hit_thresh, input, problem_space, solution, node_counter);
    
    //solution_list* ret = init_solution_list();
    
    //need another sort of recursive function or loop to go through the whole tree again.
    sort_c_solution(solution);
    if (summary){
        solution_list* c_solution = collate_solution(solution);
        return c_solution;
    }
    else{
        //
        return solution;
    }
    //delete solution;
    
    
}
/*this function checks the current string ID agains the IDs in any of the children, if they are present, then
solution has already been added to the solution list*/
bool not_in_colated_solution(string RNA_ID, solution_list* c_solution){
    for (unsigned int i = 0; i < c_solution->ID.size(); i++){
        if (c_solution->ID[i] == RNA_ID){
            return false;
        }
    }
    return true;
}
/*add to collated takes a solution list of length hits, and collapses it into a more summarized table,
for example if a sequence had 3 100 % hits and 1 98% hit, it would push those results into 2 lines reflecting
number of hits at what percent. collated list loses start_node information and individual sequences cannot be printed
from the collated table*/
void add_to_collated(string ID, solution_list* solution, solution_list* c_solution){
    int s_size = solution->ID.size();
    int c_size = c_solution->ID.size();
    for (int i = 0; i < s_size; i++){
        if (solution->ID[i] == ID){
            //if its not in the table, add it.
            if (not_in_colated_solution(solution->ID[i], c_solution)){
                c_solution->ID.push_back(solution->ID[i]);
                c_solution->percent_hit.push_back(solution->percent_hit[i]);
                c_solution->count.push_back(1);
                //c_solution->solution_start[i].push_back(solution->solution_start[i][0]);
                c_size++;
            }
            //its in the table, increment count if duplicate percentage, add new entry if new percentage
            else{
                for (int j = 0; j < c_size; j++){
                    if (c_solution->ID[j] == solution->ID[i]){
                        if (c_solution->percent_hit[j] == solution->percent_hit[i]){
                            //c_solution->solution_start[j].push_back(solution->solution_start[i][0]);
                            c_solution->count[j]++;
                        }
                        //add new entry if new percentage
                        else{
                            c_solution->ID.push_back(solution->ID[i]);
                            c_solution->percent_hit.push_back(solution->percent_hit[i]);
                            c_solution->count.push_back(1);
                            //c_solution->solution_start.push_back(solution->solution_start[i]);
                            c_size++;
                        }
                    }
                }
            }
        }
    }
}
/*collate solution iterates through a solution list and condenses entries into fewer lines. Useful
if a search has multiple matches in a single sequence*/
solution_list* collate_solution(solution_list* solution){
    //ok probably do this in a for loop, comparing strings and numbers... can identify using strings
    int len_vector = solution->ID.size();
    solution_list* c_solution = new solution_list;
    //vector <float> highest_hit;
    //vector <int> count;
    //vector <string> name_copy;
    for (int i = 0; i < len_vector; i++){
        if (not_in_colated_solution(solution->ID[i],c_solution)){
            //do something to add the solution to the new vectors
            add_to_collated(solution->ID[i], solution, c_solution);
        }
    }
    //sort_c_solution(c_solution);
    return c_solution;
}
/*sort solution uses bubble sort to sort a solution so that the higher hit percentages appear at the top of the list*/
void sort_c_solution(solution_list* solution){
    int n = solution->ID.size();
    for (int i=0; i < n; i++)
    {
        for (int j=0; j < n-i-1; j++)
        {
            if(solution->percent_hit[j] < solution->percent_hit[j+1])
            {
                swap(solution->percent_hit[j],solution->percent_hit[j+1]);
                swap(solution->ID[j],solution->ID[j+1]);
                swap(solution->count[j],solution->count[j+1]);
                swap(solution->solution_start[j],solution->solution_start[j+1]);
                
            }
        }
    }
    return;
}
/*outer check recursive is the outer loop for checking sequence matching. The current node in this function
is the start position for the internal recursive function. This iterates through every node in the tree and 
evaluates each node as a potential start position for string matching*/
void outer_check_recursive(trie_node* current_node, int hit_thresh, string input, int problem_space, solution_list* solution, int node_counter){
    if (current_node == NULL){
        return;
    }
    
    int index = 0;
    int miss = 0;
    //this no longer needs to be a bool, but it curently works, so... 
    if(check_recursive(current_node, hit_thresh, input, index, problem_space, miss, solution, current_node, node_counter)){
        //need to add current node as a start node to the solution... its a vector vector node though...
        //vector <trie_node*> node_vect;
        //node_vect.push_back(current_node);
        //solution->solution_start.push_back(node_vect);
        
    }
    
    for (int i = 0; i < 4; i++){
        outer_check_recursive(current_node->next[i], hit_thresh, input, problem_space, solution, node_counter+1);
    }
}


/*check recursive takes the current node, hit threshold, the input string, current index, input string length, miss tally, a solution list, a start node, and the node counter
as arguments. This is the inner loop of the recursive sequence check. Each iteration, it checks the input string index to the current nodes base pair and increments a miss if not
equal. If misses is higher than the threshold, the attemt is abandoned. If it reaches the end of the search string, it adds it as a valid solution. If the tree length is less than
the search string length, it assumes all subsequent (NULL) children are misses adn add those to the miss counter. If still under the miss threshold, it adds it as a valid solution*/
bool check_recursive(trie_node* current_node, int hit_thresh, string input, int index, int problem_space, int miss, solution_list* solution, trie_node* start_node, int node_counter){
    bool chockey;
    //if dead end
    if (current_node == NULL){
       // if (problem_space - index + miss > hit_thresh){
       return false;
        //}
        
    }
    
    //bp miss
    if (current_node->bp != input[index]){
        miss++;
        //increment the miss
        if (miss > hit_thresh){
            return false;
        }
    }
    //if index has reached the end of the problem space
    if (index == problem_space -1){
        for (unsigned int i = 0; i < current_node->RNA_ID.size(); i++){
            
            add_solution(solution, current_node->RNA_ID[i], problem_space, miss, start_node, node_counter);
        }
        return true;
        //solution->percent_hit.push_back();
        //solution->ID.push_back(input);
    }
    
    if (current_node->next[0] == NULL && current_node->next[1] == NULL && current_node->next[2] == NULL && current_node->next[3] == NULL){
        //cout<<"end of the road!";
        //no children, end of the database
        if ((problem_space-1) - index + miss <= hit_thresh){
            for (unsigned int i = 0; i < current_node->RNA_ID.size(); i++){
                add_solution(solution, current_node->RNA_ID[i], problem_space, miss + ((problem_space -1) - index), start_node, node_counter);
            }
            return true;
        }
        else{
            return false;
        }
    }
    //recursively continue on to all children
    bool ret = false;
    for (int i = 0; i < 4; i++){
        
        chockey = check_recursive(current_node->next[i], hit_thresh, input, index+1, problem_space, miss, solution, start_node, node_counter);
        if (chockey == true){
            ret = true;
        }
    }
    return ret;
}
/*add solution takes a solution list, and all components that need to be reccorded to the list, and appends them to the end of the list as they are found*/
void add_solution(solution_list* solution, string input, int problem_space, int miss, trie_node* start_node, int node_counter){
    float fl_miss = miss;
    float fl_p_space = problem_space;
    float percentage = 100* (1 - (fl_miss / fl_p_space));
    solution->percent_hit.push_back(percentage);
    solution->ID.push_back(input);
    solution->count.push_back(node_counter);
    solution->solution_start.push_back(start_node);
    
}
//init root initiallizes a special trie node with bp char '5' whic stands for 5' end of the mRNA. Really, though this is just for a 
//convenient starting point for the tree
trie_node* init_root(){
    trie_node* ret = new trie_node;
    ret->bp = '5';
    for (int i=0; i < 4; i++) {
        ret->next[i] = NULL;
  }
  return ret;
}

//init node initializes a new node in the tree with a specific bp and RNA_ID
trie_node* init_node(char bp, string RNA_ID){
    trie_node* new_node = new trie_node;
    new_node->bp = bp;
    //new_node->RNA_ID[0] = RNA_ID;
    new_node->RNA_ID.push_back(RNA_ID);
    for (int i=0;i<4; i++){
        new_node->next[i] = NULL;
    }
    return new_node;
}

//init solution_list
solution_list* init_solution_list(float hit, string ID){
    solution_list* new_stack = new solution_list;
    new_stack->percent_hit.push_back(hit); // = hit;
    new_stack->ID.push_back(ID); // = ID;
    //new_stack->next = NULL;
    return new_stack;
}

//terminate checks the current and next 6 nodes to see if they are all A's, if yes, then the tree is terminated and function
//returns true, else it returns false

bool terminate(){
    //TODO
    return false;
}

//determines the maximum number of bp misses before check fails
//Uses global variable percent_threshold to calculate a discrete number of allowed misses for the queary*/
int determine_threshold(string search_string){
    ///////////////////////////////////////////////////////////////////
    //TODO
    //percent_threshold 
    ///////////////////////////////////////////////////////////////////
    int x = search_string.length();
    //return (x/ 10);
    return floor(x * (1-(percent_threshold/100)));
}

//checks the nodes children to see if they have a child contianing the next bp
//returns a node if it has the right child, NULL otherwise
trie_node* check_children(trie_node* current_node, char bp_to_search_for){
    if (current_node == NULL){
        return NULL;
    }
    for (int i = 0; i < 4; i++){
        if (current_node->next[i] != NULL && current_node->next[i]->bp == bp_to_search_for){
            return current_node->next[i];
        }
    }
    return NULL;
}
/*print sequence match can only be called with an un collated solution list, as that is the solution list type that contains information
on the start node. It iterates through the tree given a sequence name and computes a hit or miss at each node. It uses string stream to
accumulate the different strings, and then prints them out line by line.*/
void print_sequence_match(trie_node* start, string input, string sequence_name){
    trie_node* cursor = start;
    cout<<"sequence match for: "<< sequence_name <<endl;
    stringstream data_string;
    stringstream hit_string;
    string d_string;
    string h_string;
    
    int leng = input.length();
    for (int i =0; i < leng; i++){
        //cout<<cursor->bp;
        if (cursor == NULL){
            data_string << "-";
            hit_string << " ";
        }
        else{
             data_string << cursor->bp;
             if (cursor->bp == input[i]){
                //cout<<"==";
                hit_string<< "|";
             }
             else{
                hit_string<< " ";
                //cout<<" ";
             }
        }
        
        cursor = get_next_node(cursor, sequence_name);
        
    }
    d_string = data_string.str();
    h_string = hit_string.str();
    int rows = leng/ 90;
    int position = 0;
    //print the strings out 90 characters at a time to account for standard dimension terminal windows
    for (int row = 0; row < rows; row++){
        for (int j = 0; j < 90; j++){
            cout<<d_string[j+position];
        }
        cout<<"\n";
        for (int k = 0; k < 90; k++){
            cout<<h_string[k+position];
        }
        cout<<"\n";
        for (int l = 0; l < 90; l++){
            cout<<input[l+position];
        }
        cout<<"\n\n";
        position = position + 90;
    }
    //print the remaining line of sequence match
    int b = 0;
    while (position + b < leng){
        cout<<d_string[position+b];
        b++;
    }
    cout<<"\n";
    b = 0;
    while (position + b < leng){
        cout<<h_string[position+b];
         b++;
    }
    cout<<"\n";
    b = 0;
    while (position + b < leng){
        cout<<input[position+b];
         b++;
    }
    cout<<"\n";
    //b = 0;
    //cout<<d_string<<endl;
    //cout<<h_string<<endl;
    //cout<<input<<endl;
}
/*get next node gets the next node in a sequence string by iterating through the child RNA_ID vectors*/
trie_node* get_next_node(trie_node* start, string sequence_name){
    if (start == NULL){
        return NULL;
    }
    //check all 4 children
    for (int i = 0; i < 4; i++){
        if (start->next[i] != NULL){
            //check all RNA_ID's in child field
            for (unsigned int j = 0; j < start->next[i]->RNA_ID.size(); j++){
                if (start->next[i]->RNA_ID[j] == sequence_name){
                    return start->next[i];
                }
            }
        }
    }
    //no matches, return NULL
    return NULL;
}
/*Blast loads a batch of files depending on the location. There are 8 segments in H1N1, and BLAST loads each segment from the location provided
Examples of location are: CO,UK,Germany,South_Korea,Hawaii,Japan,New_York,Russia.
2 patient examples are: CO1 and UK1, which have outputs more similar to simpler (and cheaper) sequencing methods.*/
solution_list* BLAST(trie_node* data_tree, string location){
    stringstream current_file;
    solution_list* i_solution = new solution_list; //intermediate solution list
    solution_list* f_solution = new solution_list; //final solution list
    string segments = "12345678";
    string lookup;
    //assemble the file path string for all 8 segments
    for (int seg = 0; seg < 8; seg++){
        current_file <<"fastas/";
        current_file << location;
        current_file << "_H1N1_seg";
        current_file << segments[seg];
        current_file << ".txt";
        lookup = current_file.str();
        lookup = import_sequence(lookup);
        
        //get and print a summarized report for the query
        i_solution = check_sequence(data_tree, lookup, true);
        for (unsigned int i = 0; i < i_solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
             f_solution->count.push_back(i_solution->count[i]);
             f_solution->percent_hit.push_back(i_solution->percent_hit[i]);
             f_solution->ID.push_back(i_solution->ID[i]);
             cout<<i_solution->count[i] << " "<< i_solution->percent_hit[i] << " percent hits in "<< i_solution->ID[i] <<endl;
        }
        current_file.str(string()); //clear the file path string
    }
    return f_solution;
    
}
//iterates through a table and prints each line
void print_c_solution(solution_list* solution){
    for (unsigned int i = 0; i < solution->ID.size(); i++){
             cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
}
/*import sequence imports a FASTA file. A fasta file is a plain text file with >marking the start of the file and > marking the start of 
the sequence. The first time > is encountered, it is added to the name string, after it is encountered again, futher characters are added
to the sequence string*/
string import_sequence(string fn){
    string strain_name;
    string sequence;
    stringstream current_s;
    int counter = 0;
    char ch;
    cout<<"reading file: "<<fn<<endl;
    fstream fin(fn, fstream::in);
    
    if (!fin){
        cout<<"error reading file"<<endl;
        return "0";
    }
    //iterate throught he open file
    while (fin >> noskipws >> ch) {
        //special FASTA character deliminator
        if (ch == '>'){
            counter++;
            if (counter == 2){
                strain_name = current_s.str();
                current_s.str(string());
            }
        }
        else {
            //ignor line terminators
            if (ch != '\n'){
                current_s << ch;
            }
        }
        //increment_lookup(lookup, ch);
        //cout<<ch;
    }
    sequence = current_s.str();
    //cout<<"import test: sstream: "<<endl;
    cout<<"strain name: "<<strain_name<<endl;
    //cout<<"sequence: "<<sequence<<endl;
    return sequence;
    
}
/*import data takes a tree root node and a file name and opens the FASTA file and enteres the new data into the
tree after recovering the RNA_ID name and the sequence. */
bool import_data(trie_node* tree_root, string fn){
    string strain_name;
    string sequence;
    stringstream current_s;
    //stringstream current_seq;
    int counter = 0;
    char ch;
    cout<<"reading file: "<<fn<<endl;
    fstream fin(fn, fstream::in);
    
    if (!fin){
        cout<<"error reading file"<<endl;
        return false;
    }
    while (fin >> noskipws >> ch) {
        if (ch == '>'){
            counter++;
            if (counter == 2){
                strain_name = current_s.str();
                current_s.str(string());
            }
        }
        else {
            //ignor line terminators
            if (ch != '\n'){
                current_s << ch;
            }
        }

    }
    sequence = current_s.str();
    cout<<"strain name: "<<strain_name<<endl;
    add_sequence(tree_root, sequence, strain_name);
    cout<<strain_name<<"\n successfully added"<<endl;
    return true;
}
/*import human_flu_PB2 is the first database I have made it only contains segment 1 from 
different geographic locations. Some of the file names have changed, and thus this function will
probably return false.*/
bool imort_human_flu_PB2(trie_node* new_tree){
    bool no_errors = true;
    no_errors = no_errors & import_data(new_tree, "fastas/Alabama_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Colorado_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/England_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Florida_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/France_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Guam_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Italy_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Minnesota_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Moscow_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_England_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/NewYork_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/S_Korea_H1N1_PB2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Vietnam_H1N1_PB2.txt");
    return no_errors;
}
/*import_human_flu imports 9 different flu genomes from different geographic locations around the world.
no errors is a boolean that keeps trach if any upload errors occur.*/
bool imort_human_flu(trie_node* new_tree){
    bool no_errors = true;
    //California !!!still need to add > in file
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/California_H1N1_seg8.txt");
    
    //Hawaii !!!still need to add > in file
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Hawaii_H1N1_seg8.txt");
    
    //japan !!still need ot add > in file
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Japan_H1N1_seg8.txt");
    
    //New York !!still need to add > in file
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/New_York_H1N1_seg8.txt");
    
    //russia
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Russia_H1N1_seg8.txt");
    
    //south_korea
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/South_Korea_H1N1_seg8.txt");
    
    //UK
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/UK_H1N1_seg8.txt");
    
    //CO
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/CO_H1N1_seg8.txt");
    
    //Germany
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg1.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg2.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg3.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg4.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg5.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg6.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg7.txt");
    no_errors = no_errors & import_data(new_tree, "fastas/Germany_H1N1_seg8.txt");
    
    //

    
    return no_errors;
}
/*change percent threshold changes the percent threshold from the default value to a user specified value.*/
void change_percent_threshold(){
    cout<<"What would you like to change the threshold to?"<<endl;
    cin>>percent_threshold;
    if (percent_threshold < 0){
        percent_threshold = 0;
    }
    else if (percent_threshold > 100){
        percent_threshold = 100;
    }
}
/*UI is a user interface that allows string searches, BLAST, data import etc. It is a bit rough around the edges and not all menue options are
as useful or as user friendly as I would like, but it serves as a decent framework to demonstrate the capabilities of the project overall.*/
bool UI(){
    string user_in; //string for user input in menues and sequence adding
    string user_in_two;
    int user_in_int;
    trie_node* new_tree = init_root();
    cout<<"Welcome to DNA string matching with trie trees! \n"<<endl;
    cout<<"the current database is empty, please add to it. I must appologize, but I only have a couple prepared datasets"<<endl;
    cout<<"press 1 for small test sample"<<endl;
    cout<<"press 2 for European Cat Flu 2018"<<endl;
    cout<<"press 3 for human flu data 2018"<<endl;
    cout<<"or any other key to terminate"<<endl;
    //cout<<"press 4 for flu BLAST"<<end;
    
    //bool have_d = false;
    cin>>user_in;
    
    //cout << string( 100, '\n' );
    if (user_in == "1"){
        string one = "AUGACCAAAAAAA";
        string name_one = "HOX";
        string two = "AUGUCCAAAAAAA";
        string name_two = "laser eyes";
        string three = "AUGUCUAAAAAAA";
        string name_three = "wolverine";
        add_sequence(new_tree, one, name_one);
        add_sequence(new_tree, two, name_two);
        add_sequence(new_tree, three, name_three);
    }
    else if (user_in == "2"){
        new_tree = load_cat_flu();
    }
    else if (user_in == "3"){
        if (imort_human_flu(new_tree)){
            cout<<"successfully added 2018 to 2019 sample strains taken from 7 different geographic regions"<<endl;
            cout<<"changing threshold to 98 due to large string sizes, hit threshold can be manually changed later."<<endl;
            percent_threshold = 98;
            
        }
        else {
            cout<<"error loading database"<<endl;
        }
    }
    else{
        cout<<"Goodbye"<<endl;
        return true;
    }
    while(true){
        //system("clear");
        cout<<"Please select an option: \n 1: search sequence \n 2: add sequence to database \n 3: genome BLAST \n 4: change the percent hit threshold \n or q to quit"<<endl;
        cin>>user_in;
        cout << string( 100, '\n' );
        //system("cls");
        if (user_in == "q"){
            cout<<"Goodbye"<<endl;
            return true;
        }
        else if (user_in == "1"){
            cout<<"just a reminder, I am case sensitive at the moment"<<endl;
            cout<<"enter a sequence you would like to look up or press i to import a sequence"<<endl;
            cin>>user_in;
            if (user_in == "i"){
                cout<<"please enter the filepath to the fasta document you want me to read"<<endl;
                cin>>user_in;
                user_in = import_sequence(user_in);
                //if (user_in == "0"){
                    
                //}
            }
            bool summary = true;
            solution_list* solution = new solution_list;
            solution = check_sequence(new_tree, user_in, summary);
            //cout << "Using corpus from '" << fn << "'" << endl;
            int s = solution->ID.size();
            
            for (int i = 0; i < s; i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
                cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hit(s) in "<< solution->ID[i] <<endl;
            }
            cout<<"\n"<<endl;
            cout<<"press 1 for a more detailed report \n press 2 to return to the main menue"<<endl;
            cin>>user_in_two;
            cout << string( 100, '\n' );
            //system("cls");
            if( user_in_two == "1"){
                summary = false;
                solution = check_sequence(new_tree, user_in, summary);
                //cout << "Using corpus from '" << fn << "'" << endl;
                s = solution->ID.size();
            
                for (int i = 0; i < s; i++){
                //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
                cout<<"index: " <<i+1<<" node position: "<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hit(s) in "<< solution->ID[i] <<endl;
                }
                //cin>>user_in_two;
                //if (user_in_two == "Y" || user_in_two == "y"){
                    while(true){
                        cout<<"enter number of the sequence you would like to print, enter 0 to return to the main menue"<<endl;
                        cin>>user_in_int;
                        if (user_in_int == 0){
                            break;
                        }
                        //check if the user input is invalid
                        else if (user_in_int < 0 || user_in_int > s){
                            cout<<"invalid input"<<endl;
                        }
                        else{
                            --user_in_int; //adjust user input to match solution index
                            
                            //this is the part where I print the sequence
                            print_sequence_match(solution->solution_start[user_in_int], user_in, solution->ID[user_in_int]);
                        }
                        
                    }
                //}
                
            }
            
        }
        else if (user_in == "2"){
            cout<<"Enter file path"<<endl;
            cin>>user_in;
            import_data(new_tree, user_in);
        }
        else if (user_in == "3"){
            cout<<"plese enter a region"<<endl;
            cin>>user_in;
            //solution_list* f_solution = new solution_list;
            //solution_list* f_solution = BLAST(new_tree, user_in);
            //print_c_solution(f_solution);
            BLAST(new_tree, user_in);
        }
        else if(user_in == "4"){
            change_percent_threshold();
            cout<<"percent threshold changed to: "<<percent_threshold<<endl;
        }
        else{
            cout<<"Invalid input"<<endl;
        }
        
    }
}
