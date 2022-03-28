#pragma once
#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

class Range{
    private:
        bst* head;
    public:
        Range(bst* head);

        void printOcc(bst* Node, std::string low, std::string high, int& num);

        std::string convertToCommas(int number);
};