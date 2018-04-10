#include <stdlib.h>
#include <math.h>

typedef struct _Node {
  struct _Node* parent;
  int compareTo;
  int value;
} Node;

tournamentSelect(int* indices, int n, int k) {
  int* out = malloc(k * sizeof(int));

  int numLeaves = n - k + 2;
  Node* array = malloc(sizeof(Node)*numLeaves);
  for (int i = 0; i < numLeaves; i++) {
    array[i].value = indices[i];
  }

  int tempSize = numLeaves;
  Node** temp = malloc(tempSize * sizeof(Node*));
  for (int i = 0; i < tempSize; i++) {
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
    }
    Node** kill = temp;
    temp = newTemp;
    tempSize = newSize;
    free(kill);
  }
  int pathIndex = 0;
  for (int i = 0; i < numLeaves; i++) {
    if (array[i].value == temp[0]->value) {
      pathIndex = i;
      break;
    }
  }
  free(temp);

  out[0] = array[pathIndex].value;
  int outIndex = 1;
  for (int i = numLeaves; i <= n; i++) {
    array[pathIndex].value = i != n ? indices[i] : -1;

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

    out[outIndex++] = node.value;
    for (int i = 0; i < numLeaves; i++) {
      if (array[i].value == node.value) {
        pathIndex = i;
        break;
      }
    }
  }

  for (int i = 0; i < k; i++) {
    indices[i] = out[i];
  }
  free(out);


}