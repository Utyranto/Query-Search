#pragma once
#include <string>
#include <iostream>

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

        ~OccHeap(); //destructor
};