#include <stdlib.h>
// loop through array, initial 4-groups DONE
// have a majority array and an all array DONE
// first all group is reference DONE
// pairwise compare all majorities with the reference all and keep track of one index of "other" DONE
// keep a counter for how many majorities are the same as the reference array (plus or minus one); all = +1 DONE
// if the compare array returns majority, instead of affecting counter, take the first two values and put them in a new array DONE
// then we go through third array and get rid of evens, append alls to the all array DONE
// pairwise the alls DONE
// when one or zero all's remain; if one all remains, compare that to the ref. DONE
// if they are the same, the majorities are contributing to "current"/ref
// if they are different or same and majority count is negative, need to see which is contributing more
//  the same:(#all groups) + #majority*2
// diff: (#all groups) - #majority*2

typedef struct _Node {
  int size;
  int indices[2];
} Node;

void populateArrays(int n, int* alls, int* majorities, int* allSize, int* majoritySize);
void handleMajorities(int* refAll, int* majorities, int* majorityRemainders, int* majorityRemainderSize, int* majoritySize, int* majorityCount, int* indexOfOther);
void handleMajorityRemainders(int* alls, int* allSize, int* majorityRemainders, const int* majoritySize);
Node* buildNodes(int* alls, int allSize, int* allNodesSize);
Node* handleAlls(int* alls, int allSize);

int execute(int n) {
  int* alls = malloc(sizeof(int)*n);
  int* majorities = malloc(sizeof(int)*n);
  int* majorityRemainders = malloc(sizeof(int)*n);
  int allSize, majoritySize, majorityRemainderSize, majorityCount, numSameAlls, totalCount, indexOfOther;
  allSize = majoritySize = majorityRemainderSize = majorityCount = 0;
  indexOfOther = -1;
 
  populateArrays(n, alls, majorities, &allSize, &majoritySize);

  int refAll[4] = { alls[0], alls[1], alls[2], alls[3] };
  handleMajorities(refAll, majorities, majorityRemainders, &majorityRemainderSize, &majoritySize, &majorityCount, &indexOfOther);
  handleMajorityRemainders(alls, &allSize, majorityRemainders, &majoritySize);
  Node* finalAll = handleAlls(alls, allSize);

  int queryReturn;
  if (finalAll != NULL) {
    numSameAlls = finalAll->size;
    int query[4] = { refAll[0], refAll[1], finalAll->indices[0], finalAll->indices[1] };
    queryReturn = QCOUNT(1, query);
  } else {
    numSameAlls = 0;
    queryReturn = 4;
  }


  if (queryReturn == 4) {
    totalCount = (majorityCount * 2) + numSameAlls;
  }else {
    totalCount = (majorityCount * 2) - numSameAlls;
  }

  if (totalCount > 0) {
    return refAll[0];
  } else if (totalCount < 0) {
    return indexOfOther;
  } else {
    return 0;
  }
}

Node* buildNodes(int* alls, int allSize, int* allNodesSize) {
  Node* allNodes = malloc(sizeof(Node)*(allSize / 4));
  int i, nodeCounter;
  for (i = nodeCounter = 0; i < allSize; i+=4, nodeCounter++) {
    allNodes[nodeCounter].indices[0] = alls[i];
    allNodes[nodeCounter].indices[1] = alls[i + 1];
    allNodes[nodeCounter].size = 4;
  }
  *allNodesSize = nodeCounter;
  return allNodes;
}

Node* handleAlls(int* alls, int allSize) {
  int allNodesSize = 0;
  Node* allNodes = buildNodes(alls, allSize, &allNodesSize);
  int i;
  while (allNodesSize > 1) {
    int newSize = 0;
    for (i = 0; i < allNodesSize - 1; i+=2) {
      int query[4] = { allNodes[i].indices[0], allNodes[i].indices[1], allNodes[i + 1].indices[0], allNodes[i + 1].indices[1] };
      int queryReturn = QCOUNT(1, query);
      Node* toBringUp = NULL;
      if (queryReturn == 0) {
        if (allNodes[i].size > allNodes[i + 1].size) {
          allNodes[i].size -= allNodes[i + 1].size;
          toBringUp = &allNodes[i];
        } else if (allNodes[i].size < allNodes[i + 1].size) {
          allNodes[i + 1].size -= allNodes[i].size;
          toBringUp = &allNodes[i + 1];
        }
      } else if (queryReturn == 4) {
        allNodes[i].size += allNodes[i + 1].size;
        toBringUp = &allNodes[i];
      }
      if (toBringUp != NULL) {
        allNodes[newSize++] = *toBringUp;
      }
    }
    if (newSize % 2 == 1) {
      allNodes[newSize++] = allNodes[allNodesSize - 1];
    }
    allNodesSize = newSize;
  }
  if (allNodesSize == 1) {
    return &allNodes[0];
  }
  return NULL;
}

void handleMajorityRemainders(int* alls, int* allSize, int* majorityRemainders, const int* majoritySize) {
  int query[4];
  int i, j, queryReturn;
  for (i = 0; i < *majoritySize; i+=4) {
    for (j = 0; j < 4; j++) {
      query[j] = majoritySize[i + j];
    }
    queryReturn = QCOUNT(1, query);
    if (queryReturn == 4) {
      for (j = 0; j < 4; j++) {
        alls[(*allSize)++] = query[j];
      }
    }
    // if things aren't working, check here for an else
  }
}

void handleMajorities(int* refAll, int* majorities, int* majorityRemainders, int* majorityRemainderSize, int* majoritySize, int* majorityCount, int* indexOfOther) {
  int query[4] = { refAll[0], refAll[1], refAll[2], refAll[3] };
  int i, queryReturn;
  for (i = 0; i < *majoritySize; i+=4) {
    query[3] = majorities[i + 2];
    query[4] = majorities[i + 3];
    queryReturn = QCOUNT(1, query);
    if (queryReturn == 4) {
      (*majorityCount) += 1;
    } else if (queryReturn == 0) {
      (*majorityCount) -= 1;
      *indexOfOther = query[3];
    } else {
      majorityRemainders[(*majorityRemainderSize)++] = majorities[i];
      majorityRemainders[(*majorityRemainderSize)++] = majorities[i + 1];
    }
  }
}

void populateArrays(int n, int* alls, int* majorities, int* allSize, int* majoritySize) {
  int i, j, queryReturn;
  int query[4];
  for (i = 0; i < n; i += 4) {
    // Populate query array with four adjacent one-based indices
    for (j = 0; j < 4; j++) {
      query[j] = i + j + 1;
    }
    queryReturn = QCOUNT(1, query);
    if (queryReturn == 4) {
      for (j = 0; j < 4; j++) {
        alls[(*allSize)++] = query[j];
      }
    } else if (queryReturn == 2) {
      for (j = 0; j < 4; j++) {
        majorities[(*majoritySize)++] = query[j];
      }
    }
  }
}