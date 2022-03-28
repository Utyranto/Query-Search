#include "hash.h"

Hash::Hash(int i){
    int tableSize = 3 * i;
    while(!TestForPrime(tableSize)){
        tableSize++;
    }
    this->hashSize = tableSize;

    hashTable = new hash_table_entry*[tableSize];

    for(int x = 0; x < tableSize; x++){
        hashTable[x] = NULL;
    }
}

void Hash::insertItem(hash_table_entry* point){
    int pos = hashFunction(point->SOC_code);
    if(hashTable[pos] == NULL){
        hashTable[pos] = point;
    } else {
        point->next = hashTable[pos]; //collision handling
        hashTable[pos] = point;
    }   
}

int Hash::hashFunction(std::string code){
    std::string newCode = code;
    newCode.erase(2,1);
    int intCode = stoi(newCode);

    return (intCode % hashSize);
}

hash_table_entry* Hash::search(std::string code){
    int position = hashFunction(code);
    hash_table_entry* Node = hashTable[position];
    if(code == Node->SOC_code){
        return Node;
    } 

    while(Node != NULL){
        if(code == Node->SOC_code){
            return Node;
        }
        Node = Node->next;
    }
    return NULL;
}

bool Hash::TestForPrime( int val )
{
    int limit, factor = 2;

    limit = (long)( sqrtf( (float) val ) + 0.5f );
    while( (factor <= limit) && (val % factor) )
        factor++;

    return( factor > limit );
}

std::string Hash::convertToCommas(int number) {
  std::string numberString = std::to_string(number);
  for (int i = numberString.length() - 3; i > 0; i -= 3) {
      numberString.insert(i, ",");
  }
  return numberString;
}