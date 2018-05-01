#define DEBUG
//#define NO_MAJORITY
#define PRINT_DATA
#define N_DEBUG 2000

#include "utils.c"
#include "graph_approach.c"

int mysubFinal(int n) {
  return execute(n);
}

int mysub(int n) {
#ifdef DEBUG
  int oneCount = 0;
#ifndef NO_MAJORITY
  for (int i = 1; i <= N_DEBUG; i++) {
    debug[i] = dshrandom(0) < 0.5 ? 0 : 1;
    oneCount += debug[i];
  }
#else
  for (int i = 1; i <= N_DEBUG; i+=2) {
    int val = dshrandom(0) < 0.5 ? 0 : 1;
    debug[i] = debug[i + 1] = val;
    oneCount += val * 2;
  }
#endif
#ifdef PRINT_DATA
  printf("Marbles: ");
  for (int i = 1; i <= N_DEBUG; i++) {
    printf("%d ", debug[i]);
  }
  printf("\n\n");
#endif

  int answer = (oneCount < N_DEBUG - oneCount) ? 0 : (oneCount > N_DEBUG - oneCount) ? 1 : -1;

  int predict = mysubFinal(N_DEBUG);
  printf("QueryCount: %d\n", queryCount);
  if ((predict == 0 && answer < 0) || (predict > 0 && debug[predict] == answer)) {
    if (answer == -1) {
      printf("Correct. No majority\n");
    } else {
      printf("Correct. %d was majority\n", answer);
    }
  }
  else {
    if (predict == 0) {
      printf("Guessed %d but was %d\n", -1, answer);
    } else {
      printf("Guessed %d but was %d\n", debug[predict], answer);
    }
  }
  return -1;
#endif

  return mysubFinal(n);
}