// Anastasia Miles (90039862) 
// Kyle Bartz (27024310)

// A utility comparison function to assist with the
// translation between 0-indexing (our program) and
// 1-indexing (COMPARE.c). 
// Also, -1 as an input value means that a comparison
// is not necessary. Just return the other value.
// If x > y, returns 1
// If y > x, returns -1
// Otherwise, returns 0
int compare(int x, int y) {
  if (x < 0 || y < 0) {
    return y < 0 ? 1 : -1;
  }
  int flag = COMPARE(x + 1, y + 1);
  if (flag == 2) {
    return -1;
  }
  if (flag == 1) {
    return 1;
  }
  return 0;
}

// Swaps two elements
void swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}