#include "range.h"


Range::Range(bst* headPass){
    this->head = headPass;
}

void Range::printOcc(bst* Node, std::string low, std::string high, int& num){
    if (Node == NULL){
        return;
    } else {
        printOcc(Node->left, low, high, num);

        if(Node->soc.occupation >= low && Node->soc.occupation <= high){
            std::cout << "\t" << Node->soc.occupation << ": YRFT: " <<  convertToCommas(Node->soc.total) << ", Female: " << convertToCommas(Node->soc.female) << ", Male: " << convertToCommas(Node->soc.male) << std::endl;
            num = num+1;
        }

       printOcc(Node->right, low, high, num);
    }
}

std::string Range::convertToCommas(int number) {
  std::string numberString = std::to_string(number);
  for (int i = numberString.length() - 3; i > 0; i -= 3) {
      numberString.insert(i, ",");
  }
  return numberString;
}