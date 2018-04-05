int randomPivot(int left, int right);

int lPartition(int* indices, int left, int right, int pivot);
int hPartition(int* indices, int left, int right, int pivot);

/* Parition Algorithms */
int random(int* indices, int left, int right, int k);
int baseline(int* indices, int left, int right, int k);
int repeatedStep(int* indices, int left, int right, int k);

void quickSelect(int (*p)(int*, int, int, int), int* indices, int left, int right, int k) {
  while (left < right) {
    int pivot = (*p)(indices, left, right, k);

    if (k < pivot) {
      right = pivot - 1;
    } else {
      left = pivot + 1;
    }
  }
}

int repeatedStep(int* indices, int left, int right, int k) {
  int n = right - left + 1;
  if (n < 9) {
    return lPartition(indices, left, right, n / 2);
  }
  int i = 0;
  int j = 0;
  while (i + 2 < n) {
    median3(indices, i, i + 1, i + 2);
    swap(&indices[i + 1], &indices[j]);
    i += 3;
    j++;
  }

  i = j = 0;
  while (i + 2 < j) {
    median3(indices, i, i + 1, i + 2);
    swap(&indices[i + 1], indices[j]);
    i += 3;
    j++;
  }
  quickSelect(repeatedStep, indices, left, j, j/2);
  return lPartition(indices, left, right, j/2);
}

int randomPivot(int left, int right) {
  return left + dshrandom(0)*(right - left + 1);
}

int hPartition(int* indices, int left, int right, int pivot) {
  swap(&indices[left], &indices[pivot]);
  int i = left + 1;
  int j = right;

  while (1) {
    while (1) {
      if (i > j || compare(indices[left], indices[i]) >= 0) {
        break;
      }
      i++;
    }
    while (compare(indices[left], indices[j]) > 0) {
      j--;
    }
    if (i >= j) {
      break;
    }
    swap(&indices[i], &indices[j]);
    i++;
    j--;
  }
  swap(&indices[left], &indices[i - 1]);
  return i - 1;
}

int lPartition(int* indices, int left, int right, int pivot) {
  int pV = indices[pivot];
  swap(&indices[pivot], &indices[right]);

  int swapIndex = left;
  for (int i = left; i < right; i++) {
    if (compare(indices[i], pV) > 0) {
      swap(&indices[swapIndex], &indices[i]);
      swapIndex++;
    }
  }
  swap(&indices[right], &indices[swapIndex]);
  return swapIndex;
}

int random(int* indices, int left, int right, int k) {
  return hPartition(indices, left, right, randomPivot(left, right));
}

int baseline(int* indices, int left, int right, int k) {
  int n = right - left + 1;
  if (n < 3) {
    return hPartition(indices, left, right, n / 2);
  }
  int i = 0;
  int j = 0;
  while (i + 2 < n) {
    median3(indices, i, i + 1, i + 2);
    swap(&indices[i + 1], &indices[j]);
    i += 3;
    j++;
  }
  quickSelect(baseline, indices, left, j, j / 2);
  return hPartition(indices, left, right, j / 2);
}