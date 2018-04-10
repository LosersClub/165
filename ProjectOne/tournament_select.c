#include <stdlib.h>
#include <math.h>

typedef struct _Node {
  struct _Node* parent;
  int compareTo;
  int value;
} Node;

int getPathIndex(Node* array, int n, int value);

tournamentSelect(int* indices, int n, int k) {
  int* out = malloc(k * sizeof(int));

  int numLeaves = n;
  Node* array = malloc(sizeof(Node)*numLeaves);
  int tempSize = numLeaves;
  Node** temp = malloc(tempSize * sizeof(Node*));
  for (int i = 0; i < numLeaves; i++) {
    array[i].value = indices[i];
    temp[i] = &array[i];
  }

  while (tempSize > 1) {
    int newSize = tempSize % 2 == 0 ? tempSize / 2 : tempSize / 2 + 1;
    Node** newTemp = malloc(newSize * sizeof(Node*));

    int j = 0;
    for (int i = 0; i < tempSize - 1; i += 2) {
      Node* parent = malloc(sizeof(Node));
      if (compare(temp[i]->value, temp[i + 1]->value) > 0) {
        parent->value = temp[i]->value;
        parent->compareTo = temp[i + 1]->value;
      } else {
        parent->value = temp[i + 1]->value;
        parent->compareTo =  temp[i]->value;
      }
      newTemp[j++] = temp[i]->parent = temp[i + 1]->parent = parent;
      parent->parent = NULL;
    }
    if (tempSize % 2 == 1) {
      newTemp[j] = temp[tempSize - 1];
      int index = 0; // 0 = 10495
      swap(&newTemp[index], &newTemp[j]);
      j++;
    }
    Node** kill = temp;
    temp = newTemp;
    tempSize = newSize;
    free(kill);
  }
  int pathIndex = getPathIndex(array, numLeaves, temp[0]->value);
  free(temp);

  out[0] = array[pathIndex].value;
  int outIndex = 1;
  for (int i = numLeaves; outIndex < k; i++) {
    array[pathIndex].value = i < n ? indices[i] : -1;

    Node node = array[pathIndex];
    while(node.parent != NULL) {
      node.parent->value = node.value;
      if (compare(node.parent->value, node.parent->compareTo) < 0) {
        int v = node.parent->value;
        node.parent->value = node.parent->compareTo;
        node.parent->compareTo = v;
      }
      node = *node.parent;
    }
    out[outIndex] = node.value;
    if (numLeaves < n && node.value == indices[i]) {
      binarySearch(out, 0, outIndex);
    }
    outIndex++;
    pathIndex = getPathIndex(array, numLeaves, node.value);
  }

  for (int i = 0; i < k; i++) {
    indices[i] = out[i];
  }
  free(out);
}

int getPathIndex(Node* array, int n, int value) {
  for (int i = 0; i < n; i++) {
    if (array[i].value == value) {
      return i;
    }
  }
}