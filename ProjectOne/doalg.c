#include <stdlib.h>
#include <math.h>
#include "utils.c"
#include "tournament_select.c"
// Anastasia Miles (90039862) 
// Kyle Bartz (27024310)

int doalg(int n, int k, int* out) {
  tournamentSelect(n, k, out);

  // Converting our 0-based to 1-based indexing for COMPARE.c
  int i;
  for (i = 0; i < k; i++) {
    out[i] += 1;
  }
  return 1;
}