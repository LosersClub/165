#include <stdlib.h>
#include <math.h>
typedef struct _Node {
  Node* parent;
  int* value;
} Node;

tournamentSelect(int* indices, int n, int k) {
  int numLeaves = n - k + 2;
  int treeSize = log2(numLeaves) + 1;
  Node* array = malloc(sizeof(Node)*numLeaves);
  Node* temp = malloc(sizeof(Node)*numLeaves);
  for (int i = 0; i <= numLeaves; i++) {
    array[i].value = &indices[i];
    temp[i] = array[i];
  }

  for (int i = 0; i < numLeaves - 1; i+=2) {
    Node* parent = malloc(sizeof(Node));
    if (compare(*(temp[i].value), *(temp[i + 1].value)) > 0) {
      parent->value = temp[i].value;
    }
    else {
      parent->value = temp[i+1].value;
    }
    temp[i].parent = temp[i + 1].parent = parent;
  }
  free(array);
}