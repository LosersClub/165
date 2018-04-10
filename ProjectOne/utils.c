#ifdef DEBUG
static int debug[DEBUG_SIZE];
static int compareCount = 0;
#endif

typedef enum {false, true} bool;

int compare(int x, int y) {
  if (x < 0 || y < 0) {
    return y < 0 ? 1 : -1;
  }

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


// TODO: Reduce comparisons?
void median3(int* indices, int a, int b, int c) {
  if (compare(indices[a], indices[c]) < 0) {
    swap(&indices[a], &indices[c]);
  }
  if (compare(indices[a], indices[b]) < 0) {
    swap(&indices[a], &indices[b]);
  }
  if (compare(indices[c], indices[b] < 0)) {
    swap(&indices[c], &indices[b]);
  }
}

void lowerMedian4(int* indices, int a, int b, int c, int d) {
  median3(indices, a, b, c);
  if (compare(indices[d], indices[b] > 0)) {
    swap(&indices[b], &indices[d]);
    if (compare(indices[b], indices[a] > 0)) {
      swap(&indices[b], &indices[a]);
    }
  }
}

void medianOf5(int* indices, int a, int b, int c, int d, int e) {
  if (compare(indices[c], indices[a]) > 0) {
    swap(&indices[a], &indices[c]);
  }
  if (compare(indices[d], indices[b]) > 0) {
    swap(&indices[b], &indices[d]);
  }
  if (compare(indices[d], indices[c]) > 0) {
    swap(&indices[c], &indices[d]);
    swap(&indices[a], &indices[b]);
  }
  if (compare(indices[e], indices[b]) > 0) {
    swap(&indices[b], &indices[e]);
  }
  if (compare(indices[e], indices[c]) > 0) {
    swap(&indices[c], &indices[e]);
    if (compare(indices[c], indices[a]) > 0) {
      swap(&indices[a], &indices[c]);
    }
  }
  else {
    if (compare(indices[c], indices[b]) > 0) {
      swap(&indices[b], &indices[c]);
    }
  }
}

void median5(int* indices, int i) {
  int* a = &indices[i];
  int* b = &indices[i + 1];
  int* c = &indices[i + 2];
  int* d = &indices[i + 3];
  int* e = &indices[i + 4];
  if (compare(*c, *a) > 0) {
    swap(a, c);
  }
  if (compare(*d, *b) > 0) {
    swap(b, d);
  }
  if (compare(*d, *c) > 0) {
    swap(c, d);
    swap(a, b);
  }
  if (compare(*e, *b) > 0) {
    swap(b, e);
  }
  if (compare(*e, *c) > 0) {
    swap(c, e);
    if (compare(*c, *a) > 0) {
      swap(a, c);
    }
  } else {
    if (compare(*c, *b) > 0) {
      swap(b, c);
    }
  }
}