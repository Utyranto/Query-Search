#include "heap.h"
using namespace std;

OccHeap::OccHeap(SOC* heapArr, int size, std::string type){
    this->heap = heapArr;
    this->length = size;
    this->category = type;
    BuildMaxHeap();
}

void OccHeap::MaxHeapify(int i){
    int l = left(i);
    int r = right(i);
    int largest;
    if(category == "total"){
        if(l < heapSize && heap[l].total > heap[i].total){
            largest = l;
        } else {
            largest = i;
        } 
        if (r < heapSize && heap[r].total > heap[largest].total) {
            largest = r;
        }
        if (largest != i){
            SOC temp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = temp;
            MaxHeapify(largest);
        }
    }

    if (category == "female"){
         if(l < heapSize && heap[l].female > heap[i].female){
            largest = l;
        } else {
            largest = i;
        } if (r < heapSize && heap[r].female > heap[largest].female) {
            largest = r;
        }
        if (largest != i){
            SOC temp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = temp;
            MaxHeapify(largest);
        }
    }

    if (category == "male"){
         if(l < heapSize && heap[l].male > heap[i].male){
            largest = l;
        } else {
            largest = i;
        } if (r < heapSize && heap[r].male > heap[largest].male) {
            largest = r;
        }
        if (largest != i){
            SOC temp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = temp;
            MaxHeapify(largest);
        }
    }
}

void OccHeap::BuildMaxHeap(){
    heapSize = length;
    for (int i = ((length - 1)/2); i >= 0; i--){
        MaxHeapify(i);
    }
}

SOC OccHeap::HeapMaximum(){
    SOC max = heap[0];
    heap[0] = heap[heapSize-1];
    heapSize = heapSize-1;
    MaxHeapify(0);
    return max;
}

int OccHeap::left(int i){
    return(((i + 1) * 2) - 1);
}

int OccHeap::right(int i){
    return (((i + 1) * 2 + 1) - 1);
}

int OccHeap::parent(int i){
    return (((i + 1) * 2 + 1) - 1);
}

std::string OccHeap::convertToCommas(int number) {
  std::string numberString = to_string(number);
  for (int i = numberString.length() - 3; i > 0; i -= 3) {
      numberString.insert(i, ",");
  }
  return numberString;
}