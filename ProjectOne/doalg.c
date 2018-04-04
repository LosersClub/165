#include "utils.c"
#include "sorts.c"
#include "floyd_select.c"
#include "floyd_select_debug.c"

int doalg(int n, int k, int* out) {
  int maxSize = 10000;
  int indices[10000];
  for (int i = 0; i < maxSize; i++) {
    indices[i] = i;
  }

  int right = n - 1;
  int left = 0;
  select(indices, left, right, k-1);
  insertionSort(indices, left, k-1);

  for (int i = 0; i < k; i++) {
    out[i] = indices[i] + 1;
  }

  return 1;
}

int doalg2(int n, int k, int* out) {
  //dshrandom(1234); // Random Seed
  int test[10000];
  int indices[10000];
  int nl = 100;
  int k1 = 10;
  for (int i = 0; i < nl; i++) {
    indices[i] = i;
    test[i] = i + 1;
  }
  for (int i = nl; i > 0; i--) {
    int j = i * dshrandom(0) + 1;
    if (j != i) {
      int t = test[i - 1];
      test[i - 1] = test[j - 1];
      test[j - 1] = t;
    }
  }

  //for (int i = 0; i < nl; i++) {
  //  printf("%d ", test[i]);
  //}
  printf("\n");

  int r = nl - 1;
  int l = 0;
  selectDebug(test, indices, l, r, k1);
  printf("%dth: %d", k1 + 1, test[indices[k1]]);
  insertionSortDebug(test, indices, 0, k1);
  
  printf("\nCompares: %d\n", compareCount);
  for (int i = 0; i <= k1; i++) {
    printf("%d ", test[indices[i]]);
  }
  printf("\n");

  return 0;
}