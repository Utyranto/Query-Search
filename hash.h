#pragma once

#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif

#include <iostream>
#include <string>
#include <math.h>

class Hash{
    private:
        int hashSize; //will be set to the next prime number 3 times the size
        hash_table_entry** hashTable;
    public:
        Hash(int i); 

        void insertItem(hash_table_entry* point);

        int hashFunction(std::string code);

        bool TestForPrime(int val);

        hash_table_entry* search(std::string code);

        std::string convertToCommas(int num);

        ~Hash();
};