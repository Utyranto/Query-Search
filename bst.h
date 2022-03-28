#pragma once
#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

class BST{
    private:
        bst* head;
    public:
        BST(bst* firstNode);

        void insert(bst* Node);

        bst* getHead();
};