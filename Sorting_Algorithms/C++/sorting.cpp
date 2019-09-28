/*

  sorting.cpp
******************************
*  Carter Reid               *
*  reidcc@colorado.edu       *
*  CSCI2270 Homework 3       * 
******************************

  Please refer to sorting.h for documentation on each function.

   + ------ N O T E: -- Big scary warning! -----------------------------\
   |                                                                    |
   |    We clearly can't use unit testing to ensure that you're         |
   |    actually implementing the correct algorithm. I suppose we could |
   |    write some complicated static analysis checker. But it is       |
   |    easier to just look at it with our eyeballs.                    |
   |                                                                    |
   |    After the assignment is due, we will look at the most recent    |
   |    version of your implementation that got the highest score and   |
   |    make sure you're not doing something sneaky like using a        |
   |    standard library call to sort a vector.                         |
   |                                                                    |
   |    Anyone caught doing this (even though it is kinda funny) will   |
   |    be given 0 points on the assignment.                            |
   |                                                                    |
   `--------------------------------------------------------------------/ 

 */

#include "sorting.h"
#include <iostream>

using namespace std;

/*quicksort recursively sorts a list/array/vector utilizing a partitioning scheme to sort all values samller than a pivot point
into region 1, which is indexed with i, and all values greater than pivot into region 2, indexed with j. Values in region 1 will always be less 
than values in region 2. Quicksort itself merely calls the partioning scheme (and itself) recursively until the list is sorted.
*/
void quicksort(vector<int> &data, int low_idx, int high_idx) {
    
    //quicksort needs to partition and recurse until low index is equal to high index, then sorting is complete
    if(low_idx < high_idx)
    {
        int p = quicksort_partition(data, low_idx, high_idx); 
        //recurse with new partition
        quicksort(data, low_idx, p-1);
        quicksort(data, p+1, high_idx);
    }
    else
    {
        return;
    }
}

int quicksort_partition(vector<int> &data, int low_idx, int high_idx) {

    //going to declare the pivot as the last element in the partition because I am lazy and do not want to implement the median of medians
    //or use a random pivot
    
    int x = data[high_idx]; //declaring pivot as high index
    int i = low_idx - 1;
    int j = low_idx;
    
    //run through Lomuto's partition scheme
    while (j < high_idx)
    {
        //if index at j is less than pivot, move it to region 1
        if(data[j] < x) 
        {
            i++;
            swap(data[i],data[j]);
        }
        j++;    
    }
    //check first location of region 2 (i+1) vs pivot. if pivot value belongs there, swap them
    if (data[high_idx] < data[i+1])
    {
        swap(data[i+1],data[high_idx]);
    }
    return i+1; //return i +1 to change the partition/indicies for further recursion
}
/*bubble sort sorts a list with comparisons between one value (indexed i) and its following value (i+1).
It uses 2 for loops and a compare/swap statement... has a poor run time. */
void bubblesort(vector<int> &data) {
    int n = data.size();
    
    //bubble sort... 2 loops and a comparison statement.
    for (int i=0; i < n; i++)
    {
        for (int j=0; j < n-i-1; j++)
        {
            if(data[j] > data[j+1])
            {
                swap(data[j],data[j+1]);
            }
        }
    }
    return;
    
}
/*mergesort is a divide and conquer algorithm. The idea is to divide the array into size 1 or 2 arrays, but 
my take is to go all the way down to size 1 arrays, as those are automatically sorted and will be merged together in order in the 
merge function. The psudo code that I saw from CSCI 3104 had a compare/swap for size 2 arrays, but I didn't think that was necessary
and from a run time analyisis, probably not a big difference. */
void mergesort(vector<int> &data) {

    //base case:
    if (data.size() == 1)
    {
        return;
    }
    else //recursive call to divide more
    {
        int mid = data.size()/2;
        vector<int> left = vector<int>(data.begin(), data.begin() + mid);
        vector<int> right = vector<int>(data.begin() + mid, data.begin() + data.size());
       
        mergesort(left);
        mergesort(right);
        data = merge(left, right);
    }

}

vector<int> merge(vector<int> &left, vector<int> &right) {

    //temporary vector declaration and memory allocation
    //using a temp vector here is not as memory efficient as a sort in place method, but seems like
    //the easiest solution given the function parameters.
    vector<int> temp;
    temp.reserve(left.size() + right.size()); //probably unnecessary but I want to build good habits
    
    //using unsigned ints to make the compiler happier because I will be comparing them to .size() which is also an unsigned value
    unsigned int i = 0; 
    unsigned int j = 0;
    //while both sorted sub arrays have values, append the smallest element from either array
    while((i <= (left.size()-1)) && (j <= (right.size()-1)))
    {
        if (left[i] <= right[j])
        {
            temp.push_back(left[i]);
            i++;
        }
        else
        {
            temp.push_back(right[j]);
            j++;
        }
    }
    //if the sorted right sub array has terminated, append from the remaining sorted left array
    while (i <= left.size()-1)
    {
        temp.push_back(left[i]);
        i++;
    }
    //if the sorted left sub array has terminated, append from the remaining sorted right array
    while(j <= right.size()-1)
    {
        temp.push_back(right[j]);
        j++;
    }
    return temp; //return the temporary array to overwrite data.

}

/*stooge sort compares the indexed value at the start to the last indexed value and swaps them if they are out of order
It partitions into 3 sections and recusively sorts those. It's similar to slowsort in that its more of a 
 multiply and surrender algorithm than a divide and conquer. It has a terrible run time, but was fun to code as I was laughing
the entire time. Enjoy this beautiful work of art! */
vector<int> stooge_sort(vector<int> &data, int i, int j){
    if(data[i] > data[j]) //if index at the start is larger than the last value, swap
    {
        swap(data[i],data[j]);
    }
    if ((j-i+1) > 2) //if not the base case recurse more!
    {
        int t = (j-i+1)/3; //partition into 3 sections rounding up
        stooge_sort(data, i, j-t); //sort the initial 2/3 of the list
        stooge_sort(data, i+t, j); //sort the final 2/3 of the list
        stooge_sort(data, i, j-t); //sort the initial 2/3 of the list again because this is a stupid algorithm
    }
    return data; //return data after a long time
}

void mystery_sort(vector<int> &data) {
  // Algorithm implemented: Stooge sort
  //I'll probably never have another opportunity to write this sorting algorithm due to its exceptionally bad time complexity.
    int i = 0;
    int j = data.size()-1;
    stooge_sort(data, i, j);
    return;
}

