//#define DEBUG
#define DEBUG_SIZE 10000
#define DEBUG_K 40

#include <stdlib.h>
#include <math.h>

#include "utils.c"
#include "tournament_select.c"

int doalgFinal(int n, int k, int* out) {
  tournamentSelect(n, k, out);

  // Converting our 0-based to 1-based indexing for COMPARE.c
  for (int i = 0; i < k; i++) {
    out[i] += 1;
  }
  return 1;
}

int doalg(int n, int k, int* out) {
#ifdef DEBUG
  for (int i = 0; i < DEBUG_SIZE; i++) {
    debug[i] = i + 1;
  }
  for (int i = DEBUG_SIZE; i > 0; i--) {
    int j = i * dshrandom(0) + 1;
    if (j != i) {
      int t = debug[i - 1];
      debug[i - 1] = debug[j - 1];
      debug[j - 1] = t;
    }
  }
  int best[DEBUG_K];
  doalgFinal(DEBUG_SIZE, DEBUG_K, best);
  printf("Best: ");
  for (int i = 0; i < DEBUG_K; i++) {
    printf("%d ", debug[best[i] - 1]);
  }
  printf("\nComparisons: %d\n", compareCount);
  return 0;
#endif
  
  return doalgFinal(n, k, out);
}