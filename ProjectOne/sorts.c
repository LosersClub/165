void swap(int* a, int* b) {
  int* temp = *a;
  *a = *b;
  *b = temp;
}

void insertionSort(int* arr, int n) {
  for (int i = 1; i < n; i++) {
    int j = i;
    while (j > 0 && COMPARE(arr[j - 1], arr[j]) == 1) {
      swap(&arr[j], &arr[j - 1]);
      j -= 1;
    }
  }
}