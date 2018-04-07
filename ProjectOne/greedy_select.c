#include <math.h>
#include <stdlib.h>

int bst(int* indices, int left, int right, int val);

int greedySelect(int* indices, int k, int n) {
  int* results = malloc((k + 1) * sizeof(int));
  for (int i = 0; i <= k; i++) {
    results[i] = indices[i];
  }
  mergeSort(results, 0, k);

  for (int i = k + 1; i < n; i++) {
    if (compare(indices[i], results[k]) > 0) {
      results[k] = indices[i];

      int left = 0;
      int right = k - 1;
      while (left <= right) {
        int mid = left + (right - left) / 2;
        if (compare(results[mid], results[k]) < 0) {
          right = mid - 1;
        } else {
          left = mid + 1;
        }
      }

      for (int x = k; x > left; x--) {
        swap(&results[x], &results[x - 1]);
      }
    }
  }

  for (int i = 0; i <= k; i++) {
    indices[i] = results[i];
  }
  free(results);
}