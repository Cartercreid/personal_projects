//
// mRNA_test.cpp
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "mRNA.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
//global debugging bool


TEST_CASE("mRNA: sanity check", "[sanity]") {
  REQUIRE(true);
}

//test init node
TEST_CASE("mRNA: node", "[create node]") {
  string gene_name = "sonic the hedge hog";
  trie_node* n = init_node('A', gene_name);
  REQUIRE(n != NULL);
  REQUIRE(n->bp == 'A');
  REQUIRE(n->RNA_ID[0] == gene_name);
  for (int i = 0; i < 4; i++){
      REQUIRE(n->next[i] == NULL);
  }
}
TEST_CASE("mRNA: groot", "[groot]"){
    trie_node* groot = init_root();
    REQUIRE(groot != NULL);
    REQUIRE(groot->bp == '5');
    REQUIRE(groot->next[0] == NULL);
    REQUIRE(groot->next[1] == NULL);
    REQUIRE(groot->next[2] == NULL);
    REQUIRE(groot->next[3] == NULL);
}
//test init solution_list
TEST_CASE("mRNA: create solution list", "[create list]") {
    string gene_name = "sonic the hedge hog";
    float hit = 99.9;
    solution_list* l = init_solution_list(hit, gene_name);
    REQUIRE(l->percent_hit[0] == hit);
    REQUIRE(l->ID[0] == gene_name);
}

//test build tree
TEST_CASE("mRNA: build tree", "[build tree]") {
    
    string one = "AUGACCAAAAAAA";
    string name_one = "HOX";
    string two = "AUGUCCAAAAAAA";
    string name_two = "laser eyes";
    string three = "AUGUCUAAAAAAA";
    string name_three = "wolverine";
    
    trie_node* new_tree = init_root();
    add_sequence(new_tree, one, name_one);
    add_sequence(new_tree, two, name_two);
    add_sequence(new_tree, three, name_three);
    REQUIRE(new_tree->next[0] != NULL);
    REQUIRE(new_tree->next[0]->bp == 'A');
    REQUIRE(new_tree->next[0]->next[0]->bp == 'U');
    REQUIRE(new_tree->next[0]->next[0]->next[0]->bp == 'G');
    
    REQUIRE(new_tree->next[0]->RNA_ID[0] == name_one);
    REQUIRE(new_tree->next[0]->RNA_ID[1] == name_two);
    REQUIRE(new_tree->next[0]->RNA_ID[2] == name_three);
    
    REQUIRE(new_tree->next[0]->next[0]->next[0]->next[0]->bp == 'A');
    REQUIRE(new_tree->next[0]->next[0]->next[0]->next[0]->RNA_ID[0] == name_one);
    
    REQUIRE(new_tree->next[0]->next[0]->next[0]->next[1]->bp == 'U');
    REQUIRE(new_tree->next[0]->next[0]->next[0]->next[1]->RNA_ID[0] == name_two);
    REQUIRE(new_tree->next[0]->next[0]->next[0]->next[1]->RNA_ID[1] == name_three);
}

//test terminate search
//test with 6 A's, test with 5 A's

//test single base pairs and AUG
TEST_CASE("mRNA: lookup sequence", "[small base]"){
    bool print = false;
    string a = "A";
    string aug = "AUG";
    string cc = "CC";
    string gg = "GG";
    string one = "AUGACCAAAAAAA";
    string name_one = "HOX";
    string two = "AUGUCCAAAAAAA";
    string name_two = "laser eyes";
    string three = "AUGUCUAAAAAAA";
    string name_three = "wolverine";
    //string mostly_wolvering = "AUGUCUAAAAA"
    trie_node* new_tree = init_root();
    add_sequence(new_tree, one, name_one);
    add_sequence(new_tree, two, name_two);
    add_sequence(new_tree, three, name_three);
    solution_list* solution = new solution_list;
    bool summary = true;
    solution = check_sequence(new_tree, a, summary);
    if (print) {
        cout<<"searching for string: " << a <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
    }
    REQUIRE(solution->ID.size() == 3);
    
    //delete solution;
    //solution_list* solution = new solution_list;
    solution = check_sequence(new_tree, aug, summary);
    cout<<"searching for string " << aug <<endl;
     //cout << "Using corpus from '" << fn << "'" << endl;
    if (print){
        
    
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
    }
    REQUIRE(solution->ID.size() == 3);
    
    //delete solution;
    //solution_list* solution = new solution_list;
    solution = check_sequence(new_tree, cc, summary);
    if (print){
        
   
        cout<<"searching for string " << cc <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
        //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
    }
    REQUIRE(solution->ID.size() == 2);
    
    //delete solution;
    //solution_list* solution = new solution_list;
    solution = check_sequence(new_tree, gg, summary);
    if (print){
        
    
        cout<<"searching for string " << gg <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
        if(solution->ID.size()==0){
            cout<<"no results"<<endl;
        }
    }
    REQUIRE(solution->ID.size() == 0);
    solution = check_sequence(new_tree, three, summary);
    if (print){
        
    
        //solution = check_sequence(new_tree, three, summary);
        cout<<"searching for string " << three <<endl;
             //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
        if(solution->ID.size()==0){
            cout<<"no results"<<endl;
        }
    }
    REQUIRE(solution->ID.size() == 2);
    
    solution = check_sequence(new_tree, two, summary);
    if (print){
        
    
        cout<<"searching for string " << two <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
        if(solution->ID.size()==0){
            cout<<"no results"<<endl;
        }
    }
    REQUIRE(solution->ID.size() == 3);
    solution = check_sequence(new_tree, one, summary);
    if (print){
        
   
        cout<<"searching for string " << one <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
        if(solution->ID.size()==0){
            cout<<"no results"<<endl;
        }
    }
    REQUIRE(solution->ID.size() == 2);
    string one_too_many = "AUGACCAAAAAAAC";
    solution = check_sequence(new_tree, one_too_many, summary);
    if (print){
        cout<<"searching for string " << one_too_many <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
        //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
    }
    
    REQUIRE(solution->ID.size() > 0);
    
}
TEST_CASE("mRNA: cat flu", "[cat flu]"){
    string cat_one = "agcgaaagcaggtcaaatatattcaatatggagagaataaaggaattaagagatctaatgtcacagtcccgcactcgcgagatactaacaaaaaccactgtggaccatatggccataatcaagaaatacacatcaggaag acaagagaagaaccctgctctcagaatgaaatggatgatggcaatgaaatatccaatcacagcggacaagagaataatagagatgattcctgaaaggaatgaacagggacagacactctggagcaagacaaatgatgctggatcggacag ggtgatggtgtctcccctagctgtaacttggtggaataggaatgggccgacgacaagtacagttcattatccaaaggtttataaaacatactttgagaaggttgaaaggttaaaacatggaacctttggtcccgttcatttccgaaacca agttaaaatacgccgccgagttgatacaaatcctggccatgcagatctcagcgctaaagaagcacaagatgtcatcatggaggtcgttttcccaaatgaagtgggagctagaatattgacttcagagtcacaattgacaataacgaaaga gaagaaagaagagctccaagattgtaagattgctcccttaatggttgcatacatgttggaaagggaactggtccgcaaaaccagattcctaccagtagcaggcggaacaagcagtgtgtacattgaggtattgcatttgactcaaggaac ttgctgggaacagatgtacactccaggcggagaagtaagaaatgacgatgttgaccagagtttgatcattgctgccagaaacattgttaggagagcaacggtatcagcggatccactggcatcactgctggagatgtgtcacagcacaca aattggtgggataaggatggtggacatccttaggcaaaatccaactgaggaacaagctgtggatatatgc961aaagcagcaatgggtctgaggatcagttcatcctttagctttggaggcttcactttcaaaagaacaagtggatcatccgt cacgaaggaagaggaagtgcttacaggcaacctccaaacattgaaaataagagtacatgaggggtatgaggaattcacaatggttgggcagagggcaacagctatcctgaggaaagcaactagaaggctgattcagttgatagtaagtgg aagaaacgaacaatcaatcgctgaggcaatcattgtagcaatggtgttctcacaggaggattgcatgataaaagcagtccgaggcgatctgaatttcgtaaacagagcaaaccaaagattaaaccccatgcatcaactcctgagacattt tcaaaaggacgcaaaagtgctatttcagaattggggaactgaacccattgataatgtcatggggatgatcggaatattacctgacatgactcccagcacagaaatgtcactgagaggagtaagagttagtaaagtgggagtagatgaata ttccagcactgagagagtagttgtaagcattgaccgcttcttaagggttcgagaccagcgggggaacgtactcttatctcccgaagaggtcagcgaaacacagggaacagagaagttgacaataacatattcatcatcaatgatgtggga aatcaacggtcctgagtcagtgcttgttaacacctatcaatggatcatcagaaactgggagacagtgaaaattcagtggtctcaagaccccactatgttgtacaataagatggagtttgaaccgttccaatccttggtacctaaagctgc cagaggtcaatacagtggatttgtgagaacattattccaacaaatgcgtgacgtactggggacatttgatactgtccagataataaagctgctaccatttgcagcagccccaccgaagcagagcagaatgcagttttcttctctaactgt gaatgtgagaggctcaggaatgagaatactcataaggggcaattcccctgtgttcaactacaataaggca accaaaagacttaccgttcttggaaaggacgcaggtgcattaacagaggatccagatgaggggacagccggagtggaatc tgcagtactgagggggttcctaattctaggcaaggaggacaaaagatatggaccagcattgagcatcaatgaactgagcaatcttgcaaaaggggagaaagctaatgtgctgatagggcaaggagacgtggtgttggtaatgaaacggaa acgggactctagcatacttactgacagccagacagcgaccaaaagaattcggatggccatcaattagtgtcgaattgtttaaaaacgaccttgtttctact";
    string cat_one_name = "H1N1 - PB2";
    string cat_two = "agcaaaagcaggcaaaccatttgaatggatgtcaatccgactttacttttcttgaaagtaccagtgcaaaatgctataagtaccacattcccttatactggagaccctccatacagccatgggacagggacaggatacaccatggacacagtcaacagaacacaccaatattcagaaaaggggaagtggacaacaaacacagagactggagcaccccaactcaacccgattgatggaccactacctgaggataatgagcccagtggttatgcacaaacagattgtgtattggaagcaatggctttccttgaagaatcccacccagggatctttgaaaactcgtgtcttgaaacgatggaaattgttcaacaaacaagagtggataaactgacccaaggtcgtcagacctatgactggacattgaatagaaaccaaccggctgcaaccgctttggccaacactatagaaatcttcagatcgaacggtctaacagccaatgaatcgggacggctaatagatttcctcaaggatgtgatggaatcaatggataaggaagaaatggagataacaacacacttccagaggaagagaagagtgagagacaacatgaccaaaaagatggtcacacaaagaacaatagggaagaaaaagcaaaggctgaacaaaaagagctacctgataagagcactgacactgaatacaatgacaaaagatgcagaaagaggcaaattgaagaggcgagcaattgcaacacccggaatgcaaatcagaggattcgtgtactttgttgaaacattagcgaggagtatctgtgagaaacttgagcaatctggactcccagttggagggaatgaaaagaaggctaaattggcaaacgtcgtgaggaagatgatgactaactcacaagatactgaactctcctttacaattactggagacaatactaaatggaatgagaatcagaatcctaggatgtttctggcaatgataacatacatcacaaggaaccagccagaatggtttcggaatgtcttaagcattgcccctataatgttctcaaacaaaatggcaagattaggaagaggatacatgttcgaaagtaagagcatgaagttgcgaacacaaataccagcagaaatgcttgcaaacattgatcttaaatacttcaatgaattaacgaaaaagaaaattgaaaaaataagacctctattaatagatggtacagcctcattgagccctggaatgatgatgggcatgttcaacatgctgagtacagttctgggagtctcaatcttgaaccttggacagaaaaggtacaccaaaaccacatattggtgggacggactccaatcctctgatgatttcgctctcatcgtaaatgcaccgaatcatgagggaatacaagcaggagtggatagattttataggacttgtaaattagttggaatcaacatgagcaagaagaagtcttacataaatcggacagggacatttgaattcacgagctttttctaccgctatggatttgtagccaatttcagtatggagctgcccagttttggagtgtctggaattaatgaatcggccgacatgagcattggtagcacagtgataaggaacaatatgataaacaacgaccttgggccagcaacagctcagatggctcttcagctattcatcaaggactacagatacacataccgatgccacagaggggatacgcaaatccaaacgaggagatcattcgagctgaagaagctgtgggagcaaacccgttcaaaggcaggactgttggtttcagatggaggaccaaatctatacaatatccgaaatctccatatccctgaggtctgcttgaaatgggaattgatggatgaagattaccagggcagactgtgtaatcctctgaatccgttcgtcagccataaggaaattgaatctgtcaacaatgctgtagtaatgccagctcatggcccggccaagggtatggaatatgatgccgttgcaactacgcattcatggattcctaaaaggaatcgttccattctcaatacgagtcaaaggggaattcttgaggatgaacagatgtaccagaagtgctgcaatctatttgagaaattcttccccagcagttcatatcggaggccagttggaatttccagcatggtggaggccatggtgtctagggcccgaattgacgcacgaattgatttcgagtctggaaggattaagaaagaagagtttgctgagatcatgaagatctgttccaccattgaagagctcagacggccaaaatagtgaatttagcttgtccttcgtgaaaaaatgccttgtttctact";
    string cat_two_name = "H5N1-PB1";
    string cat_three = "agcaaaagcaggtactgattcaaaatggaagatcttgtgcgacaatgcttcaatccaatgattgtcgagcttgcggaaaaggcaatgaaagaatatggggaagatccgaaaatcgaaacgaacaaatttgctgcaatatgcacacacttggaggtctgtttcatgtattcggattttcactttattgatgaacggagtgaatcaataattgtagaatctggagatccgaatgcattattgaaacaccgatttgaaataattgaaggaagagaccgaacgatggcctggactgtggtgaatagtatttgcaacaccacaggagtcgagaaacctaaatttctcccagatttgtatgactacaaagagaaccgattcattgaaattggagtgacacggagggaagttcatacatactatctggagaaggccaacaagataaaatccgagaagacacatattcacatattctcattcacaggggaggaaatggcaaccaaagcggactacacccttgatgaagaaagcagggcaagaattaaaaccaggctgttcaccataagacaggaaatggccagtaggggtctatgggattcctttcgtcaatccgagagaggcgaagagacaattgaagaaaaatttgaaatcactggaaccatgcgcagacttgctgaccaaagtctcccaccgaacttctccagccttgaaaactttagagcctatgtggatggattcgaaccgaacggctgcattgagggcaagctttctcaaatgtcaaaagaagtgaatgctagaattgagccatttttgaagacaacgccacgccctctcagattacctgatgggcctccttgctctcagcggtcgaaattcctgctgatggatgcccttaaattaagtatcgaagacccgagtcatgagggggaggggataccactatacgatgcaatcaaatgcatgaagacatttttcgggtggaaggagcccaacatcgtgaaaccacatgaaaagggtataaaccccaattacctcctgacttggaagcaagtgctggcagaactccaagatattgaaaatgaggagaaaatcccaaaaacaaagaacatgaaaaaaacaagccagttgaggtgggcactcggtgagaacatggcaccagagaaagtagactttgaggactgcaaagatgttagcgatctaagacagtatgacagtgatgaaccagaatctagatcactagcaagctggattcagagtgaattcaacaaggcatgtgaattgacagactcgagttggattgaacttgatgaaataggagaggatgttgctccaattgaacacattgcaagtatgagaaggaactattttacagcggaagtatcccattgcagagccactgaatacataatgaagggagtgtacataaacacagccctgttgaatgcatcctgtgcagccatggatgactttcaactgattccaatgataagcaaatgcagaaccaaagaaggaagacggaaaactaatctgtatggattcattataaaaggaagatcccacttgaggaatgataccgatgtggtaaattttgtgagtatggaattctctcttactgatccgagattggagccacacaagtgggaaaagtactgtgtcctcgagataggagacatgctcctccggactgcagtaggtcaagtttcaaggcccatgttcctgtatgtgagaaccaacggaacctccaagatcaaaatgaaatggggcatggaaatgagacgatgccttcttcaatcccttcaacaaattgaaagcatgattgaagccgaatcttctgttaaagagaaggacatgaccaaagaattctttgaaaacaaatctgaaacatggccgatcggagaatcccccaaaggagtggaggaaggctccattggaaaggtgtgcagaaccttgctggcgaagtctgtgttcaacagtttatatgcatcttcacaactcgaggggttttcagctgaatcaagaaaattgcttctcattgctcaggcacttagggacaacctggaacctgggaccttcgatcttggagggctatatgaagcaattgaggagtgcctgattaacgatccctgggttttgcttaatgcgtcttggttcaactccttcctcgcacatgcactgaaatagttggggcaatgctactacttgctatccatactgtccaaaaaagtaccttgtttctact";
    string cat_three_name = "H5N1-PA";
    string cat_four = "agcgaaagcaggggttcaatctgtcaaaatggagaaaatagtgcttctttttgcaatagtcagtcttgttaaaagtgatcagatttgcattggttaccatgcaaacaactcgacagagcaggttgacacaataatggaaaagaacgtcactgttacacacgcccaagacatactggaaaagacacacaacgggaagctctgcgatctagatggagtgaagcctctaattttaagagattgtagtgtagctggatggctcctcgggaacccaatgtgtgacgaattcctcaatgtgccggaatggtcttacatagtggagaagatcaatccagccaatgacctctgttacccaggggatttcaacgactatgaagaactgaaacacctattgagcagaataaaccattttgagaaaattcagatcatccccaaaagttcttggtcagatcatgaagcctcatcaggggtgagctcagcatgtccataccagggaaggtcctccttttttagaaatgtggtatggcttatcaaaaaggacaatgcatacccaacaataaagagaagttacaataataccaaccaagaagatcttttggtactgtgggggattcaccatccaaatgatgcggcagagcagacaaggctctatcaaaacccaaccacctatatttccgttgggacatcaacactaaaccagagattggtaccaaaaatagctactagatccaaggtaaacgggcaaagtggaaggatggagttcttttggacaattttaaaaccgaatgatgcaataaactttgagagtaatggaaatttcattgctccagaaaatgcatacaaaattgtcaagaaaggggactcaacaattatgaaaagtgaattggaatatggtaactgcaacaccaagtgtcaaactccaataggggcgataaactctagtatgccattccacaacatccaccctctcaccatcggggaatgccccaaatatgtgaaatcaaacagattagtccttgcgactgggctcagaaatagccctcaaggagagagaagaagaaaaaagagaggactatttggagctatagcaggttttatagagggaggatggcagggaatggtagatggttggtatgggtaccaccatagcaacgagcaggggagtgggtacgctgcagacaaagaatccactcaaaaggcaatagatggagtcaccaataaggtcaactcgatcattaacaaaatgaacactcagtttgaggccgttggaagggaatttaataacttagaaaggagaatagaaaatttaaacaagaagatggaagacggattcctagatgtctggacttataatgctgaacttctggttctcatggaaaatgagagaactctagactttcatgactcaaatgtcaagaacctttacgacaaagtccgactacagcttagggataatgcaaaggagcttggtaacggttgtttcgagttctatcacagatgtgataatgaatgcatggaaagtgtaagaaacggaacgtatgactacccgcagtattcagaagaagcaagattaaaaagagaggaaataagtggagtaaaattggaatcaataggaacctaccaaatactgtcaatttattcaacagtggcgagctccctagcactggcaatcatggtggctggtctatctttatggatgtgctccaatggatcgttacaatgcagaatttgcatttaaatttgtgagttcagattgtagttaaaaacacccttgtttctact";
    string cat_four_name = "H5N1-HA";
    string cat_five = "agcaaaagcagggtagataatcactcaccgagtgacatcaaaatcatggcgtctcaaggcaccaaacgatcttatgaacagatggaaactggtggagaacgccagaatgctactgagatcagggcatctgttggaagaatgattagtggcatcgggaggttctacatacagatgtgcacagaactcaaactcagtgactatgaagggaggcttatccagaacagcataacaatagagagaatggtactctctgcatttgatgaacgaaggaacagatacctggaagaacaccccagtgcggggaaggacccgaagaaaactggaggtccaatttataggaggagagacgggaaatgggtgagagagctgattctgtacgacaaagaggagatcaggaggatttggcgtcaagcgaacaatggagaggatgcaactgctggtcttacccacctgatgatatggcattccaatctaaatgatgccacatatcagagaacgagagctctcgtacgtactggaatggaccccaggatgtgctctctgatgcaagggtcaactctcccgaggagatctggagctgccggtgcagcagtgaagggggtagggacaatggtgatggagctgattcggatgataaaacgagggatcaacgaccggaatttctggagaggcgaaaatggaagaagaacaaggattgcatatgagagaatgtgcaacatcctcaaagggaaattccaaacagcagcacaaagagcaatgatggatcaggtgcgagagagcagaaatcctgggaatgctgaaattgaagatctcatttttctggcacggtctgcactcatcctgagaggatcaatagcccataagtcctgcttgcctgcttgtgtgtacggacttgctgtggccagtggatatgactttgagagagaagggtactctctggttggaatagatcctttccgtctgcttcaaaacagccaggtctttagtctcattagaccaaatgagaatccagcacataagagtcaattagtgtggatggcatgccactctgcagcatttgaggaccttagagtctcaagtttcatcagagggacaagagtggtcccaagaggacagctatccaccagaggggttcaaattgcttcaaatgagaacatggaagcaatggactccaacactcttgaactgagaagtagatattgggctataagaaccagaagcggaggaaacaccaaccagcagagggcatctgcaggacagataagcattcagcccactttctcggtacagagaaaccttcccttcgaaagagcgaccattatggcagcattcacaggaaatactgagggcagaacgtctgacatgagaactgaaatcataagaatgatggaaagtgccagaccagaagatgtgtcattccaggggcggggagtcttcgagctctcggacgaaaaggcaacgaacccgatcgtgccttcctttgacatgaataatgaaggatcttatttcttcggagacaatgcagaggagtatgacaattaaagaaaaatacccttgtttctact";
    string cat_five_name = "H5N1-NP";
    string cat_six = "agcaaaagcaggagttcaaaatgaatccaaatcagaagataataaccatcggatcaatctgtatggtaattggaatagttagcttaatgttacaaattgggaacatgatctcaatatgggtcagtcattcaattcagacaggtaatcaacgccaagctgaaccaatcagcaatactaaatttcttactgagaaagctgtggcttcagtaacattagcgggcaattcatctctttgccccattagcggatgggctgtatacagtaaggacaacagtataaggatcggttccaggggggatgtgtttgttataagagagccgttcatctcatgctcccacttggaatgcagaactttctttttgactcagggagccttgctgaatgacaagcactccaatgggactgtcaaagacagaagccctcacagaacattaatgagttgtcctgtgggtgaggctccctccccatataactcaaggtttgagtctgttgcttggtcagcaagtgcttgccatgatggcaccagttggttgacaattggaatttctggtccagacaatggggctgtggctgtattgaaatacaatggcataataacagacaccatcaagagttggaggaacaacatactgagaactcaagagtctgaatgtgcatgtgtaaatggctcttgctttactgtaatgactgatggaccaagtaatgggcaggcatcatataagatcttcaaaatggaaaaagggaaagtggttaaatcagtcgaattggatgctcctaattatcactatgaggagtgctcctgttatcctgatgccggcgaaatcacatgtgtgtgcagggataattggcatggctcaaataggccatgggtatctttcaatcaaaatttggagtatcaaataggatatatatgcagtggagttttcggagacaatccacgccccaatgatggaacaggtagttgtgatccggtgtcccctaacggggcatatggggtaaaagggttttcatttaaatacggcaatggtgtctggatcgggagaaccaaaagcactaattccaggagcggctttgaaatgatttgggatccaaatgggtggactggaacggacagtagcttttcggtgaagcaagatatcgtagcaataactgattggtcaggatatagcgggagttttgtccagcatccagaactgacaggattagattgcataagaccttgtttctgggttgagttaatcagagggcggcctaaagagagcacaatttggaccagtgggagcagcatatctttttgtggtgtaaatagtgacactgttagttggtcttggccagacggtgctgagttgccattcaccattgacaagtagtttgttcaaaaaactccttgtttctact";
    string cat_six_name = "H5N1-NA";
    string cat_seven = "agcaaaagcaggtagatgttgaaagatgagtcttctaaccgaggtcgaaacgtacgttctctctatcatcccgtcaggccccctcaaagccgagatcgcgcagaaacttgaagatgtctttgcaggaaagaacaccgatctcgaggctctcatggagtggctaaagacaagaccaatcctgtcacctctgactaaagggatgttgggatttgtattcacgctcaccgtgcccagtgagcgaggactgcagcgtagacgctttgtccagaatgccctaaatggaaatggagatccaaataatatggatagggcagttaagctatataagaagctgaaaagagaaataacattccatggggctaaggaggtcgcactcagctactcaaccggtgcactcgccagttgcatgggtctcatatacaacagaatgggcacagtgactacggaagtggcttttggcctagtgtgtgccacttgtgagcagattgcagattcacagcatcggtctcacagacagatggcaactatcaccaacccactaatcaggcatgagaacagaatggtgctggccagcactacagctaaggctatggagcagatggcgggatcaagtgagcaggcagcggaagccatggaggtcgctaatcaggctaggcaaatggtgcaggcaatgagaacaattgggactcatcctaactctagtgctggtctgagagataaccttcttgaaaatttgcaggcctaccagaaacgaatgggagtgcagatgcagcgattcaagtgatcctcttgttgttgccgcaagtatcattgggatcttgcacttgatattatggattcttgatcgtcttttcttcaaatgcatttatcgtcgccttaaatacggtttgaaaagagggccttctacggaaggagtacctgagtctatgagggaagagtatcggcaggaacagcagaatgctgtggatgttgacgatggtcattttgtcaacatagaattggagtaaaaaactaccttgtttctact";
    string cat_seven_name = "H5N1-M1";
    string cat_eight = "agcaaaagcagggtgacaaaaacataatggattccaacactgtgtcaagctttcaggtagactgctttctttggcatgtccgcaaacgatttgcagaccaagaactgggtgatgccccattccttgaccggcttcgccgagatcagaagtccctaagaggaagaggcaacactcttggtctggacatcgaaacagctactcgtgcgggaaagcagatagtggagcgaattctggaggaggagtctgatgaggcacttaaaatgccagcttcacgctacctaactgatatgactctcgaggaaatgtcaagggactggttcatgctcatgcccaagcagaaagtgacaggttccctttgcatcaaaatggaccaggcaataatggataaaaccatcatattgaaagcaaacttcagtgtgatttatgaccggttagagaccctaatactacttagagctttcacagaagaaggagcaatcgtgggagaaatctcaccattaccttctcttccaggacatactaatgaggatgtcaaaaatgcaattggcgtcctcatcggaggacttgaatggaatgataacacagttcgaatctctgaaattatacagagattcgcttggagaagcattgatgaggatgggagacttccactccctccagatcagaaacgaaaaatggcgagaacaattgagtcaaaagtttgaagaaataaggtggctgattgaagaaatacgacatagattgaaaattacagaaaacagcttcgaacagataacgtttatgcaagccttacaactactgcttgaagtggagcaagagataagaaccttctcgtttcagcttatttaatgataaaaaacacccttgtttctact";
    string cat_eight_name = "H5N1-NS1";
    trie_node* cat_tree = init_root();
    add_sequence(cat_tree, cat_one, cat_one_name);
    add_sequence(cat_tree, cat_two, cat_two_name);
    add_sequence(cat_tree, cat_three, cat_three_name);
    add_sequence(cat_tree, cat_four, cat_four_name);
    add_sequence(cat_tree, cat_five, cat_five_name);
    add_sequence(cat_tree, cat_six, cat_six_name);
    add_sequence(cat_tree, cat_seven, cat_seven_name);
    add_sequence(cat_tree, cat_eight, cat_eight_name);
    bool summary = true;
    bool print = true;
    string test_one = "agcgaaagcag"; // snippet from cat - 1
    solution_list* solution = new solution_list;
    solution = check_sequence(cat_tree, test_one, summary);
    if (print){
        
    
        cout<<"searching for string " << test_one <<endl;
         //cout << "Using corpus from '" << fn << "'" << endl;
        for (int i = 0; i < solution->ID.size(); i++){
            //printf("%f, %s ", solution->percent_hit[i], solution->ID[i]);
            cout<<solution->count[i] << " "<< solution->percent_hit[i] << " percent hits in "<< solution->ID[i] <<endl;
        }
    }
    REQUIRE(solution->percent_hit[0] == 100);
}

TEST_CASE("data import", "[import]"){
    string fn = "import_test.txt";
    trie_node* test_tree = init_root();
    REQUIRE(import_data(test_tree,fn));
    string match = "ATCGCC";
    solution_list* solution = new solution_list;
    solution = check_sequence(test_tree, match, true);
    REQUIRE(solution->percent_hit[0] == 100);
    solution = check_sequence(test_tree, match, false);
    print_sequence_match(solution->solution_start[0], match, solution->ID[0]);
    //print_solution()
    trie_node* testy_tree = init_root();
    //big time memory leak here
    solution_list* solution1 = new solution_list;
    solution_list* solution2 = new solution_list;
    fn = "fastas/Colorado_H1N1_PB2.txt";
    REQUIRE(import_data(testy_tree,fn));
    string match1 = "TCAAATA";
    solution1 = check_sequence(testy_tree, match1, true);
    REQUIRE(solution1->percent_hit[0] == 100);
    solution1 = check_sequence(testy_tree, match1, false);
    
    print_sequence_match(solution1->solution_start[0], match1, solution1->ID[0]);
    
    
    //string match2 = "TCAAATATATTCAATATGGAGAGAATAAAAGAGCTGAGAGATCTAATGTCGCAGTCCCGCACTCGCGAGATACTCACTAAGACCACTGTGGACCATATGGCCATCATCAAAAAGTACACATCGGGAAGGCAAGAGAAGAACCCCGCACTCAGAATGAAGTGGATGATGGCAATGAAATACCCAATTACGGCAGACAAGAGAATAATGGACATAATCCCAGAGAGGAATGAACAAGGACAAACCCTCTGGAGCAAAACAAACGATGCTGGATCAGACCGAGTGATGGTATCACCTCTGGCCGTAACATGGTGGAATAGGAATGGCCCAACAACAAGTACAGTTCATTACCCTAAGGTATATAAAACTTATTTCGAAAAGGTCGAAAGGTTGAAACATGGTACCTTCGGCCCTGTCCACTTCAGAAATCAAGTTAAAATAAGGAGGAGAGTTGATACAAACCCTGGCCATGCAGATCTCAGTGCCAAGGAGGCACAGGATGTGATTATGGAAGTTGTTTTCCCAAATGAAGTGGGGGCAAGAATACTGACATCAGAGTCACAGCTGGCAATAACAAAAGAGAAGAAAGAAGAGCTCCAG";
    string match2 = "TCAAATATATTCAATATGGAGAGAATAAAAGAGCTGAGAGATCTAATGTCGCAGTCCCGCACTCGCGAGATACTCACTAAGACCACTGTGGACCATATGGCCATCATCAAAAAGTACACATCGGGAAGGCAAGAGAAGAACCCCGCACTCAGAATGAAGTGGATGATGGCAATGAAATACCCAATTACGGCAGACAAGAGAATAATGGACATAATCCCAGAGAGGAATGAACAAGGACAAACCCTCTGGAGCAAAACAAACGATGCTGGATCAGACCGAGTGATGGTATCACCTCTGGCCGTAACATGGTGGAATAGGAATGGCCCAACAACAAGTACAGTTCATTACCCTAAGGTATATAAAACTTATTTCGAAAAGGTCGAAAGGTTGAAACATGGTACCTTCGGCCCTGTCCACTTCAGAAATCAAGTTAAAATAAGGAGGAGAGTTGATACAAACCCTGGCCATGCAGATCTCAGTGCCAAGGAGGCACAGGATGTGATTATGGAAGTTGTTTTCCCAAATGAAGTGGGGGCAAGAATACTGACATCAGAGTCACAGCTGGCAATAACAAAAGAGAAGAAAGAAGAGCTCCAGAATTGTAAAATTGCTCCCTTGATGGTGGCGTACATGCTAGAAAGAGAATTGGTCCGTAAAACAAGGTTTCTCCCGGTAGCTGGTGGGACAGGCAGTGTTTATATTGAGGTGTTGCACTTGACCCAAGGGACGTGCTGGGAGCAGATGTACACTCCGGGAGGAGAAGTGAGAAATGATGATGTTGACCAAAGCTTGATTATCGCTGCTAGAAACATAGTAAGAAGAGCAGCAGTGTCAGCAGACCCATTAGCATCTCTCTTGGAAATGTGCCACAGCACACAGATTGGAGGTGTGAAGATGGTGGACATCCTTAAACAGAATCCAACTGAGGAGCAAGCCGTAGACATATGCAAGGCAGCAATAGGGTTGAGGATCAGCTCATCTTTCAGTTTTGGTGGGTTCACTTTCAAAAGGACAAGCGGATCATCAGTCAAGAAAGAAGAAGAAATGCTAACGGGCAACCTCCAAACACTGAAATTAAGAGTACATGAAGGGTATGAAGAATTCACAATGGTTGGGAGAAGAGCAACAGCTATTCTCAGAAAGGCAACCAGGAGATTGATCCAATTAATAGTAAGCGGGAGAGACGAGCAGTCAATAGCTGAAGCAATAATTGTGGCCATGGTATTCTCACAAGAGGATTGCATGATCAAAGCAGTTAGGGGCGATCTGAACTTTGTCAATAGGGCAAACCAGAGACTGAATCCCATGCACCAACTCTTGAGGCATTTCCAAAAAGATGCAAAAGTGCTTTTCCAGAACTGGGGAATTGAAACCATCGACAATGTAATGGGAATGATCGGAATACTGCCCGACATGACCCCAAGCACGGAGATGTCACTGAGAGGAATAAGAGTCAGCAAGATGGGAGTAGATGAATACTCCAGCACGGAGAGAGTGGTAGTGAGTATTGACCAATTTTTGAGGGTTAGAGATCAAAGAGGAAACGTACTATTGTCTCCCGAAGAAGTCAGTGAAACGCAAGGAACTGAGAAGTTGACAATAACTTATTCGTCATCAATGATGTGGGAGATCAACGGCCCTGAGTCAGTGCTAGTCAACACTTATCAATGGATAATCAGAAACTGGGAAATTGTGAAAATTCAATGGTCACAAGACCCCACAATGTTATACAACAAAATGGAATTTGAACCATTTCAGTCTCTTGTCCCTAAGGCAACCAGAAGCCGGTACAGTGGATTCGTAAGGACACTGTTCCAGCAAATGAGGGATGTGCTTGGGACATTTGACACTGTCCAAATAATAAAACTTCTCCCCTTTGCTGCTGCTCCACCAGAACAGAGCAGGATGCAATTTTCTTCATTGACTGTGAATGTGAGAGGATCAGGGTTAAGGATACTGGTAAGAGGCAATTCTCCAGTATTCAATTACAACAAGGCAACCAAACGACTTACGGTTCTTGGAAAGGATGCAGGTGCATTGACTGAAGATCCAGATGAAGGCACATCTGGGGTGGAGTCTGCTGTCCTGAGAGGATTCCTCATTTTAGGCAAAGAAGACAAGAGATATGGCCCAGCATTAAGCATCAATGAACTGAGCAATCTTGCAAAAGGAGAGAAAGCTAATGTGCTAATTGGGCAGGGGGACATAGTGTTGGTAATGAAACGAAAACGGGACTCTAGCATACTTACTGACAGCCAGACAGCGACCAAAAGAATTCGGATGGCCATCAATTAGTGTCGAATTGTTTAAAAACGAAA";
    //solution = check_sequence(test_tree, match, true);
    //REQUIRE(solution->percent_hit[0] == 100);
    solution2 = check_sequence(testy_tree, match2, false);
    
    print_sequence_match(solution2->solution_start[0], match2, solution2->ID[0]);
    
   
    /*fstream fin(fn, fstream::in);
    char ch;
    string strain_name;
    string sequence;
    stringstream current_s;
    int counter = 0;
    cout<<"import test: char:"<<endl;
   //cout<< fin>>ch;
    while (fin >> noskipws >> ch) {
        if (ch == '>'){
            counter++;
            if (counter == 2){
                strain_name = current_s.str();
                current_s.str(string());
            }
        }
        else {
          current_s << ch;
        }
        //increment_lookup(lookup, ch);
        cout<<ch;
    }
    sequence = current_s.str();
    cout<<"import test: sstream: "<<endl;
    cout<<"strain name: "<<strain_name<<endl;
    cout<<"sequence: "<<sequence<<endl;
    cout<<"import test : "<<endl;*/
}
TEST_CASE("mRNA: UI", "[UI]"){
    bool exit = UI();
    REQUIRE(exit == true);
}
//test 5 bp string


//test 20 bp string


