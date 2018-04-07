#include <math.h>
#include <stdlib.h>

minHeapify(int* results, int parentIndex, int k);

heap_select(int* indices, int k, int n) {
  int* results = malloc(sizeof(int)*(k + 1));
  for (int i = 0; i <= k; i++) {
    results[i] = indices[i];
  }
  for (int i = floor(k / 2); i >= 0; i--) {
    minHeapify(results, i, k);
  }
  for (int i = k + 1; i < n; i++) {
    if (compare(indices[i], results[0]) > 0) {
      results[0] = indices[i];
      minHeapify(results, 0, k);
    }
  }
  for (int i = 0; i <= k; i++) {
    indices[i] = results[i];
  }
  free(results);
}

minHeapify(int* results, int parentIndex, int k) {
  int leftChildIndex = 2 * parentIndex + 1;
  int rightChildIndex = 2 * parentIndex + 2;
  int smallestIndex = parentIndex;
  if (leftChildIndex <= k && compare(results[leftChildIndex], results[smallestIndex]) < 0) {
    smallestIndex = leftChildIndex;
  }
  if (rightChildIndex <= k && compare(results[rightChildIndex], results[smallestIndex]) < 0) {
    smallestIndex = rightChildIndex;
  }
  if (smallestIndex != parentIndex) {
    swap(&results[parentIndex], &results[smallestIndex]);
    minHeapify(results, smallestIndex, k);
  }
}

heap_select_max(int* indices, int k, int n) {
  int* results = malloc(sizeof(int)*(n));
  for (int i = 0; i < n; i++) {
    results[i] = indices[i];
  }
  for (int i = floor(n / 2); i >= 0; i--) {
    maxHeapify(results, i, n);
  }
  for (int i = 0; i <= k; i++) {
    indices[i] = extractMax(results, &n);
  }
  free(results);
}

maxHeapify(int* results, int parentIndex, int n) {
  int leftChildIndex = 2 * parentIndex + 1;
  int rightChildIndex = 2 * parentIndex + 2;
  int largestIndex = parentIndex;
  if (leftChildIndex < n && compare(results[leftChildIndex], results[largestIndex]) > 0) {
    largestIndex = leftChildIndex;
  }
  if (rightChildIndex < n && compare(results[rightChildIndex], results[largestIndex]) > 0) {
    largestIndex = rightChildIndex;
  }
  if (largestIndex != parentIndex) {
    swap(&results[parentIndex], &results[largestIndex]);
    maxHeapify(results, largestIndex, n);
  }
}

int extractMax(int* results, int* n) {
  if (n == 1) {
    return results[0];
  }
  int toReturn = results[0];
  results[0] = results[*n - 1];
  *n -= 1;
  maxHeapify(results, 0, *n);
  return toReturn;
}