floydExchange(int* array, short* indices, short left, short right, short k);
wirthExchange(int* array, int* indices, int left, int right, int k);


select(int* array, short* indices, short left, short right, short k) {

}

floydExchange(int* array, short* indices, short left, short right, short k) {

}

wirthExchange(int* array, int* indices, int left, int right, int k) {
  while (left < right) {
    int i = left;
    int j = right;
    do {
      while (compareDebug(array, i, k) > 0) {
        i++;
      }
      
      while(compareDebug(array, j, k) < 0) {
        j--;
      }
      if (i <= j) {
        swap(&indices[i], &indices[j]);
        i++;
        j--;
      }
    } while (i <= j);
    if (j > k) {
      left = i;
    }
    if (k > i) {
      right = j;
    }
  }
}