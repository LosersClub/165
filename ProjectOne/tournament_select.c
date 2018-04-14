typedef struct _Node {
  struct _Node* parent;
  struct _Node* leftChild;
  struct _Node* rightChild;
  int compareTo;
  int value;
} Node;

Node* newNode(Node* left, Node* right);
void freeNode(Node* node);
int getPathIndex(Node* array, int n, int value);

tournamentSelect(int n, int k, int* out) {
  int numLeaves = n;
  int tempSize = numLeaves;
  Node* array = malloc(numLeaves * sizeof(Node));
  Node** temp = malloc(tempSize  * sizeof(Node*));
  for (int i = 0; i < numLeaves; i++) {
    array[i].value = i;
    array[i].leftChild = NULL;
    array[i].rightChild = NULL;
    temp[i] = &array[i];
  }

  // Build initial tournament tree
  while (tempSize > 1) {
    int newSize = 0;
    for (int i = 0; i < tempSize - 1; i += 2) {
      Node* parent = newNode(temp[i], temp[i+1]);
      if (compare(temp[i]->value, temp[i + 1]->value) > 0) {
        parent->value = temp[i]->value;
        parent->compareTo = temp[i + 1]->value;
      } else {
        parent->value = temp[i + 1]->value;
        parent->compareTo =  temp[i]->value;
      }
      temp[newSize++] = temp[i]->parent = temp[i + 1]->parent = parent;
    }

    if (tempSize % 2 == 1) {
      temp[newSize] = temp[tempSize - 1];
      swap(&temp[0], &temp[newSize++]);
    }
    tempSize = newSize;
  }
  int pathIndex = getPathIndex(array, numLeaves, temp[0]->value);
  Node* root = temp[0];
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

  // Free tree
  freeNode(root);
  free(array);
}

Node* newNode(Node* left, Node* right) {
  Node* out = malloc(sizeof(Node));
  out->parent = NULL;
  out->leftChild = left;
  out->rightChild = right;
  return out;
}

void freeNode(Node* node) {
  if (node->leftChild->leftChild != NULL) {
    freeNode(node->leftChild);
  }
  if (node->rightChild->rightChild != NULL) {
    freeNode(node->rightChild);
  }
  free(node);
}

int getPathIndex(Node* array, int n, int value) {
  return value;
}