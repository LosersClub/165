#include <stdlib.h>
#include <math.h>
#include "utils.c"
#include "tournament_select.c"
// Anastasia Miles (90039862) 
// Kyle Bartz (27024310)

int doalg(int n, int k, int* out) {
  if (n < 10 || n > 10000) {
    printf("Error: n must be between 10 and 10000\n");
    return -1;
  }
  if (k < 1 || k > n || k > 100) {
    printf("Error: k must be between 1 and 100 and less than n\n");
    return -1;
  }

  tournamentSelect(n, k, out);

  // Converting our 0-based to 1-based indexing for COMPARE.c
  int i;
  for (i = 0; i < k; i++) {
    out[i] += 1;
  }
  return 1;
}