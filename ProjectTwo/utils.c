#ifdef DEBUG
static int debug[N_DEBUG + 1];
static int queryCount = 0;
#endif

int doQuery(int a, int b, int c, int d) {
  int query[4] = { a, b, c, d };
  return QUERY(query);
}

int QUERY(int* array) {
#ifdef DEBUG
  int total = 0;
  for (int i = 0; i < 4; i++) {
    int val = array[i];
    if (val < 1 || val > N_DEBUG) {
      printf("Index in query is out of range\n");
      return -1;
    }
    for (int j = i + 1; j < 4; j++) {
      if (array[j] == val) {
        printf("Duplicate index %d in query\n", val);
      }
    }
    total += debug[val];
  }
  queryCount++;
  total = 4 - 2 * total;
  return total < 0 ? -total : total;
#endif

  return QCOUNT(1, array);
}