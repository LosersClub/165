#include "utils.c"
#include "sorts.c"
#include "floyd_select.c"

int doalg(int n, int k, int* out) {
  int test[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
  int indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  int nl = 20;

  int r = 19;
  int l = 0;
  int k1 = 3;
  wirthExchange(test, indices, l, r, k1);
  
  printf("Indices: ");
  for (int i = 0; i < nl; i++) {
    printf("%d ", indices[i]);
  }
  printf("\nValues: ");
  for (int i = 0; i < nl; i++) {
    printf("%d ", test[indices[i]]);
  }
  printf("\nCompares: %d\n", compareCount);
  printf("%dth largest: %d\n", k1+1, test[indices[k1]]);

  return 0;
}