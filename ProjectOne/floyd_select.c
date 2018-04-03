void floydExchange(int* array, int* indices, int left, int right, int k);
void wirthExchange(int* array, int* indices, int left, int right, int k);


select(int* array, short* indices, short left, short right, short k) {

}

void floydExchange(int* array, int* indices, int left, int right, int k) {
  while (left < right) {
    int t = indices[k];
    int i = left;
    int j = right;
    swap(&indices[left], &indices[k]);
    if (compareDebug(array, indices[right], t) < 0) {
      swap(&indices[left], &indices[right]);
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
    if (indices[left] == t) {
      swap(&indices[left], &indices[j]);
    } else {
      j++;
      swap(&indices[j], &indices[right]);
    }
    if (j <= k) {
      left = j + 1;
    }
    if (k <= j) {
      right = j - 1;
    }
  }
}

void wirthExchange(int* array, int* indices, int left, int right, int k) {
  while (left < right) {
    int t = indices[k];
    int i = left;
    int j = right;
    do {
      while (compareDebug(array, indices[i], t) > 0) {
        i++;
      }
      
      while(compareDebug(array, indices[j], t) < 0) {
        j--;
      }
      if (i <= j) {
        swap(&indices[i], &indices[j]);
        i++;
        j--;
      }
    } while (i <= j);
    if (j < k) {
      left = i;
    }
    if (k < i) {
      right = j;
    }
  }
}