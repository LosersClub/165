#include <math.h>
#include <stdlib.h>

int binarySearch(int* array, int left, int right);

int greedySelect(int* indices, int k, int n) {
  int* results = malloc((k + 1) * sizeof(int));
  //mergeSort(indices, 0, k);
  for (int i = 0; i <= k; i++) {
    results[i] = indices[i];
    binarySearch(results, 0, i);
  }

  for (int i = k + 1; i < n; i++) {
    if (compare(indices[i], results[k]) > 0) {
      results[k] = indices[i];

      binarySearch(results, 0, k);

      //int left = 0;
      //int right = k - 1;
      //while (left <= right) {
      //  int mid = left + (right - left) / 2;
      //  if (compare(results[mid], results[k]) < 0) {
      //    right = mid - 1;
      //  } else {
      //    left = mid + 1;
      //  }
      //}

      //for (int x = k; x > left; x--) {
      //  swap(&results[x], &results[x - 1]);
      //}
    }
  }

  for (int i = 0; i <= k; i++) {
    indices[i] = results[i];
  }
  free(results);
}

/*
 * Binary Searches between left and right - 1 to find where to
 * place value stored in right.
 */
int binarySearch(int* array, int left, int right) {
  int val = right--;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (compare(array[mid], array[val]) > 0) {
      left = mid + 1;
     } else {
      right = mid - 1;
    }
  }
  //int swapIndex = left;
  //if (compare(array[swapIndex], array[val]) > 0) {
  //  swapIndex++;
  //}

  for (int i = val; i > left; i--) {
    swap(&array[i], &array[i - 1]);
  }
}