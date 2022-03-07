#include "heap.h"
#include <string>
#include <stdlib.h>
using namespace std;

OccHeap::OccHeap(SOC* heapArr, int size, string type){
    this->heap = heapArr;
    this->heapSize = size;
    this->category = type;
}

OccHeap::MaxHeapify(int i){

}