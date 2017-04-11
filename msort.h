#ifndef MSORT_H
#define MSORT_H

#include <vector>

template <class T, class Comparator>
void mergeSort(vector<T>& myArray, Comparator comp) {
	vector<T> other(myArray);
	mergeSortHelper(other, myArray, 0, other.size(), comp);
}

template <class T, class Comparator>
void mergeSortHelper(vector<T>& input, vector<T>& output, int start, int end, Comparator comp) {

	// base case:
	if (start >= end) { return; }

	// resursive case
	else {
		int middle = (start + end) / 2;
		mergeSortHelper(input, output, start, middle, comp);
		mergeSortHelper(input, output, middle, end, comp);
		merge(input, output, start, middle, middle, end, start, comp);
		copy(input, output);
	}
}

template <class T, class Comparator>
void merge(vector<T>& input, vector<T>& output, int s1, int e1, int s2, int e2, int index, Comparator comp) {
	
	// base case: reach the end of both halves
	if (s1 == e1 && s2 == e2) {
		return;
	}

	// recursive case:
	else {

		// reach the end of the first half
		if (s1 == e1) {
			output[index] = input[s2];
			mergeSortHelper(input, output, s1, e1, s2 + 1, e2, index + 1, comp);
		}

		// reach the end of the second half
		else if (s2 == e2) {
			output[index] = input[s1];
			mergeSortHelper(input, output, s1 + 1, e1, s2, e2, index + 1, comp);
		}

		// in the middle: compare s1 and s2
		else {
			if (comp(input[s1], input[s2])) {
				output[index] = input[s1];
				mergeSortHelper(input, output, s1 + 1, e1, s2, e2, index + 1, comp);
			}
			else {
				output[index] = input[s2];
				mergeSortHelper(input, output, s1, e1, s2 + 1, e2, index + 1, comp);
			}
		}
	}
}

template <class T, class Comparator>
void copy(vector<T>& input, vector<T>& output) {
	input = output;
}

#endif