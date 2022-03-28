/*
*   Author: Riley Than
*   Date: 2/22/2022
*   Professor: Syrotiuk
*   CSE 310
*   Description: Make a program that takes in CSV files and fetch certain files from query
*/
#include <stdio.h>
#include <iostream> //basic libraries
#include <fstream>
#include <string>

#ifndef DEFNS
#define DEFNS
#include "defns.h"
#endif

#include "heap.h" //modules
#include "ratio.h"
#include "hash.h"
#include "bst.h"
#include "range.h"

using namespace std;

void fillSOCArray(SOC* arr, int arrSize, std::string workerType, std::string year);
void fillEarningsArray(earnings* arr, int arrSize, int yearOne, int yearTwo);
void createHashTableAndBST(std::string year, int size, BST*& tree, Hash*& hash);

SOC parseOccupationLine(std::string line);
earnings parseEarnLine(std::string line);

earnings* globalEarnings = NULL; //Global. Only declared once.
BST* globaltree = NULL;
Hash* globalHash = NULL;

int main(int argc, char** argv){
    std::string yearAsString = argv[1];
    int year = stoi(yearAsString); //This takes in the argument inputed in the terminal to store the year.

    std::string queryNum;
    getline(cin, queryNum); //Take in the first line as the query number

    int numQuery = stoi(queryNum); //Convert to integer to properly establish loop.
 
    for (int i = 0; i < numQuery; i++) { //This takes in all the queries within the text file and performs the respective functions.
        std::string query;
        getline(cin, query); //This gets the query from the line

        cout << "Query: " << query << endl << endl;

        int pos = 0;
        while (query[pos] != ' ') {
           pos++;
        }
        std::string queryType = query.substr(0, pos);
        query.erase(0, pos + 1); //by doing this, I erase the section of the text file to only have the other words contained.

        if (queryType == "find") {
            pos = 0; //reset the position int to take in another.
            while(query[pos] != ' '){
                pos++; 
            }
            queryType = query.substr(0, pos);
            query.erase(0, pos + 1);

            if (queryType == "max"){
                pos = 0;
                while (query[pos] != ' '){
                    pos++;
                }
                std::string workerType = query.substr(0, pos);
                int number = stoi(query.substr(pos));
                SOC* occArr = new SOC[NUM_OCC];
                fillSOCArray(occArr, number, workerType, yearAsString);

                //I just create the object as it will build the heap from there.
                OccHeap* heap = new OccHeap(occArr, NUM_OCC, workerType);

                cout << "Top " << number << " occupations in " << year << " for " << workerType << " workers:" << endl;

                for(int i = 0; i < number; i++) {
                    SOC max = heap->HeapMaximum();
                    cout << "\t" << max.occupation << ": ";
                    if (workerType == "total"){
                        cout << heap->convertToCommas(max.total) << endl;
                    }

                    if(workerType == "female"){
                        cout << heap->convertToCommas(max.female) << endl;
                    }

                    if(workerType == "male"){
                        cout << heap->convertToCommas(max.male) << endl;
                    }
                }
            } else if (queryType == "ratio"){

                pos = 0;
                while (query[pos] != ' '){
                    pos++;
                }
                std::string firstYear = query.substr(0,pos);

                int yearOne = stoi(firstYear);

                query.erase(0,pos+1);
                
                std::string secondYear = query; //since its just second year now

                int yearTwo = stoi(secondYear);

                if(yearOne > yearTwo){
                    int temp = yearOne;
                    yearOne = yearTwo;
                    yearTwo = temp;
                }

                if (globalEarnings == NULL){
                    globalEarnings = new earnings[60];
                    fillEarningsArray(globalEarnings, 60, yearOne, yearTwo);
                }
                Ratio* ratio = new Ratio(globalEarnings, 60);
                cout << "The female-to-male earnings ratio for " << yearOne << "-" << yearTwo << ":" << endl;
                ratio->printRatios(yearOne, yearTwo);
            } else if (queryType == "occupation"){
                if(globaltree == NULL || globalHash == NULL){
                    createHashTableAndBST(yearAsString, NUM_OCC, globaltree, globalHash);
                }

                hash_table_entry* result = globalHash->search(query);

                if (result == NULL){
                    cout << "Occpuation with SOC code " << query << " not found." << endl;
                } else {
                    cout << "The occupation with SOC code " << query << ":" << endl;
                    cout << "\t" << result->node->soc.occupation << ":" << " YRFT: " << globalHash->convertToCommas(result->node->soc.total) << ", Female: " << globalHash->convertToCommas(result->node->soc.female) << ", Male: " << globalHash->convertToCommas(result->node->soc.male) << endl;
                }
            } else {
                //abort
            }

        } else if(queryType == "range") {
            pos = 0; //reset the position int to take in another.
            while(query[pos] != ' '){
                pos++; 
            }
            query.erase(0, pos+1);

            pos = query.find('\"') + 1;

            std::string firstArg = "";
            std::string secondArg = "";

            while(query[pos] != '\"'){
                firstArg += query[pos];
                pos++;
            }

            pos++;

            while(query[pos] != '\"'){
                pos++;
            }

            pos++;

            while (query[pos] != '\"'){
                secondArg += query[pos];
                pos++;
            }

            if(globaltree == NULL || globalHash == NULL){
                createHashTableAndBST(yearAsString, NUM_OCC, globaltree, globalHash);
            }
            Range* range = new Range(globaltree->getHead());

            cout << "The occupations in the range " << '\"' << firstArg << '\"' << " to " << '\"' << secondArg << '\"' << ":" << endl;
            int found = 0;
            range->printOcc(globaltree->getHead(), firstArg, secondArg, found);
            if(found == 0){
                cout << "\tNo occupations found in the given range" << endl;
            } 

        } else{
            //abort
        }
    }
    return 0;
}

void fillSOCArray(SOC* arr, int arrSize, std::string workerType, std::string year) {
    ifstream occFile;
    occFile.open("Occupation-Dist-All-" + year + ".csv");

    std::string line;
    for(int i = 0; i < 5; i++){ //this skips the first 5 lines
        getline(occFile, line);
    }

    int arrayPos = 0;

    for(line; getline(occFile, line);){ //loop until there is nothing left
        arr[arrayPos] = parseOccupationLine(line);
        arrayPos++;
    }

    occFile.close();
}

SOC parseOccupationLine(std::string line) {
        struct SOC obj; //every loop, create a new object.
        std::string occName;
        std::string code;
        std::string totalString;
        std::string femaleString;
        std::string maleString;
        
        //occupation name
        if(line[0] == '\"'){
            line.erase(0, 1);
            while(line[0] != '\"'){
                occName += line.at(0);
                line.erase(0, 1);
            }
            line.erase(0, 2); //to accomodate for the ,
        } else {
            while(line[0] != ','){
                occName += line.at(0);
                line.erase(0, 1);
            }
            line.erase(0, 1); //to accomodate for the ,
        }

        //SOC Code
        if(line[0] == '\"') {
            line.erase(0, 1);
            while(line[0] != '\"') {
                code += line.at(0);
                line.erase(0, 1);
            }
            line.erase(0, 2);
        } else {
            while (line[0] != ',') {
                code += line.at(0);
                line.erase(0, 1);
            }
            line.erase(0, 1);
        }

        // total workers
        if (line[0] == '\"') {
            line.erase(0, 1);
            while (line[0] != '\"') {
                totalString += line[0];
                line.erase(0, 1);
            }
            line.erase(0, 2);
        } else {
            while (line[0] != ',') {
                totalString += line[0];
                line.erase(0, 1);
            }
            line.erase(0, 1);
        }

        // female workers
        if (line[0] == '\"') {
            line.erase(0, 1);
            while (line[0] != '\"') {
                femaleString += line[0];
                line.erase(0, 1);
            }
            line.erase(0, 2);
        } else {
            while (line[0] != ',') {
                femaleString += line[0];
                line.erase(0, 1);
            }
            line.erase(0, 1);
        }
        // male workers
        if (line[0] == '\"') {
            line.erase(0, 1);
            while (line[0] != '\"') {
                maleString += line[0];
                line.erase(0, 1);
            }
            line.erase(0, 2);
        } else {
            while (line.length() > 0) {
                if (line[0] != '\r') {
                    maleString += line[0];
                }
                line.erase(0, 1);
            }
            line.erase(0, 1);
        }

        std::string newTotal;
        std::string newFemale;
        std::string newMale;

        for(int i = 0; i < totalString.length(); i++){
            if (totalString[i] != ','){
                newTotal += totalString[i];
            }
        }

        for(int i = 0; i < femaleString.length(); i++){
            if (femaleString[i] != ',') {
                newFemale += femaleString[i];
            }
        }

        for(int i = 0; i < maleString.length(); i++){
            if (maleString[i] != ','){
                newMale += maleString[i];
            }
        }


        int totalInt = stoi(newTotal);
        int femaleInt = stoi(newFemale);
        int maleInt = stoi(newMale);
        //set object parameters

        // Fill in C-style strings
        strcpy(obj.occupation, occName.c_str());
        strcpy(obj.SOC_code, code.c_str());

        // Fill in integers
        obj.total = totalInt;
        obj.female = femaleInt;
        obj.male = maleInt;

        return obj;
}

void fillEarningsArray(earnings* arr, int size, int yearOne, int yearTwo) {
    ifstream earningsFile;
    earningsFile.open("Earnings-1960-2019.csv");

    std::string line;
    for(int i = 0; i < 8; i++){ //this skips the first 8 lines
        getline(earningsFile, line);
    }

    int arrayPos = 0;

    for(line; getline(earningsFile, line);) {
        arr[arrayPos] = parseEarnLine(line);
        arrayPos++;
    }

    earningsFile.close();

}

earnings parseEarnLine(std::string line) {
        earnings earn;
        std::string yearString = "";
        std::string maleTotalString = "";
        std::string maleWithEarningsString = "";
        std::string maleEarningsString = "";
        std::string maleMOEString = "";
        std::string femaleTotalString = "";
        std::string femaleWithEarningsString = "";
        std::string femaleEarningsString = "";
        std::string femaleMOEString = "";

        int i = 0;
        while(line[i] != ','){
            i++;
        }
        yearString = line.substr(0, i);
        line.erase(0,i+1);

        int yearInt = stoi(yearString);
        
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') { 
                line.erase(0,1);
                continue;
            }
            maleTotalString += line[0];
            line.erase(0,1);
            }
            line.erase(0,1); 
        } else {
            while (line[0] != ',') {
            maleTotalString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            maleWithEarningsString += line[0];
            line.erase(0,1);
            }
            line.erase(0,1); 
        } else {
            while (line[0] != ',') {
            maleWithEarningsString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            maleEarningsString += line[0];
            line.erase(0,1); 
            }
            line.erase(0,1);
        } else {
            while (line[0] != ',') {
            maleEarningsString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1);
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            maleMOEString += line[0];
            line.erase(0,1); 
            }
            line.erase(0,1);
        } else {
            while (line[0] != ',') {
            maleMOEString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            femaleTotalString += line[0];
            line.erase(0,1); 
            }
            line.erase(0,1);
        } else {
            while (line[0] != ',') {
            femaleTotalString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            femaleWithEarningsString += line[0];
            line.erase(0,1); 
            }
            line.erase(0,1);
        } else {
            while (line[0] != ',') {
            femaleWithEarningsString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            femaleEarningsString += line[0];
            line.erase(0,1); 
            }
            line.erase(0,1);
        } else {
            while (line[0] != ',') {
            femaleEarningsString += line[0];
            line.erase(0,1);
            }
        }

        line.erase(0,1); 
        if (line[0] == '\"') {
            line.erase(0,1); 
            while (line[0] != '\"') {
            if (line[0] == ',') {
                line.erase(0,1);
                continue;
            }
            femaleMOEString += line[0];
            line.erase(0,1);
            }
        } else {
            while (line.length() > 0) {
                if (line[0] == '\r') {
                    break;
                }
                femaleMOEString += line[0];
                line.erase(0,1);
            }
        }

        if (yearString == "N") {
            earn.year = NULL;
        }
        else {
            earn.year = stoi(yearString);
        }
        // 
        if (maleTotalString == "N") {
            earn.male_total = NULL;
        }
        else {
            earn.male_total = stoi(maleTotalString);
        }

        if (maleWithEarningsString == "N") {
            earn.male_with_earnings = NULL;
        }
        else {
            earn.male_with_earnings = stoi(maleWithEarningsString);
        }

        if (maleEarningsString == "N") {
            earn.male_earnings = NULL;
        }
        else {
            earn.male_earnings = stoi(maleEarningsString);
        }
        
        if (maleMOEString == "N") {
            earn.male_earnings_moe = NULL;
        }
        else {
            earn.male_earnings_moe = stoi(maleMOEString);
        }

        if (femaleTotalString == "N") {
            earn.female_total = NULL;
        }
        else {
            earn.female_total = stoi(femaleTotalString);
        }

        if (femaleWithEarningsString == "N") {
            earn.female_with_earnings = NULL;
        }
        else {
            earn.female_with_earnings = stoi(femaleWithEarningsString);
        }

        if (femaleEarningsString == "N") {
            earn.female_earnings = NULL;
        }
        else {
            earn.female_earnings = stoi(femaleEarningsString);
        }
        
        if (femaleMOEString == "N") {
            earn.female_earnings_moe = NULL;
        }
        else {
            earn.female_earnings_moe = stoi(femaleMOEString);
        }

        return earn;
}

void createHashTableAndBST(std::string year, int size, BST*& tree, Hash*& hash){
    ifstream occFile;
    occFile.open("Occupation-Dist-All-" + year + ".csv");

    string line;
    for(int i = 0; i < 5; i++){ //this skips the first 5 lines
        getline(occFile, line);
    }

    bst* head = new bst;
    getline(occFile,line);

    SOC newStruct = parseOccupationLine(line);

    head->soc = newStruct;
    head->left = NULL;
    head->right = NULL;

    tree = new BST(head);

    hash = new Hash(size);

    for(int i = 0; i < size-1; i++){
        getline(occFile,line);
        newStruct = parseOccupationLine(line);

        bst* newNode = new bst;
        newNode->soc = newStruct;
        newNode->left = NULL;
        newNode->right = NULL;

        std::string code = newStruct.SOC_code;
        if(code.length() > 7){
            int numberOfCodes = 1;
            for(int j = 0; j < code.length(); j++){
                if(code[j] == ','){
                    numberOfCodes++;
                }

                if(code[j] == '&' && numberOfCodes < 2){
                    numberOfCodes++;
                }
            }

            string* newArr = new string[numberOfCodes];
            int tracker = 0;
            for(int k = 0; k < code.length(); k++){
                if(code[k] == ','){
                    tracker++;
                }

                if(code[k] == '&' && tracker < 1){
                    tracker++;
                }

                if(code[k] == '-' || (code[k] >= '0' && code[k] <= '9')){
                    newArr[tracker] += code[k];
                }
            }

            for(int l = 0; l < numberOfCodes; l++){
                hash_table_entry* newEntry = new hash_table_entry;

                newEntry->node = newNode;
                strcpy(newEntry->SOC_code, newArr[l].c_str());
                newEntry->next = NULL;
                hash->insertItem(newEntry);
            }
        } else {
                hash_table_entry* newEntry = new hash_table_entry;

                newEntry->node = newNode;
                strcpy(newEntry->SOC_code, newStruct.SOC_code);
                newEntry->next = NULL;
                hash->insertItem(newEntry);
        }
        tree->insert(newNode);
    }

    occFile.close();

}