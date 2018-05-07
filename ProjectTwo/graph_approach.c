// Anastasia Miles (90039862)
// Kyle Bartz (27024310)

// Project 2 Solution

// This approach utilizes the idea of a collection (ChainList)
// of Chains, which consist of Nodes. Nodes store two indices
// of the array for which we need to find a majority. A Chain
// is a linked list of Nodes, where each Chain has an identity,
// either Majority or All. An All chain consists only of Nodes
// that we know contain all the same elements, and a Majority
// chain consists of Nodes that, when queried together, returned
// Majority. The ChainList is a linked list of Chains, with
// all Majority Chains on the left and all All Chains on the right,
// separated by a special Node.
// At the start, all Chains have only 1 Node and are classified as
// Majority. By following a series of logical rules, we progressively
// eliminate Nodes and Chains, or combine Chains until either 0 or 1 
// Chains remain, and return a conclusion. By the design of the rules,
// a Majority Chain can only be size 1 or 2.
// See the project report for more information.

#include <stdlib.h>

enum Type {All, Maj};

typedef struct _Node {
  int indices[2];
  struct _Node* next;
  struct _Node* prev;
} Node;

typedef struct _Chain {
  enum Type type;
  Node* first;
  Node* last;
  int size;
  struct _Chain* prev;
  struct _Chain* next;
} Chain;

typedef struct _ChainList {
  Chain* first;
  Chain* last;
  int size;
} ChainList;

// Declared in order of definition
void initHead(ChainList* head);
void addChain(Chain* chain, ChainList* head);
void moveChain(Chain* chain, ChainList* chainList);
void deleteChain(Chain* chain, ChainList* chainList);
void deleteChainList(ChainList* head);
int query(Node* nodeOne, Node* nodeTwo);
Chain* buildChain(Node* node, enum Type type);
void combineChains(Chain* left, Chain* right, ChainList* chainList);
void addNode(Chain* chain, Node* node);
Node* removeNode(Chain* chain, Node* node, ChainList* chainList);
void deleteNode(Chain* chain, Node* node, ChainList* chainList);
Node* buildNode(int indexOne, int indexTwo);
void sortIncreasing(Chain* start, Chain* end, ChainList* chainList);
void evalRule(Chain* left, Chain* right, ChainList* chainList);
void majAllRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void majMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void allAllRule(Chain* majLeft, Chain* majRight, ChainList* chainList);

// The main function.
int execute(int n) {
  int index = -1;
  if (n < 5) {
    printf("Too few elements to determine majority.");
    return index;
  }
  // Initialize the data structures: separate the data into 2-element Nodes
  // and 1-Node Chains, type Majority
  ChainList head;
  initHead(&head);
  int i;
  for (i = 0; i < n - 1; i+=2) {
    addChain(buildChain(buildNode(i + 1, i + 2), Maj), &head);
  }
  // Separator will be a temporary Chain that separates Maj Chains from
  // All Chains.
  Chain* separator = buildChain(NULL, All);
  addChain(separator, &head);
  // While Maj Chains exist, if they are size-1 (1 node), they will be
  // pairwise-queried and resolved. If they are size-2 and an All Chain is
  // available, they will be queried and resolved against an All Chain.
  // If there are no All Chains, pairwise-query Maj Chains.
  while (separator != head.first) {
    // If only one Maj Chain remains, force a Maj-All rule.
    if (separator->prev == head.first) {
      if (separator->next != NULL) {
        evalRule(head.first, separator->next, &head);
      }
      break;
    }
    // left and right will be the Chains we pairwise-query.
    // ref will keep track of the Chain that will be the next "left".
    Chain* left = head.first;
    Chain* right = NULL;
    Chain* ref = left->next;
    // Loop through Maj Chains. If left is size 2, try to pairwise
    // with an All. Otherwise just pairwise with another Maj.
    while (ref != separator) {
      if (left->size == 2 && separator->next != NULL) {
        right = separator->next;
      } else {
        right = ref;
        ref = ref->next;
      }
      evalRule(left, right, &head);
      left = ref;
      ref = ref == separator ? ref : ref->next;
      // Sort the All Chains by increasing order, so the smallest
      // All Chains will always be put up against the Maj Chains.
      sortIncreasing(separator->next, head.last, &head);
    }
  }
  // Delete the Separator Chain as it's no longer needed.
  deleteChain(separator, &head);

  // Resolve the remaining All Chains by merging/eliminating
  // as needed.
  while (head.size > 1) {
    // Sort the All Chains so that the largest are merged first.
    sortIncreasing(head.first, head.last, &head);
    int totalSize = 0;
    Chain* temp = head.last;
    while (temp != NULL) {
      totalSize += temp->size;
      temp = temp->prev;
    }
    // We can stop early if the largest All Chain's size is greater
    // than half the total size.
    if (head.last->size > ceil(totalSize / 2)) {
      index = head.last->first->indices[0];
      break;
    }

    Chain* first = head.last;
    Chain* second = first->prev;
    Chain* next = second->prev;
    while (second != NULL) {
      evalRule(first, second, &head);
      first = next;
      second = first == NULL ? NULL : first->prev;
      next = second == NULL ? NULL : second->prev;
    }
  }
  
  // Handle if only one Chain remains and it is a Maj Chain.
  if (head.size == 1 && head.first->type == Maj) {
    Chain* maj = head.first;
    if (maj->size > 2) {
      printf("problem!\n");
    }
    Node* all = NULL;
    int refIndex = 1;
    while (all == NULL) {
      int q[4];
      for (int i = 0; i < 4; i += 2) {
        Node* temp = maj->first;
        while (temp != NULL) {
          if (temp->indices[0] == refIndex) {
            temp = maj->first;
            refIndex += 2;
          }
          else {
            temp = temp->next;
          }
        }
        q[i] = refIndex;
        q[i + 1] = refIndex + 1;
        refIndex += 2;
      }
      int queryReturn = QUERY(q);
      if (queryReturn == 4) {
        all = buildNode(q[0], q[1]);
      } else {
        Node* temp = maj->first;
        while (temp != NULL) {
          if (temp->indices[0] == refIndex) {
            temp = maj->first;
            refIndex += 2;
          }
          else {
            temp = temp->next;
          }
        }
        int hold = q[3];
        q[3] = refIndex;
        int secondQuery = QUERY(q);
        if (secondQuery == queryReturn) {
          all = buildNode(refIndex, hold);
          break;
        }
        if (secondQuery == 4) {
          all = buildNode(q[0], q[1]);
          break;
        }
        int hold2 = q[2];
        q[2] = hold;
        int thirdQuery = QUERY(q);
        if (thirdQuery == secondQuery) {
          all = buildNode(hold2, hold);
          break;
        }
        if (thirdQuery == 4) {
          all = buildNode(hold2, hold);
          break;
        }
        all = buildNode(hold2, refIndex);
      }
    }

    if (query(all, maj->first) == 2) {
      deleteNode(maj, maj->first, &head);
    } else if (maj->size == 2) {
      deleteNode(maj, maj->last, &head);
    }
  }

  //Evaluating the results:
  if (head.size == 0) {
    // Even split (no Chains left), except there was 1 element 
    // left out from the start. Return that element.
    index = (n % 2 == 1) ? n : 0;
  }
  // There was one remaining All Chain: return either index.
  if (index == -1) {
    index = head.first->first->indices[0];
  }
  deleteChainList(&head);
  return index;
}

// CHAINLIST FUNCTIONS
// Initialize the ChainList.
void initHead(ChainList* chainList) {
  chainList->first = chainList->last = NULL;
  chainList->size = 0;
}

// Adds chain to the end of the ChainList.
void addChain(Chain* chain, ChainList* chainList) {
  if (chainList->size == 0) {
    chainList->first = chain;
  } else {
    chainList->last->next = chain;
    chain->prev = chainList->last;
  }
  chainList->size += 1;
  chainList->last = chain;
}

// Changes chain's type to All and moves it to the end of chainList.
void moveChain(Chain* chain, ChainList* chainList) {
  if (chain->next != NULL) {
    chain->next->prev = chain->prev;
  }
  if (chain->prev != NULL) {
    chain->prev->next = chain->next;
  }
  if (chainList->first == chain) {
    chainList->first = chain->next;
  }
  if (chainList->last == chain) {
    chainList->last = chain->prev;
  }
  chain->type = All;
  chain->prev = chainList->last;
  chainList->last->next = chain;
  chain->next = NULL;
  chainList->last = chain;
}

// Deletes chain from the chainList, including the chain's Nodes.
void deleteChain(Chain* chain, ChainList* chainList) {
  Node* node = chain->first;
  Node* temp;
  // Delete all of chain's Nodes
  while (node != NULL) {
    temp = node->next;
    free(node);
    node = temp;
  }
  if (chain->next != NULL) {
    chain->next->prev = chain->prev;
  }
  if (chain->prev != NULL) {
    chain->prev->next = chain->next;
  }
  if (chainList->first == chain) {
    chainList->first = chain->next;
  }
  if (chainList->last == chain) {
    chainList->last = chain->prev;
  }
  free(chain);
  chainList->size -= 1;
}

// Deletes chainList and all of its Chains and Nodes.
void deleteChainList(ChainList* head) {
  Chain* chain = head->first;
  Chain* temp;
  while (chain != NULL) {
    temp = chain->next;
    deleteChain(chain, head);
    chain = temp;
  }
}

// CHAIN FUNCTIONS

// Runs a query between the two given Nodes. 
int query(Node* nodeOne, Node* nodeTwo) {
  int q[4] = { nodeOne->indices[0],
    nodeOne->indices[1],
    nodeTwo->indices[0],
    nodeTwo->indices[1]
  };
  return QUERY(q);
}

// Builds a new Chain with node as its first and last Node, and
// the given type. 
Chain* buildChain(Node* node, enum Type type) {
  Chain* chain = malloc(sizeof(Chain));
  chain->first = chain->last = node;
  chain->size = node != NULL ? 1 : 0;
  chain->type = type;
  chain->prev = chain->next = NULL;
  return chain;
}

// Combines the source Chain into the target Chain.
void combineChains(Chain* target, Chain* source, ChainList* chainList) {
  target->last->next = source->first;
  source->first->prev = target->last;
  target->last = source->last;
  target->size += source->size;
  source->first = NULL;
  deleteChain(source, chainList);
}

// Adds node to the end of chain.
void addNode(Chain* chain, Node* node) {
  if (chain->size == 0) {
    chain->first = node;
    node->prev = NULL;
  } else {
    chain->last->next = node;
    node->prev = chain->last;
  }
  chain->size += 1;
  chain->last = node;
  node->next = NULL;
}

// Removes node from chain, deleting chain if it is now empty.
// Returns the removed node so it can be placed elsewhere.
Node* removeNode(Chain* chain, Node* node, ChainList* chainList) {
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  if (chain->first == node) {
    chain->first = node->next;
  }
  if (chain->last == node) {
    chain->last = node->prev;
  }
  chain->size -= 1;
  node->prev = NULL;
  node->next = NULL;

  if (chain->size == 0) {
    deleteChain(chain, chainList);
  }
  return node;
}

// Deletes node from chain, deleting the source Chain
// if it is now empty.
void deleteNode(Chain* chain, Node* node, ChainList* chainList) {
  if (node == NULL) {
    return;
  }
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  if (chain->first == node) {
    chain->first = node->next;
  }
  if (chain->last == node) {
    chain->last = node->prev;
  }
  chain->size -= 1;
  free(node);

  if (chain->size == 0) {
    deleteChain(chain, chainList);
  }
}

// NODE FUNCTIONS

// Builds and returns a new Node that stores the given two
// indices.
Node* buildNode(int indexOne, int indexTwo) {
  Node* node = malloc(sizeof(Node));
  node->indices[0] = indexOne;
  node->indices[1] = indexTwo;
  node->next = node->prev = NULL;
  return node;
}

// Sort the Chainst in chainList from start to end (inclusive) 
// by increasing size. It follows the Insertion Sort algorithm.
void sortIncreasing(Chain* start, Chain* end, ChainList* chainList) {
  // Safety checks: chainList is valid and has more than one Chain
  if (start == NULL || end == NULL || start->next == end) {
    return;
  }
  // next will be the next Chain that will be insertion-sorted
  Chain* next = start->next;
  // Continue until we hit the end of chainList.
  // left and right will end up being the Chains being swapped.
  // temp is used for repairing pointers that were broken by swaps.
  while (next != end->next) {
    Chain* temp = next;
    next = next->next;
    // Fix our start and end pointers if they participated in
    // a swap
    while (temp != start && temp->size < temp->prev->size) {
      if (temp->prev == start) {
        start = temp;
      }
      if (temp == end) {
        end = temp->prev;
      }
      Chain* left = temp->prev;
      Chain* right = temp;
      // Used for the swap
      Chain* nextOfRight = right->next;
      if (left->prev != NULL) {
        left->prev->next = right;
      }
      if (right->next != NULL) {
        right->next->prev = left;
      }
      // Perform the swap of left and right
      right->next = left;
      right->prev = left->prev;
      left->next = nextOfRight;
      left->prev = right;
      // Repair chainList if we have moved its first or last Chain
      if (chainList->first == left) {
        chainList->first = right;
      }
      if (chainList->last == right) {
        chainList->last = left;
      }
    }
  }
}

// Calls the proper rule based on the identities of the Chains.
void evalRule(Chain* left, Chain* right, ChainList* chainList) {
  if (left->type == Maj && right->type == All) {
    majAllRule(left, right, chainList);
  } else if (left->type == Maj && right->type == Maj) {
    majMajRule(left, right, chainList);
  } else if (left->type == All && right->type == All) {
    allAllRule(left, right, chainList);
  } else {
    printf("This was an unexpected combination!");
  }
}

// The left Chain is Majority, and the right Chain is All.
void majAllRule(Chain* maj, Chain* all, ChainList* chainList) {
  // Run a query on the last Node in maj and the first node in all
  int queryReturn = query(maj->last, all->first);
  // If query = majority, maj's Node must have been 01/10, so delete it.
  if (queryReturn == 2) {
    int majSize = maj->size;
    deleteNode(maj, maj->last, chainList);
    // If maj has another node, it must be a 00 or 11, so move it to
    // the "All section" of chainList.
    if (majSize > 1) {
      moveChain(maj, chainList);
    }
  }
  // If query = all, maj's Node must have been the same as the all's,
  // add maj's Node to the all Chain. If maj had another Node, it 
  // must be 01/10, so delete it.
  else if (queryReturn == 4) {
    deleteNode(maj, maj->last->prev, chainList);
    addNode(all, removeNode(maj, maj->last, chainList));
  }
  // If query = even, maj's Node was either 00 or 11, the opposite
  // of whatever the all Chain carried. If maj had another Node, it
  // must be 01 or 10. So, delete all of maj, and the first Node in all.
  else if (queryReturn == 0) {
    deleteNode(maj, maj->last->prev, chainList);
    deleteNode(maj, maj->last, chainList);
    deleteNode(all, all->first, chainList);
  }
  else {
    printf("majAllRule: unexpected error on query!");
  }
}

// The left Chain is Majority, and the right Chain is Majority.
void majMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList) {
  // Run a query on the last Node in majLeft and the first node in majRight
  int queryReturn = query(majLeft->last, majRight->first);
  // If query = majority, combine the Chains (into majLeft).
  // Run implicit edge rules the Chain of size at most 2
  if (queryReturn == 2) {
    combineChains(majLeft, majRight, chainList);
    // Implicit edge handling
    // If the Chain's size is already 2, we're finished
    if (majLeft->size == 2) {
      return;
    }
    // First implicit edge, run only when majLeft->size >= 3.
    // Query the first and third Nodes in majLeft.
    // Majority is not a possible outcome for the query.
    // If query = all, Nodes 2 and 4 (if majLeft->sie == 4)
    // must be 01 or 10, so discard them, and move the now-All
    // Chain to the end of chainList
    if (query(majLeft->first, majLeft->first->next->next) == 4) {
      if (majLeft->size == 4) {
        deleteNode(majLeft, majLeft->last, chainList);
      }
      deleteNode(majLeft, majLeft->first->next, chainList);
      moveChain(majLeft, chainList);
      return;
    }
    // If query = even, discard Nodes 1 and 3.
    deleteNode(majLeft, majLeft->first, chainList);
    deleteNode(majLeft, majLeft->first->next, chainList);
    // If majLeft still has 2 Nodes, query them together. If query = all,
    // move the Chain to the All section of chainList. If query = even, 
    // delete the Chain. Majority is not a possible outcome.
    // If it only had 1 Node, leave it as a Majority Chain.
    if (majLeft->size == 2) {
      if (query(majLeft->first, majLeft->last) == 4) {
        moveChain(majLeft, chainList);
      } else {
        deleteChain(majLeft, chainList);
      }
    }
  }
  else if (queryReturn == 4) {
    deleteNode(majLeft, majLeft->last->prev, chainList);
    deleteNode(majRight, majRight->first->next, chainList);
    // Make the majLeft->last and majRight->first into a single all chain
    int leftSize = majLeft->size;
    int rightSize = majRight->size;
    Chain* temp = buildChain(removeNode(majLeft, majLeft->last, chainList), All);
    addChain(temp, chainList);
    addNode(temp, removeNode(majRight, majRight->first, chainList));
    // Break up remaining majLeft and majRight chains
  }

  // If the query on the last Node in majLeft and the first node in majRight = even,
  // just delete the two Nodes that were queried. No conclusion can be made about
  // the rest
  else if (queryReturn == 0) {
    int originalLeftSize = majLeft->size;
    int originalRightSize = majRight->size;
    deleteNode(majLeft, majLeft->last, chainList);
    deleteNode(majRight, majRight->first, chainList);
   /* if (originalLeftSize == 1 || originalRightSize == 1) {
      return;
    }
    if (query(majLeft->first, majRight->first) == 4) {
      combineChains(majLeft, majRight, chainList);
      moveChain(majLeft, chainList);
    } else {
      deleteChain(majLeft, chainList);
      deleteChain(majRight, chainList);
    }*/
  }
  else {
    printf("majMajRule: unexpected error on query!");
  }
}

// The left Chain is All, and the right Chain is All.
void allAllRule(Chain* allLeft, Chain* allRight, ChainList* chainList) {
  // Query the last Node of allLeft and the first Node of allRight
  int queryReturn = query(allLeft->last, allRight->first);
  // Majority should not be a possible outcome for the query.
  // If query = all, just combine the Chains.
  if (queryReturn == 4) {
    combineChains(allLeft, allRight, chainList);
  }
  // If query = even, delete an equal amount of Nodes from both Chains.
  // The number of Nodes deleted = the size of the smaller Chain.
  else if (queryReturn == 0) {
    int numNodesToDelete = allLeft->size <= allRight->size ? 
      allLeft->size : allRight->size;
    int i;
    for (i = 0; i < numNodesToDelete; i++) {
      deleteNode(allLeft, allLeft->first, chainList);
      deleteNode(allRight, allRight->first, chainList);
    }
  }
  else {
    printf("allAllRule: unexpected error on query!");
  }
}