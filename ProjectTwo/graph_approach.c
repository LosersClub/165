#include <stdlib.h>
// need to typedef
enum Type {All, Maj, Unknown};

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

Node* buildNode(int indexOne, int indexTwo);
Node* removeNode(Chain* chain, Node* node, ChainList* chainList);
void deleteNode(Chain* chain, Node* node, ChainList* chainList);
Chain* buildChain(Node* node, enum Type type);
void deleteChain(Chain* chain, ChainList* chainList);
void combineChains(Chain* left, Chain* right, ChainList* chainList);
int dropEveryOtherBackward(Chain* chain, Node* node, ChainList* chainList);
int dropEveryOtherForward(Chain* chain, Node* node, ChainList* chainList);
void breakUpChain(Chain* chain, ChainList* chainList);
void addNode(Chain* chain, Node* node);
int query(Node* nodeOne, Node* nodeTwo);
void evalRule(Chain* left, Chain* right, ChainList* chainList);
void unkUnkRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void majAllRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void allMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void majMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void allAllRule(Chain* majLeft, Chain* majRight, ChainList* chainList);
void initHead(ChainList* head);
void deleteChainList(ChainList* head);
void addChain(ChainList* head, Chain* chain);


//NOTE: CHAINS CALLED TOGETHER MUST BE ADJACENT OR COMBINE CHAINS WILL BREAK
int execute(int n) {
  ChainList head;
  Node* tempNode;
  Chain* tempChain;
  if (n < 5) {
    printf("Too few elements to determine majority.");
    return -1;
  }
  initHead(&head);
  int i;
  for (i = 0; i < n - 1; i+=2) {
    tempNode = buildNode(i + 1, i + 2);
    tempChain = buildChain(tempNode, Unknown);
    addChain(&head, tempChain);
  }
  // TODO: Handle case of odd number of nodes.

  //LOOP THROUGH ALL ADJACENT PAIRS OF CHAINS IN THE CHAINLIST, AND CALL
  // EVALRULE ON THEM. CONTINUE UNTIL THERE IS < 1 CHAIN IN THE CHAINLIST

  //Chain* temp = head.first;
  //while (temp != NULL) {
  //  temp->type = Maj;
  //  temp = temp->next;
  //}

  //while (head.size > 1) {
  //  Chain* left = head.first;
  //  Chain* right = NULL;
  //  Chain* ref = head.first->next;
  //  while (right == NULL) {
  //    if (ref == NULL) {
  //      // Could not find desired case
  //      right = left->next;
  //      continue;
  //    }
  //    if (left->type != Maj || ref->type == All) {
  //      right = ref;
  //    }
  //    ref = ref->next;
  //  }
  //  evalRule(left, right, &head);
  //}

  //while (head.size > 1) {
  //  Chain refLastChain;
  //  refLastChain.first = NULL;
  //  refLastChain.prev = NULL;
  //  refLastChain.next = NULL;
  //  addChain(&head, &refLastChain);
  //  Chain* left = head.first;
  //  Chain* right = NULL;
  //  Chain* ref = head.first->next;
  //  while (ref != &refLastChain) {
  //    if (left->type != Maj) {
  //      right = ref;
  //    }

  //    Chain* temp = ref;
  //    while (temp != &refLastChain && right == NULL) {
  //      if (temp->type == All) {
  //        right = temp;
  //      }
  //      temp = temp->next;
  //    }
  //    if (right == NULL) {
  //      right = ref;
  //    }
  //    ref = ref->next;
  //    if (left == &refLastChain || right == &refLastChain) {
  //      printf("Fuck this\n");
  //    }
  //    evalRule(left, right, &head);
  //    right = NULL;
  //    left = ref;
  //    if (ref != &refLastChain) {
  //      ref = ref->next;
  //    }
  //  }
  //  deleteChain(&refLastChain, &head);
  //  //if (head.size > 1 && head.last->type == Unknown) {
  //  //  head.last->type = Maj;
  //  //}
  //}

  //while (head.size > 1) {
  //  Chain* left = head.first;
  //  Chain* right = NULL;
  //  Chain* ref = head.first->next;
  //  while (ref != NULL) {
  //    if (right != NULL) {
  //      evalRule(left, right, &head);
  //      left = ref;
  //      right = NULL;
  //      ref = ref->next;
  //      continue;
  //    }

  //    if (left->type != Maj || ref->type == All) {
  //      right = ref;
  //    }
  //    ref = ref->next;
  //  }
  //  if (left != NULL && head.last != left) {
  //    // must do Maj vs Maj
  //    evalRule(left, left->next, &head);
  //  }
  //  if (head.size > 1 && head.last->type == Unknown) {
  //    head.last->type = Maj;
  //  }
  //}

  while (head.size > 1) {
    Chain* first  = head.first;
    Chain* second = first->next;
    Chain* next = second->next;
    while (second != NULL) {
      
      evalRule(first, second, &head);

      first = next;
      second = first == NULL ? NULL : first->next;
      next = second == NULL ? NULL : second->next;
    }
    if (head.size > 1 && head.last->type == Unknown) {
      head.last->type = Maj;
    }
  }

  if (head.size == 1 && head.first->type == Maj) {
    printf("Majority at Top\n");
  }

  //Evaluating the results:
  if (head.size == 0) {
    //even split, except there was 1 element left out from the start
    if (n % 2 == 1) {
      return n;
    }
    //even split
    return 0;
  }
  //else, there was one remaining chain
  return head.first->first->indices[0];
  deleteChainList(&head);
}

void initHead(ChainList* head) {
  head->first = NULL;
  head->last = NULL;
  head->size = 0;
}

void deleteChainList(ChainList* head) {
  Chain* temp = head->first;
  while (temp != NULL) {
    temp = temp->next;
    deleteChain(temp->prev, head);
  }

}

void addChain(ChainList* head, Chain* chain) {
  if (head->size == 0) {
    head->first = chain;
  }
  else {
    head->last->next = chain;
    chain->prev = head->last;
  }
  head->size += 1;
  head->last = chain;
}

Node* buildNode(int indexOne, int indexTwo) {
  Node* node = malloc(sizeof(Node));
  node->indices[0] = indexOne;
  node->indices[1] = indexTwo;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void deleteNode(Chain* chain, Node* node, ChainList* chainList) {
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
  //free(node);

  if (chain->size == 0) {
    deleteChain(chain, chainList);
  }
}

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

Chain* buildChain(Node* node, enum Type type) {
  Chain* chain = malloc(sizeof(Chain));
  chain->first = node;
  chain->last = node;
  chain->size = 1;
  chain->type = type;
  chain->next = NULL;
  chain->prev = NULL;
  return chain;
}

void deleteChain(Chain* chain, ChainList* chainList) {
  Node* node = chain->first;
  Node* temp;
  while (node != NULL) {
    temp = node->next;
    //free(node);
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
  //free(chain);
  chainList->size -= 1;
}


void combineChains(Chain* left, Chain* right, ChainList* chainList) {
  left->last->next = right->first;
  right->first->prev = left->last;
  left->last = right->last;
  left->size += right->size;
  right->first = NULL;
  deleteChain(right, chainList);
}

////inclusive
//int dropEveryOtherBackward(Chain* chain, Node* node, ChainList* chainList) {
//  int numDropped = 0;
//  if (node == NULL) {
//    return 0;
//  }
//  Node* temp = node->prev;
//  while (temp != NULL) {
//    deleteNode(chain, temp->next, chainList);
//    numDropped++;
//    if (temp->prev == NULL) {
//      return numDropped;
//    }
//    if (temp->prev->prev == NULL) {
//      deleteNode(chain, temp->prev, chainList);
//      return ++numDropped;
//    }
//    temp = temp->prev->prev;
//  }
//  return numDropped;
//}

int dropEveryOtherBackward(Chain* chain, Node* node, ChainList* chainList) {
  int numDropped = 0;
  while (node != NULL) {
    Node* temp = node->prev;
    if (temp != NULL) {
      temp = temp->prev;
    }
    deleteNode(chain, node, chainList);
    numDropped++;
    node = temp;
  }
  return numDropped;
}

int dropEveryOtherForward(Chain* chain, Node* node, ChainList* chainList) {
  int numDropped = 0;
  while (node != NULL) {
    Node* temp = node->next;
    if (temp != NULL) {
      temp = temp->next;
    }
    deleteNode(chain, node, chainList);
    numDropped++;
    node = temp;
  }
  return numDropped;
}

//inclusive
//int dropEveryOtherForward(Chain* chain, Node* node, ChainList* chainList) {
//  int numDropped = 0;
//  if (node == NULL) {
//    return 0;
//  }
//  Node* temp = node->next;
//  while (temp != NULL) {
//    deleteNode(chain, temp->prev, chainList);
//    numDropped++;
//    if (temp->next == NULL) {
//      return numDropped;
//    }
//    if (temp->next->next == NULL) {
//      deleteNode(chain, temp->next, chainList);
//      return ++numDropped;
//    }
//    temp = temp->next->next;
//  }
//  return numDropped;
//}

void breakUpChain(Chain* chain, ChainList* chainList) {
  Node* temp = chain->first;
  while (temp != NULL) {
    Chain* newChain = buildChain(temp, All);
    addChain(chainList,  newChain);
    temp = temp->next;
  }
  chain->first = NULL;
  deleteChain(chain, chainList);
}


void addNode(Chain* chain, Node* node) {
  if (chain->size == 0) {
    chain->first = node;
    node->prev = NULL;
  }
  else {
    chain->last->next = node;
    node->prev = chain->last;
  }
  chain->size += 1;
  chain->last = node;
  node->next = NULL;
}

int query(Node* nodeOne, Node* nodeTwo) {
  int q[4] = { nodeOne->indices[0],
               nodeOne->indices[1],
               nodeTwo->indices[0],
               nodeTwo->indices[1]
  };
  return QUERY(q);
}

void evalRule(Chain* left, Chain* right, ChainList* chainList) {

  if (left->type == Maj && right->type == All) {
    majAllRule(left, right, chainList);
  }
  else if (left->type == All && right->type == Maj) {
    allMajRule(left, right, chainList);
  }
  else if (left->type == Maj && right->type == Maj) {
    majMajRule(left, right, chainList);
  }
  else if (left->type == All && right->type == All) {
    allAllRule(left, right, chainList);
  }
  else if (left->type == Unknown && right->type == Unknown) {
    unkUnkRule(left, right, chainList);
  }
  else {
    printf("This was an unexpected combination!");
  }
}

void unkUnkRule(Chain* unkLeft, Chain* unkRight, ChainList* chainList) {
  int queryReturn = query(unkLeft->first, unkRight->first);
  if (queryReturn == 2) {
    combineChains(unkLeft, unkRight, chainList);
    unkLeft->type = Maj;
  }
  else if (queryReturn == 4) {
    Chain* temp = buildChain(removeNode(unkLeft, unkLeft->last, chainList), All);
    addChain(chainList, temp);
    addNode(temp, removeNode(unkRight, unkRight->first, chainList));

    //combineChains(unkLeft, unkRight, chainList);
    //unkLeft->type = All;
  }
  else if (queryReturn == 0) {
    deleteChain(unkLeft, chainList);
    deleteChain(unkRight, chainList);
  }
  else {
    printf("unkunkRule: unexpected error on query!");
  }
}

void majAllRule(Chain* maj, Chain* all, ChainList* chainList) {
  int queryReturn = query(maj->last, all->first);
  if (queryReturn == 2) {
    int majSize = maj->size;
    dropEveryOtherBackward(maj, maj->last, chainList);
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else if (queryReturn == 4) {
    dropEveryOtherBackward(maj, maj->last->prev, chainList);
    int majSize = maj->size;
    addNode(all, removeNode(maj, maj->last, chainList));
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else if (queryReturn == 0) {
    dropEveryOtherBackward(maj, maj->last->prev, chainList);
    //remove maj->last and all->first
    int majSize = maj->size;
    deleteNode(maj, maj->last, chainList);
    deleteNode(all, all->first, chainList);
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else {
    printf("majAllRule: unexpected error on query!");
  }
}

void allMajRule(Chain* all, Chain* maj, ChainList* chainList) {
  int queryReturn = query(all->last, maj->first);
  if (queryReturn == 2) {
    int majSize = maj->size;
    dropEveryOtherForward(maj, maj->first, chainList);
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else if (queryReturn == 4) {
    dropEveryOtherForward(maj, maj->first->next, chainList);
    int majSize = maj->size;
    addNode(all, removeNode(maj, maj->first, chainList));
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else if (queryReturn == 0) {
    dropEveryOtherForward(maj, maj->first->next, chainList);
    //remove all->last and maj->first
    deleteNode(all, all->last, chainList);
    int majSize = maj->size;
    deleteNode(maj, maj->first, chainList);
    if (majSize > 1) {
      breakUpChain(maj, chainList);
    }
  }
  else {
    printf("allMajRule: unexpected error on query!");
  }
}

void majMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList) {
  int queryReturn = query(majLeft->last, majRight->first);
  if (queryReturn == 2) {
    combineChains(majLeft, majRight, chainList);

    // Implicit edge handling (majLeft = maj chain)
    if (majLeft->size == 2) {
      // merged chain is size 2
      return;
    }
    // First implicit edge. majLeft->size >= 3
    // If All, discard everyOther from first->next, build all chain.
    if (query(majLeft->first, majLeft->first->next->next) == 4) {
      dropEveryOtherForward(majLeft, majLeft->first->next, chainList);
      majLeft->type = All;
      return;
    }
    // Otherwise drop the two compared nodes.
    deleteNode(majLeft, majLeft->first, chainList);
    deleteNode(majLeft, majLeft->first->next->next, chainList);
    // Compare the other two. If one leave as majority
    if (majLeft->size == 2) {
      if (query(majLeft->first, majLeft->last) == 4) {
        majLeft->type = All;
      } else {
        deleteChain(majLeft, chainList);
      }
    }
  }
  else if (queryReturn == 4) {
    dropEveryOtherBackward(majLeft, majLeft->last->prev, chainList);
    dropEveryOtherForward(majRight, majRight->first->next, chainList);
    // Make the majLeft->last and majRight->first into a single all chain
    int leftSize = majLeft->size;
    int rightSize = majRight->size;
    Chain* temp = buildChain(removeNode(majLeft, majLeft->last, chainList), All);
    addChain(chainList, temp);
    addNode(temp, removeNode(majRight, majRight->first, chainList));
    // Break up remaining majLeft and majRight chains
    if (leftSize > 1) {
      breakUpChain(majLeft, chainList);
    }
    if (rightSize > 1) {
      breakUpChain(majRight, chainList);
    }
  }
  else if (queryReturn == 0) {
    deleteNode(majLeft, majLeft->last, chainList);
    deleteNode(majRight, majRight->first, chainList);
  }
  else {
    printf("majMajRule: unexpected error on query!");
  }
}

void allAllRule(Chain* allLeft, Chain* allRight, ChainList* chainList) {
  int queryReturn = query(allLeft->last, allRight->first);
  if (queryReturn == 2) {
    printf("allAllRule: unexpected majority return!");
  }
  else if (queryReturn == 4) {
    combineChains(allLeft, allRight, chainList);
  }
  else if (queryReturn == 0) {
    if (allLeft->size < allRight->size) {
      int i;
      for (i = 0; i < allLeft->size; i++) {
        deleteNode(allRight, allRight->first, chainList);
      }
      deleteChain(allLeft, chainList);
    }
    else if (allRight->size < allLeft->size) {
      int i;
      for (i = 0; i < allRight->size; i++) {
        deleteNode(allLeft, allLeft->first, chainList);
      }
      deleteChain(allRight, chainList);
    }
    else {
      deleteChain(allLeft, chainList);
      deleteChain(allRight, chainList);
    }
  }
  else {
    printf("allAllRule: unexpected error on query!");
  }
}