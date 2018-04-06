#include <math.h>
#include <stdlib.h>
int greedySelect(int* indices, int k, int n) {
  int* results = malloc((k + 1) * sizeof(int));
  for (int i = 0; i <= k; i++) {
    results[i] = indices[i];
  }
  mergeSort(results, 0, k);
  for (int i = k + 1; i < n; i++) {
    if (compare(indices[i], results[k]) > 0) {
      results[k] = indices[i];
      for (int j = k; j > 0 && compare(results[j], results[j - 1]) > 0; j--) {
        swap(&results[j], &results[j - 1]);
      }
    }
  }

  for (int i = 0; i <= k; i++) {
    indices[i] = results[i];
  }
  free(results);
}