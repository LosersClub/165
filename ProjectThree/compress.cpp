#include <iostream>
#include <string>

#include "suffix_array.h"
// TODO: The filename should be a command-line arg, as well as the parameters
int main() {
  SuffixArray sa("testtest");
  sa.print();
  std::pair<int, int> result = sa.getMatch("tttt");

  std::cout << "Match - len: " << result.first << ", "
    << " offset: " << result.second << std::endl;
  system("PAUSE");
}