#ifdef DEBUG
static int debug[DEBUG_SIZE];
static int compareCount = 0;
#endif

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

/*
* Binary Searches between left and right - 1 to find where to
* place value stored in right.
*/
int binarySearch(int* array, int left, int right) {
  int val = right--;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (compare(array[mid], array[val]) > 0) {
      left = mid + 1;
    }
    else {
      right = mid - 1;
    }
  }

  for (int i = val; i > left; i--) {
    swap(&array[i], &array[i - 1]);
  }
}