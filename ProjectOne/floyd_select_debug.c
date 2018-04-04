#include <math.h>
void floydExchangeDebug(int* array, int* indices, int* left, int* right, int k);
void wirthExchangeDebug(int* array, int* indices, int* left, int* right, int k);

selectDebug(int* array, int* indices, int left, int right, int k) {
  int minSize = k;
  double splitConstant = 0.2;
  while (left < right) {
    if (right - left > minSize) {
      int n = right - left + 1;
      int i = k - left + 1;
      double z = log(n);
      double s = splitConstant * exp(2 * z / 3);
      double sd = splitConstant * sqrt(z*s*(n - s) / n) * sign(i - n / 2);
      int newLeft = max(left, k - (i*s / n) + sd);
      int newRight = min(right, k + ((n - i)*s / n) + sd);
      selectDebug(array, indices, newLeft, newRight, k);
    }
    wirthExchangeDebug(array, indices, &left, &right, k);
  }
}

void floydExchangeWhileDebug(int* array, int* indices, int* left, int* right, int k) {
  while (left < right) {
    floydExchange(array, indices, left, right, k);
  }
}

void floydExchangeDebug(int* array, int* indices, int* left, int* right, int k) {
  int t = indices[k];
  int i = *left;
  int j = *right;
  swap(&indices[*left], &indices[k]);
  if (compareDebug(array, indices[*right], t) < 0) {
    swap(&indices[*left], &indices[*right]);
  }
  while (i < j) {
    swap(&indices[i], &indices[j]);
    i++;
    j--;
    while (compareDebug(array, indices[i], t) > 0) {
      i++;
    }

    while (compareDebug(array, indices[j], t) < 0) {
      j--;
    }
  }
  if (indices[*left] == t) {
    swap(&indices[*left], &indices[j]);
  }
  else {
    j++;
    swap(&indices[j], &indices[*right]);
  }
  if (j <= k) {
    *left = j + 1;
  }
  if (k <= j) {
    *right = j - 1;
  }
}

void wirthExchangeWhileDebug(int* array, int* indices, int left, int right, int k) {
  while (left < right) {
    wirthExchange(array, indices, left, right, k);
  }
}

void wirthExchangeDebug(int* array, int* indices, int* left, int* right, int k) {
  int t = indices[k];
  int i = *left;
  int j = *right;
  do {
    while (compareDebug(array, indices[i], t) > 0) {
      i++;
    }

    while (compareDebug(array, indices[j], t) < 0) {
      j--;
    }
    if (i <= j) {
      swap(&indices[i], &indices[j]);
      i++;
      j--;
    }
  } while (i <= j);
  if (j < k) {
    *left = i;
  }
  if (k < i) {
    *right = j;
  }
}