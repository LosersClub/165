#include <stdlib.h>

// need to typedef
enum Type {All, Maj, Unknown};

typedef struct _Node {
  int indices[2];
  Node* next;
  Node* prev;
} Node;

typedef struct _Chain {
  enum Type type;
  Node* first;
  Node* last;
  int size;
  Chain* prev;
  Chain* next;
} Chain;

typedef struct _ChainList {
  Chain* first;
  Chain* last;
  int size;
} ChainList;

Node* buildNode(int indexOne, int indexTwo);
Node* removeNode(Chain* chain, Node* node);
void deleteNode(Chain* chain, Node* node);
Chain* buildChain(Chain* chain, Node* node, enum Type type);
void deleteChain(Chain* chain, ChainList* chainList);
void combineChains(Chain* left, Chain* right, ChainList* chainList);
int dropEveryOtherBackward(Chain* chain, Node* node);
int dropEveryOtherForward(Chain* chain, Node* node);
void breakChain(Chain* chain, ChainList* chainList);
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
int main(int n) {
  ChainList head;
  Node* tempNode;
  Chain* tempChain;
  if (n < 1) {
    printf("Size is danged!");
    return -1;
  }
  else if (n == 1) {
    return 1;
  }
  initHead(&head);
  int i;
  for (i = 0; i < n - 1; i+=2) {
    tempNode = buildNode(i + 1, i + 2);
    buildChain(tempChain, tempNode, Unknown);
    addChain(&head, tempChain);
  }
  // TODO: Handle case of odd number of nodes.

  //LOOP THROUGH ALL ADJACENT PAIRS OF CHAINS IN THE CHAINLIST, AND CALL
  // EVALRULE ON THEM. CONTINUE UNTIL THERE IS < 1 CHAIN IN THE CHAINLIST

  while (head.size > 1) {
    Chain* first  = head.first;
    Chain* second = first->next;
    while (first != head.last || second != NULL) {
      evalRule(first, second, &head);
      if (first->next != second) {
        // If a chain was deleted we only need to go to the next
        first = first->next;
      } else {
        // No chain was deleted?
        first = second->next;
      }
      // Need to handle if both chains were deleted?
      second = first->next;
    }
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
  head->last->next = chain;
  head->size += chain->size;
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

void deleteNode(Chain* chain, Node* node) {
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  //it's the last
  else {
    node->prev->next = NULL;
    chain->last = node->prev;
  }
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  //it's the first
  else {
    node->next->prev = NULL;
    chain->first = node->next;
  }
  chain->size -= 1;
  free(node);
}

Node* removeNode(Chain* chain, Node* node) {
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
  //it's the last
  else {
    node->prev->next = NULL;
    chain->last = node->prev;
  }
  if (node->prev != NULL) {
    node->prev->next = node->next;
  }
  //it's the first
  else {
    node->next->prev = NULL;
    chain->first = node->next;
  }
  node->prev = NULL;
  node->next = NULL;
  chain->size -= 1;
  return node;
}

Chain* buildChain(Chain* chain, Node* node, enum Type type) {
  Chain* out = malloc(sizeof(Chain));
  chain->first = node;
  chain->last = node;
  chain->size = 1;
  chain->type = type;
  chain->next = NULL;
  chain->prev = NULL;
  return out;
}

void deleteChain(Chain* chain, ChainList* chainList) {
  Node* node = chain->first;
  while (node != NULL) {
    node = node->next;
    deteleNode(node->prev);
  }
  if (chain->next != NULL) {
    chain->next->prev = chain->prev;
  }
  //it's the last
  else {
    chain->prev->next = NULL;
    chainList->last = chain->prev;
  }
  if (chain->prev != NULL) {
    chain->prev->next = chain->next;
  }
  //it's the first
  else {
    node->next->prev = NULL;
    chainList->first = chain->next;
  }
  free(chain);
  chainList->size -= 1;
}


void combineChains(Chain* left, Chain* right, ChainList* chainList) {
  left->last->next = right->first;
  left->last = right->last;
  left->size += right->size;
  deleteChain(right, chainList);
}

//inclusive
int dropEveryOtherBackward(Chain* chain, Node* node) {
  int numDropped = 0;
  Node* temp = node->prev;
  while (temp != NULL) {
    removeNode(chain, temp->next);
    numDropped++;
    if (temp->prev == NULL) {
      return numDropped;
    }
    if (temp->prev->prev == NULL) {
      removeNode(chain, temp->prev);
      return ++numDropped;
    }
    temp = temp->prev->prev;
  }
  return numDropped;
}

//inclusive
int dropEveryOtherForward(Chain* chain, Node* node) {
  int numDropped = 0;
  Node* temp = node->next;
  while (temp != NULL) {
    removeNode(chain, temp->prev);
    numDropped++;
    if (temp->next == NULL) {
      return numDropped;
    }
    if (temp->next->next == NULL) {
      removeNode(chain, temp->next);
      return ++numDropped;
    }
    temp = temp->next->next;
  }
  return numDropped;
}

void breakChain(Chain* chain, ChainList* chainList) {
  Node* temp = chain->first;
  while (temp != NULL) {
    Chain* newChain;
    //NEED TO ADD TO FULL SET OF CHAINS
    buildChain(newChain, temp, All);
    addChain(newChain, chainList);
    temp = temp->next;
  }
  deleteChain(chain, chainList);
}


void addNode(Chain* chain, Node* node) {
  node->prev = chain->last;
  chain->last->next = node;
  node->next = NULL;
  chain->size += 1;
}

int query(Node* nodeOne, Node* nodeTwo) {
  int q[4] = { nodeOne->indices[0],
               nodeOne->indices[1],
               nodeTwo->indices[0],
               nodeTwo->indices[1]
  };
  return QCOUNT(q);
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
    combineChains(unkLeft, unkRight, chainList);
    unkLeft->type = All;
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
    dropEveryOtherBackward(maj, maj->last);
    breakUpChain(maj, chainList);
  }
  else if (queryReturn == 4) {
    dropEveryOtherBackward(maj, maj->last->prev);
    addNode(all, removeNode(maj, maj->last));
    breakUpChain(maj, chainList);
  }
  else if (queryReturn == 0) {
    dropEveryOtherBackward(maj, maj->last->prev);
    //remove maj->last and all->first
    removeNode(maj, maj->last);
    removeNode(maj, all->first);
    breakUpChain(maj, chainList);

  }
  else {
    printf("majAllRule: unexpected error on query!");
  }
}

void allMajRule(Chain* all, Chain* maj, ChainList* chainList) {
  int queryReturn = query(all->last, maj->first);
  if (queryReturn == 2) {
    dropEveryOtherForward(maj, maj->first);
    breakUpChain(maj, chainList);
  }
  else if (queryReturn == 4) {
    dropEveryOtherForward(maj, maj->first->next);
    addNode(all, remvoeNode(maj, maj->first));
    breakUpChain(maj, chainList);
  }
  else if (queryReturn == 0) {
    dropEveryOtherForward(maj, maj->first->prev);
    //remove all->last and maj->first
    removeNode(all, all->last);
    removeNode(maj, maj->first);
    breakUpChain(maj, chainList);
  }
  else {
    printf("allMajRule: unexpected error on query!");
  }
}

void majMajRule(Chain* majLeft, Chain* majRight, ChainList* chainList) {
  int queryReturn = query(majLeft->last, majRight->first);
  if (queryReturn == 2) {
    combineChains(majLeft, majRight, chainList);
  }
  else if (queryReturn == 4) {
    dropEveryOtherBackward(majLeft, majLeft->last->prev);
    dropEveryOtherForward(majRight, majRight->first->next);
    // Make the majLeft->last and majRight->first into a single all chain
    Chain* temp;
    buildChain(temp, removeNode(majLeft, majLeft->last), All);
    addChain(temp, chainList);
    addNode(temp, removeNode(majRight, majRight->first));
    // Break up remaining majLeft and majRight chains
    breakUpChain(majLeft, chainList);
    breakUpChain(majRight, chainList);
  }
  else if (queryReturn == 0) {
    removeNode(majLeft, majLeft->last);
    removeNode(majRight, majRight->first);
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
        removeNode(allRight, allRight->first);
      }
      deleteChain(allLeft, chainList);
    }
    else if (allRight->size < allLeft->size) {
      int i;
      for (i = 0; i < allRight->size; i++) {
        removeNode(allLeft, allLeft->first);
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