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

// Builds the initial tournament tree
Node* buildTournamentTree(int numLeaves, Node** root, int* pathIndex) {
  // Create the array of leaves
  // helperArray will be used to build the new levels of the tree
  int currentLevelSize = numLeaves;
  Node* leaves = malloc(numLeaves * sizeof(Node));
  Node** helperArray = malloc(currentLevelSize * sizeof(Node*));
  for (int i = 0; i < numLeaves; i++) {
    leaves[i].value = i;
    leaves[i].leftChild = NULL;
    leaves[i].rightChild = NULL;
    helperArray[i] = &leaves[i];
  }

  // Repeat until we hit the root
  while (currentLevelSize > 1) {
    int newLevelSize = 0;
    // For each pair of nodes in the current level, create and configure
    // a parent node
    for (int i = 0; i < currentLevelSize - 1; i += 2) {
      Node* parent = createNewNode(helperArray[i], helperArray[i + 1]);
      if (compare(helperArray[i]->value, helperArray[i + 1]->value) > 0) {
        parent->value = helperArray[i]->value;
        parent->comparedTo = helperArray[i + 1]->value;
      }
      else {
        parent->value = helperArray[i + 1]->value;
        parent->comparedTo = helperArray[i]->value;
      }
      // Store this parent node in helperArray, starting from the left
      // side of helperArray. In essence, helperArray is being continually
      // updated to represent the new level being built.
      helperArray[newLevelSize++] = helperArray[i]->parent = 
        helperArray[i + 1]->parent = parent;
    }
    // In the case where the current level's size is not a power of 2,
    // move up the odd node. The swap helps keep the tree moderately
    // balanced
    if (currentLevelSize % 2 == 1) {
      helperArray[newLevelSize] = helperArray[currentLevelSize - 1];
      swap(&helperArray[0], &helperArray[newLevelSize++]);
    }
    // Set the new level size to be able to loop through helperArray
    // properly
    currentLevelSize = newLevelSize;
  }
  *pathIndex = helperArray[0]->value;
  *root = helperArray[0];
  free(helperArray);
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