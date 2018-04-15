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
// Build the initial tournament tree
Node* buildTournamentTree(int numLeaves, Node** root, int* pathIndex);

tournamentSelect(int n, int k, int* out) {
  int pathIndex;
  int numLeaves = n;
  Node* root = NULL;
  Node* leaves = buildTournamentTree(numLeaves, &root, &pathIndex);

  // Add first winner to output
  out[0] = leaves[pathIndex].value;
  int outIndex = 1;

  // Get the remaing k - 1 largest values.
  for (int i = numLeaves; outIndex < k; i++) {
    leaves[pathIndex].value = i < n ? i : -1;

    Node node = leaves[pathIndex];
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
  free(leaves);
}

Node* buildTournamentTree(int numLeaves, Node** root, int* pathIndex) {
  int currentLevelSize = numLeaves;
  Node* leaves = malloc(numLeaves * sizeof(Node));
  Node** temp = malloc(currentLevelSize * sizeof(Node*));
  for (int i = 0; i < numLeaves; i++) {
    leaves[i].value = i;
    leaves[i].leftChild = NULL;
    leaves[i].rightChild = NULL;
    temp[i] = &leaves[i];
  }

  // Build initial tournament tree
  while (currentLevelSize > 1) {
    int newSize = 0;
    for (int i = 0; i < currentLevelSize - 1; i += 2) {
      Node* parent = createNewNode(temp[i], temp[i + 1]);
      if (compare(temp[i]->value, temp[i + 1]->value) > 0) {
        parent->value = temp[i]->value;
        parent->comparedTo = temp[i + 1]->value;
      }
      else {
        parent->value = temp[i + 1]->value;
        parent->comparedTo = temp[i]->value;
      }
      temp[newSize++] = temp[i]->parent = temp[i + 1]->parent = parent;
    }

    if (currentLevelSize % 2 == 1) {
      temp[newSize] = temp[currentLevelSize - 1];
      swap(&temp[0], &temp[newSize++]);
    }
    currentLevelSize = newSize;
  }
  *pathIndex = temp[0]->value;
  *root = temp[0];
  free(temp);
  return leaves;
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