// Anastasia Miles (90039862) 
// Kyle Bartz (27024310)

// Insert actually accurate description here.

// A Node struct for nodes in the tree.
typedef struct _Node {
  struct _Node* parent;
  struct _Node* leftChild;
  struct _Node* rightChild;
  int comparedTo;
  int value;
} Node;

// Allocates and initializes a new Node struct
Node* createNewNode(Node* left, Node* right);
// Recursively frees the internal nodes of the tree
void freeInternalNodes(Node* node);

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
      Node* parent = createNewNode(temp[i], temp[i+1]);
      if (compare(temp[i]->value, temp[i + 1]->value) > 0) {
        parent->value = temp[i]->value;
        parent->comparedTo = temp[i + 1]->value;
      } else {
        parent->value = temp[i + 1]->value;
        parent->comparedTo =  temp[i]->value;
      }
      temp[newSize++] = temp[i]->parent = temp[i + 1]->parent = parent;
    }

    if (tempSize % 2 == 1) {
      temp[newSize] = temp[tempSize - 1];
      swap(&temp[0], &temp[newSize++]);
    }
    tempSize = newSize;
  }
  int pathIndex = temp[0]->value;
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
      if (compare(node.parent->value, node.parent->comparedTo) < 0) {
        swap(&node.parent->value, &node.parent->comparedTo);
      }
    }

    out[outIndex] = node.value;
    // Only need to sort when not all values are in tree
    if (numLeaves < n && node.value == i) {
      binarySearch(out, 0, outIndex);
    }

    outIndex++;
    pathIndex = node.value;
  }

  // Free tree
  freeInternalNodes(root);
  free(array);
}

// Allocates and initializes a new Node struct
Node* createNewNode(Node* left, Node* right) {
  Node* out = malloc(sizeof(Node));
  out->parent = NULL;
  out->leftChild = left;
  out->rightChild = right;
  return out;
}

// Recursively deallocates the internal nodes of the tree
void freeInternalNodes(Node* node) {
  // Do not deallocate the bottom level (all the elements of the array)
  // as this will cause repeated free's, and because a higher-level node
  // points to values in the bottom level. This will cause issues with
  // memory.
  if (node->leftChild->leftChild != NULL) {
    freeInternalNodes(node->leftChild);
  }
  if (node->rightChild->rightChild != NULL) {
    freeInternalNodes(node->rightChild);
  }
  free(node);
}