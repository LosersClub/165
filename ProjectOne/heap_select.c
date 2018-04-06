#include <math.h>
#include <stdlib.h>
buildHeap(int* indices, int k, int n) {
  int* results = malloc(sizeof(int)*(k+1));
  for (int i = 0; i <= k; i++) {
    results[i] = indices[i];
  }
  for (int i = floor(k / 2); i >= 0; i--) {
    heapify(results, i, k);
  }
  for (int i = k+1; i < n; i++) {
    if (compare(indices[i], results[0]) > 0) {
      results[0] = indices[i];
      heapify(results, 0, k);
    }
  }
  for (int i = 0; i <= k; i++) {
    indices[i] = results[i];
  }
  free(results);
}

heapify(int* results, int parentIndex, int k) {
  int leftChildIndex = 2*parentIndex + 1;
  int rightChildIndex = 2*parentIndex + 2;
  int smallestIndex = parentIndex;
  if (leftChildIndex <= k && compare(results[leftChildIndex], results[parentIndex]) < 0) {
    smallestIndex = leftChildIndex;
  }
  if (rightChildIndex <= k && compare(results[rightChildIndex], results[smallestIndex]) < 0) {
    smallestIndex = rightChildIndex;
  }
  if (smallestIndex != parentIndex) {
    swap(&results[parentIndex], &results[smallestIndex]);
    heapify(results, smallestIndex, k);
  }
}