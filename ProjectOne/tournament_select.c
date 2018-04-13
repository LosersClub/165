typedef struct _Node {
  struct _Node* parent;
  int compareTo;
  int value;
} Node;

int getPathIndex(Node* array, int n, int value);

tournamentSelect(int n, int k, int* out) {
  int numLeaves = n;
  Node* array = malloc(numLeaves * sizeof(Node));
  int tempSize = numLeaves;
  Node** temp = malloc(tempSize * sizeof(Node*));
  for (int i = 0; i < numLeaves; i++) {
    array[i].value = i;
    temp[i] = &array[i];
  }

  // Build initial tournament tree
  while (tempSize > 1) {
    int newSize = 0;
    for (int i = 0; i < tempSize - 1; i += 2) {
      Node* parent = malloc(sizeof(Node));
      if (compare(temp[i]->value, temp[i + 1]->value) > 0) {
        parent->value = temp[i]->value;
        parent->compareTo = temp[i + 1]->value;
      } else {
        parent->value = temp[i + 1]->value;
        parent->compareTo =  temp[i]->value;
      }
      temp[newSize++] = temp[i]->parent = temp[i + 1]->parent = parent;
      parent->parent = NULL;
    }

    if (tempSize % 2 == 1) {
      temp[newSize] = temp[tempSize - 1];
      swap(&temp[0], &temp[newSize++]);
    }
    tempSize = newSize;
  }
  int pathIndex = getPathIndex(array, numLeaves, temp[0]->value);
  free(temp);

  // Add first winner to output
  out[0] = array[pathIndex].value;
  int outIndex = 1;

  // Get the remaing k - 1 largest values.
  for (int i = numLeaves; outIndex < k; i++) {
    array[pathIndex].value = i < n ? i : -1;

    Node node = array[pathIndex];
    for (; node.parent != NULL; node = *node.parent) {
      node.parent->value = node.value;
      if (compare(node.parent->value, node.parent->compareTo) < 0) {
        swap(&node.parent->value, &node.parent->compareTo);
      }
    }

    out[outIndex] = node.value;
    // Only need to sort when not all values are in tree
    if (numLeaves < n && node.value == i) {
      binarySearch(out, 0, outIndex);
    }

    outIndex++;
    pathIndex = getPathIndex(array, numLeaves, node.value);
  }
}

int getPathIndex(Node* array, int n, int value) {
  for (int i = 0; i < n; i++) {
    if (array[i].value == value) {
      return i;
    }
  }
}