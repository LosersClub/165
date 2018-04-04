#ifdef DEBUG
static int debug[DEBUG_SIZE];
static int compareCount = 0;
#endif

typedef enum {false, true} bool;

int compare(int x, int y) {
#ifdef DEBUG
  if (debug[x] > debug[y]) {
    compareCount++;
    return 1;
  }
  if (debug[y] > debug[x]) {
    compareCount++;
    return -1;
  }
  return 0;
#endif

  int flag = COMPARE(x + 1, y + 1);
  if (flag == 2) {
    return -1;
  }
  if (flag == 1) {
    return 1;
  }
  return 0;
}

void swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int sign(double a) {
  return a < 0 ? -1 : 1;
}

int max(double a, double b) {
  return a < b ? (int)b : (int)a;
}

int min(double a, double b) {
  return a < b ? (int)a : (int)b;
}