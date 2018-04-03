void insertionSort(int* arr, int left, int right) {
  int n = right - left + 1;
  for (int i = left + 1; i < n; i++) {
    int j = i;
    while (j > 0 && compare(arr[j - 1], arr[j]) > 0) {
      swap(&arr[j], &arr[j - 1]);
      j -= 1;
    }
  }
}

void insertionSortDebug(int* data, int* arr, int left, int right) {
  int n = right - left + 1;
  for (int i = left + 1; i < n; i++) {
    int j = i;
    while (j > 0 && compareDebug(data, arr[j - 1], arr[j]) < 0) {
      swap(&arr[j], &arr[j - 1]);
      j -= 1;
    }
  }
}