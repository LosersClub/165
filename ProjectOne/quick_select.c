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

int expandPartition(int* indices, int a, int p, int b, int left, int right) {
  int i = left;
  int j = right;
  while (1) {
    while (i < a && compare(indices[i], indices[p]) > 0) {
      i++;
    }
    while (j > b && compare(indices[j], indices[p]) < 0) {
      j--;
    }
    if (i == a || j == b) {
      break;
    }
    swap(&indices[i], &indices[j]);
    i++;
    j--;
  }
  if (i != a) {
    return hPartition(indices, i, a-1, p);
  }
  if (j != b) {
    return hPartition(indices, b, j, p);
  }
  return p;
}

int random(int* indices, int left, int right, int k) {
  return hPartition(indices, left, right, randomPivot(left, right));
}

int median(int* indices, int left, int right, int k) {
  return hPartition(indices, left, right, left + (right - left + 1) / 2);
}

int baseline(int* indices, int left, int right, int k) {
  int n = right - left + 1;
  if (n < 5) {
    return hPartition(indices, left, right, left + n / 2);
  }
  int i = left;
  int j = 0;
  while (i + 4 < n) {
    median5(indices, i);
    swap(&indices[i + 2], &indices[left + j]);
    i += 5;
    j++;
  }

  quickSelect(baseline, indices, left, j, left + j / 2);
  return hPartition(indices, left, right, left + j / 2);
}

int baselineImproved(int* indices, int left, int right, int k) {
  int n = right - left + 1;
  if (n < 5) {
    return hPartition(indices, left, right, left + n / 2);
  }
  int f = n / 5;
  int i = 0;
  for (int j = left + 2 * f; j < left + 3 * f; j++) {
    medianOf5(indices, i, i + 1, j, 3*f + i, 3*f + i + 1);
    i += 2;
  }
  int newLeft = left + 2 * f;
  quickSelect(baselineImproved, indices, newLeft, left + 3 * f - 1, left + f / 2);
  return expandPartition(indices, newLeft, newLeft + f / 2, left + 3 * f - 1, left, right);
}


int repeatedStepFarLeft(int* indices, int left, int right, int k) {
  int n = right - left + 1;
  if (n < 12) {
    return hPartition(indices, left, right, left + n / 2);
  }
  int f = n / 4;
  for (int i = left + f; i < left + 2 * f; i++) {
    lowerMedian4(indices, i - f, i, i + f, i + 2 * f);
  }
  int f2 = f / 3;
  for (int i = left + f; i < left + f + f2; i++) {
    if (compare(indices[i + f2], indices[i]) > 0) {
      swap(&indices[i + f2], &indices[i]);
    }
    if (compare(indices[i + 2 * f2], indices[i]) > 0) {
      swap(&indices[i + 2 * f2], &indices[i]);
    }
  }
  int newLeft = left + f;
  quickSelect(repeatedStepFarLeft, indices, newLeft, newLeft + f2, newLeft + (k - left)*f2 / n);
  return expandPartition(indices, newLeft, newLeft + (k - left)*f2/n, newLeft+f2-1, left, right);
}