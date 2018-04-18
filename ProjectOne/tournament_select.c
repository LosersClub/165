// Anastasia Miles (90039862) 
// Kyle Bartz (27024310)

// The tournamentSelect method builds a tournament tree to find
// the k largest elements of an input array (in this case, it
// works with indices instead of the elements directly). All 
// elements are stored in the leaves, and internal nodes store
// the maximum of their children. After the tree is built, the
// value stored at the root is the max element. We output the 
// root's value. We then follow the path through the tree from
// the max element's leaf to the root, replacing the max element
// with -1 along the way to avoid unnecessary comparisons. At the
// same time, we use the comparisons along this path to find the 
// second largest element. The second largest element becomes the new
// root, it is output, and its path is followed to get the third. 
// This process repeats until all k values are output. 
// See the project report for more details.

// A Node struct for nodes in the tree.
typedef struct _Node {
  struct _Node* parent;
  struct _Node* leftChild;
  struct _Node* rightChild;
  int comparedTo;
  // value stores an index in the array we are acting on.
  // This will be the same as the index of the Node in the
  // leaves array below.
  int value; 
} Node;

Node* buildTournamentTree(int numLeaves, Node** root);
Node* createNewNode(Node* left, Node* right);
void freeInternalNodes(Node* node);

// The main selection algorithm.
void tournamentSelect(int n, int k, int* out) {
  // Begin by building the tree, storing the root.
  int numLeaves = n;
  Node* root = NULL;
  Node* leaves = buildTournamentTree(numLeaves, &root);

  // winningIndex is the index of the leaf storing the max element.
  int winningIndex = root->value;

  // Add the first winning (max) element to output.
  out[0] = leaves[winningIndex].value;

  // Get the remaing k - 1 largest values.
  int outIndex;
  for (outIndex = 1; outIndex < k; outIndex++) {
    // Replace the winningIndex's value with -1, so that in the future
    // we don't waste comparisons. We already knew this was the max and
    // don't need to worry about it anymore.
    Node* node = &leaves[winningIndex];
    node->value = -1;

    // Follow the path of the max element up the tree.
    for (; node->parent != NULL; node = node->parent) {
      // Set the parent's value as the maximum of the current node's
      // value and the other child's value (stored as comparedTo). 
      // Store the lesser into comparedTo.
      node->parent->value = node->value;
      if (compare(node->parent->value, node->parent->comparedTo) < 0) {
        swap(&node->parent->value, &node->parent->comparedTo);
      }
    }
    // Update the output and the winningIndex.
    out[outIndex] = node->value;
    winningIndex = node->value;
  }

  // Free the tree.
  freeInternalNodes(root);
  free(leaves);
}



// Builds the initial tournament tree.
Node* buildTournamentTree(int numLeaves, Node** root) {
  // Create the array of leaves.
  // helperArray will be used to build the new levels of the tree.
  int currentLevelSize = numLeaves;
  Node* leaves = malloc(numLeaves * sizeof(Node));
  Node** helperArray = malloc(currentLevelSize * sizeof(Node*));
  int i;
  for (i = 0; i < numLeaves; i++) {
    leaves[i].value = i;
    leaves[i].leftChild = NULL;
    leaves[i].rightChild = NULL;
    helperArray[i] = &leaves[i];
  }

  // Repeat until we hit the root.
  while (currentLevelSize > 1) {
    int newLevelSize = 0;
    // For each pair of nodes in the current level, create and configure
    // a parent node.
    for (i = 0; i < currentLevelSize - 1; i += 2) {
      Node* parent = createNewNode(helperArray[i], helperArray[i + 1]);
      if (compare(helperArray[i]->value, helperArray[i + 1]->value) > 0) {
        parent->value = helperArray[i]->value;
        parent->comparedTo = helperArray[i + 1]->value;
      }
      else {
        parent->value = helperArray[i + 1]->value;
        parent->comparedTo = helperArray[i]->value;
      }
      // Store this new parent node in helperArray, starting from the left
      // side of helperArray. In essence, helperArray is being continually
      // updated to represent the new level being built.
      helperArray[newLevelSize++] = helperArray[i]->parent = 
        helperArray[i + 1]->parent = parent;
    }
    // In the case where the current level's size is not a power of 2,
    // move up the odd node. The swap helps keep the tree moderately
    // balanced.
    if (currentLevelSize % 2 == 1) {
      helperArray[newLevelSize] = helperArray[currentLevelSize - 1];
      Node* temp = helperArray[0];
      helperArray[0] = helperArray[newLevelSize];
      helperArray[newLevelSize] = temp;
      newLevelSize += 1;
    }
    // Set the new level size to be able to loop through helperArray
    // properly (the size decreases as we move up the tree).
    currentLevelSize = newLevelSize;
  }
  // At this point, helperArray[0] is the root.
  *root = helperArray[0];
  free(helperArray);
  return leaves;
}

// Allocates and initializes a new Node struct.
Node* createNewNode(Node* left, Node* right) {
  Node* out = malloc(sizeof(Node));
  out->parent = NULL;
  out->leftChild = left;
  out->rightChild = right;
  return out;
}

// Recursively deallocates the internal nodes of the tree.
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