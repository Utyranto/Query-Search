#pragma once
#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif
#include <string>
#include <iostream>
#include <stdlib.h>

class OccHeap{
    private:
        std::string category;
        int heapSize;
        int length;
        SOC* heap;
    public:
        OccHeap(SOC* arr, int size, std::string type);

        void BuildMaxHeap();

        SOC HeapMaximum();

        void MaxHeapify(int i);

        int parent(int i);

        int left(int i);

        int right(int i);

        std::string convertToCommas(int num);

        ~OccHeap(); //destructor
};