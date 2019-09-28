About this project:

We were instructed to use a data structure to write a program that has *some* real world application.

Project mRNA overview:
mRNA is a program for string matching, specifically tailored to matching messeger RNA (mRNA) sequences. 
The idea being, that a researcher could recieve a sequence of mRNA from a sample, and be able to identify it from a known database of mRNA sequences.
The program should then print out a list of likely hits, in order of highest probability.  The researcher could then ask for the start position of each string match and/or 
print out the sequence to examine the base pair hits/misses.
The program uses a trie tree data structure to store its database, and takes strings of nuclaeic acids and attempts to
match them to the database. The algorithm allows for "fuzzy matching" by having two recursive loops for matching. The outer loop
checks every node in the tree as a potential starting node for a string match and the inner loop goes throught he solutino until
it reaches the end of the search string or a hit threshold is reached, at which point the solution is deemed invalid, and the inner loop 
terminates and returns to the outer loop. Once all nodes in the tree have been checked as potential starting noeds for a sequence match,
the program returns a list of all matches (if any). 

Over the course of the project, several key aspects and goals changed. 
1. mRNA data is harder to obtain/work with than DNA data, so the test database changed from mRNA to viral DNA (flu data).

2. Working with DNA instead of mRNA exasperated some of the issues with assuming correct string matching alignment, and introduced the possibility of
searching for a longer string than the database contained, which was easily solved at the 3' (the tail) end of the strings, but was not solved for the 5' (head) end.
Example being, if you search the human flu database included in this project, and call a BLAST on South Korea flu, you find a match in Colorado flu strains, but when you BLAST
the Colorado flu strain, it does not detect a match with the South Korean Flu, because the Colorado flu sequence is longer than the South Korean flu sequence. This is because initially, checking whole genomes was not the inteded use of this application, but I found it interesting to do so anyway.

3. When checking smaller real world patien flu sequences, the reality of the flu makes my example not as cool, as the flu mutates a lot, and some of the results can be disappinting.
For example: If you do a BLAST on region UK1 - a 2018 flu patient from England, with random sequence segments generated form the patient's flu's genome, the program perfectly detects witch part of the flu genome the sequence belongs to, but due to the variations in flu, is unable to accurately predict the geographic region that flu strain came from. This could potentially be solved by increasing the database size.

Data structures used:
The database is stored as a trie tree, with the stating node (the root) being labeled as 5'. This is a convention from biology, as
mRNA is read from the 5' cap - a group of proteins at the 5' end of the molecule protecting that end from degrading. The tail is composed of adenosine molecules
called the poly A tail. They slowly fall off the mRNA molecule providing a lifespan for the messenger RNA. 

The algorithms were designed with the structure of RNA in mind, hence string alignment was not a top priority during development as all RNA molecules should start at the 5' cap, or head postion.
The goal of the project was molecule identification, and the program can successfully identify genes and molecules. The program has several shortcomings when comparing whole genomes mainly being string alignment, and threshold level. If the threshold is set too low for large sequences, then the program runs a bit slow, though that could be argued to be user error.

Run time:
The mRNA project runs in worst case scenario of:
symbol definitions:
n = number of nodes in the database
s = length of the search string
percent error = the minimum amount of error for string detection i.e. 90% string match is the default
hit threshold = floor(S * (1 - percent_error))

WORST CASE:
O (n x s)
To achieve the worst case run time, you would either need to search for a string that will have multiple 100% hits in a database. This can be achieved by searching for sequence AAAAAAAAAA
in a large database of A nodes, or you could search for any string with a percent error of 0%, making the threshold equal to the string length. Either way, the search would be pretty stupid and the results would be meaningless.

AVERAGE CASE:
Theta (n * (5/4) * floor(s * 1 - percent_error))
This is the average case scenario. The 5/4 comes form the probablility of getting a bp hit (1/4) chance that the bp you are comparing to is the same.

BEST Case:
Ohm (n )
percent_error set to 100%, making the error threshold equal to 0, and therefore only checking each node in the database once.

Future development:
1. In order to save space in the database itself, the first imporvement will be to index all the gene names, and then instead of storing an array of rather long strings in each node, I would then store an array of integers on each trie node, saving space. I would then use the integer to look up the gene name after the algorithm has completed the recusive comparisons to retrieve the gene name. This should be a simple implementation that should have a clear improvement on storage useage.

2. Change the structure of the trie tree to reference a variable for its number of children to allow for protein sequences to be compared and computed as well as DNA/RNA. With nucleic acid, each node has a maximum of 4 children, so I initially hardcoded 4 child nodes into the structure, but allowing that number to be variable would allow for protein stings which would have up to 21 child nodes, making the trie tree much shorter but wider, and therefore probably more efficient.

3. Fix the head alignment issue. This will be rather difficult to fix without rewriting the searching algorithm. Currently, the best method I can think of to fix this is to break up the searching string into smaller sequences and run those through database and combine the results afterwards. This would help with the overlap issue, but would require a lot more work in terms of rewriting the existing code and creating additional tests. This change/improvement is slated last even though it could have the largest impact on the project because assuring that the strings line up is out of the initial scope of the project. 