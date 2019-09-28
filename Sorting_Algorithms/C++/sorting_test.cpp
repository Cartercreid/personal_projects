//
// sorting_test.cpp
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sorting.h"
#include <iostream>
#include <vector>

using namespace std;

// Forward declarations for helper functions. They are defined after
// the test cases.

vector<int> get_random_data(int how_many, int upper_bound);
bool vector_contains(vector<int> &data, int value);
bool vectors_contain_same(vector<int>& blue, vector<int> &green);
bool is_nondecreasing(vector<int> &data);
vector<int> get_copy(vector<int> &data);


// Test cases

TEST_CASE("Sorting: Sanity Check", "[sanity]") {
  vector<int> data;
  data.push_back(5);
  data.push_back(25);
  data.push_back(35);
  data.push_back(25);
  data.push_back(55);

  vector<int> response;
  response.push_back(5);
  response.push_back(25);
  response.push_back(30);
  response.push_back(25);
  response.push_back(5);

  vector<int> good_response;
  good_response.push_back(5);
  good_response.push_back(25);
  good_response.push_back(25);
  good_response.push_back(35);
  good_response.push_back(55);

  REQUIRE(vector_contains(data, 5));
  REQUIRE_FALSE(vector_contains(data, 6));
  REQUIRE_FALSE(vectors_contain_same(data, response));
  REQUIRE(vectors_contain_same(data, good_response));
  REQUIRE_FALSE(is_nondecreasing(response));
  REQUIRE(is_nondecreasing(good_response));
}

TEST_CASE("Sorting: QuickSort", "[quick]") {
  vector<int> input_data = get_random_data(1000, 500);
  vector<int> input_data_orig = get_copy(input_data);
  size_t size_before = input_data.size();
  quicksort(input_data, 0, (int) size_before-1);
  REQUIRE(input_data.size() == size_before);
  REQUIRE(vectors_contain_same(input_data, input_data_orig));
  REQUIRE(is_nondecreasing(input_data));
}

TEST_CASE("Sorting: BubbleSort", "[bubble]") {
  vector<int> input_data = get_random_data(1000, 500);
  vector<int> input_data_orig = get_copy(input_data);

  size_t size_before = input_data.size();
  bubblesort(input_data);
  REQUIRE(input_data.size() == size_before);
  REQUIRE(vectors_contain_same(input_data, input_data_orig));
  REQUIRE(is_nondecreasing(input_data));
}

TEST_CASE("Sorting: MergeSort", "[merge]") {
  vector<int> input_data = get_random_data(1000, 500);
  vector<int> input_data_orig = get_copy(input_data);

  size_t size_before = input_data.size();
  mergesort(input_data);
  REQUIRE(input_data.size() == size_before);
  REQUIRE(vectors_contain_same(input_data, input_data_orig));
  REQUIRE(is_nondecreasing(input_data));
}

TEST_CASE("Sorting: MysterySort", "[mystery]") {
  vector<int> input_data = get_random_data(1000, 500);
  vector<int> input_data_orig = get_copy(input_data);

  size_t size_before = input_data.size();
  mystery_sort(input_data);
  REQUIRE(input_data.size() == size_before);
  REQUIRE(vectors_contain_same(input_data, input_data_orig));
  REQUIRE(is_nondecreasing(input_data));

}

vector<int> get_random_data(int how_many, int upper_bound) {
  vector<int> numbers;
  for (int i=0; i < how_many; i++) {
    int r = rand() % upper_bound;
    numbers.push_back(r);
  }
  return numbers;
}

bool vector_contains(vector<int> &data, int value) {
  bool ret = false;
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i] == value) {
      ret = true;
      break;
    }
  }
  return ret;
}

bool vectors_contain_same(vector<int>& blue, vector<int> &green) {
  bool ret = true;
  if (blue.size() != green.size()) {
    ret = false;
  } else {
    for (size_t i=0; i < blue.size(); i++) {
      int target = blue[i];
      bool found = vector_contains(green, target);
      if (!found) {
	ret = false;
	break;
      }
    }
  }
  return ret;
}

bool is_nondecreasing(vector<int>& data) {
  bool ret = true;
  if (data.size() > 1) {
    int prev = data[0];
    for (size_t i=1; i < data.size(); i++) {
      int here = data[i];
      if (prev > here) {
	ret = false;
	break;
      }
      prev = here;
    }
  }
  return ret;
}

vector<int> get_copy(vector<int> &data) {
  vector<int> ret;
  for (size_t i=0; i < data.size(); i++) {
    ret.push_back(data[i]);
  }
  return ret;
}
