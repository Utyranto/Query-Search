#include "bst.h"

BST::BST(bst* firstNode){
    this->head = firstNode;
}

void BST::insert(bst* Node){
    bst* y = NULL;
    bst* x = head;
    while(x != NULL){
        y = x;
        if(strcmp(Node->soc.occupation, x->soc.occupation) < 0){
            x = x->left;
        } else {
            x = x->right;
        }
    }

    if (y == NULL){
        head = Node;
    } else if (strcmp(Node->soc.occupation, y->soc.occupation) < 0){
        y->left = Node;
    } else {
        y->right = Node;
    }
}

bst* BST::getHead(){
    return head;
}