#include <stdlib.h>

#define COMPARELIMIT 10000
#define KLIMIT 100
void insertionSort(int* arr, int left, int right) {
  int n = right - left + 1;
  for (int i = left + 1; i < n; i++) {
    int j = i;
    while (j > 0 && compare(arr[j - 1], arr[j]) < 0) {
      swap(&arr[j], &arr[j - 1]);
      j -= 1;
    }
  }
}

mergeSort(int* indices, int left, int right) {
  if (left < right) {
    int midpoint = floor((right + left) / 2);
    mergeSort(indices, left, midpoint);
    mergeSort(indices, midpoint + 1, right);
    merge(indices, left, midpoint, right);
    
  }
}

merge(int* indices, int left, int midpoint, int right) {
  int leftSize = midpoint - left + 1;
  int rightSize = right - midpoint;
  int* leftArray = malloc(sizeof(int)*leftSize);
  int* rightArray = malloc(sizeof(int)*rightSize);
 
  for (int i = 0; i < leftSize; i++) {
    leftArray[i] = indices[left + i];
  }
  for (int j = 0; j < rightSize; j++) {
    rightArray[j] = indices[midpoint + j + 1];
  }


  int leftTracker = 0;
  int rightTracker = 0;
  int indicesTracker = left;

  while (leftTracker < leftSize && rightTracker < rightSize) {
    if (compare(leftArray[leftTracker], rightArray[rightTracker]) > 0) {
      indices[indicesTracker] = leftArray[leftTracker];
      leftTracker++;
    } else {
      indices[indicesTracker] = rightArray[rightTracker];
      rightTracker++;
    }
    indicesTracker++;
  }
  while (leftTracker < leftSize) {
    indices[indicesTracker] = leftArray[leftTracker];
    indicesTracker++;
    leftTracker++;
  }
  while (rightTracker < rightSize) {
    indices[indicesTracker] = rightArray[rightTracker];
    indicesTracker++;
    rightTracker++;
  }

  free(leftArray);
  free(rightArray);
}