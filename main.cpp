/*
*   Author: Riley Than
*   Date: 2/22/2022
*   Professor: Syrotiuk
*   CSE 310
*   Description: Make a program that takes in CSV files and fetch certain files from query
*/
#include <stdio.h>
#include <iostream> //basic libraries
#include <ifstream>
#include <string>
#include "defns.h"
#include "heap.h" //modules
#include "ratio.h"
using namespace std;

void findMax(SOC* arr, int arrSize, string workerType);
void findRatio(earnings* arr, int arrSize, int yearOne, int yearTwo);

int main(int argc, char** argv){
    string yearAsString = argv[1];
    int year = stoi(yearAsString); //This takes in the argument inputed in the terminal to store the year.

    string queryNum;
    getline(cin, queryNum); //Take in the first line as the query number

    int numQuery = stoi(queryNum); //Convert to integer to properly establish loop.

    for(int i = 0; i < numQuery; i++){ //This takes in all the queries within the text file and performs the respective functions.
        string queryName;
        getline(cin, queryName); //This gets the query from the line

        int pos = 0;
        while(queryName[pos] != ' '){
           pos++; 
        }
        string queryType = queryName.substr(0, pos);
        queryName.erase(0, pos + 1); //by doing this, I erase the section of the text file to only have the other words contained.

        if(queryType == "find"){
            pos = 0; //reset the position int to take in another.
            while(queryName[pos] != ' '){
                pos++; 
            }
            queryType = queryName.substr(0, pos);
            queryName.erase(0, pos);

            if (queryName == "max"){
                pos = 0;
                while (queryName[pos] != ' '){
                    pos++
                }
                string workerType = queryName.substr(0, pos);
                int number = stoi(query.substr(pos+1));

                SOC* occArr = new SOC[number];
                findMax(occArr, number, workerType);

                OccHeap* heap = new OccHeap(occArr, number, workerType);
                //I just create the object as it will build the heap from there.
            } else if (queryName == "ratio"){
                pos = 0;
                while (queryName[pos] != ' '){
                    pos++
                }
                string firstYear = queryName.substr(0,pos);

                int yearOne = stoi(firstYear);

                queryName.erase(0,pos+1);
                
                pos = 0;
                while (queryName[pos] != '\n'){
                    pos++
                }
                string secondYear = queryName.substr(0,pos); //since the end of the line is a \n

                int yearTwo = stoi(secondYear);

                if(yearOne > yearTwo){
                    int temp = yearOne;
                    yearOne = yearTwo;
                    yearTwo = temp;
                }

                earnings* earn = new earnings[2];
                findRatio(earn, 2, yearOne, yearTwo);


            } else if (queryName == "occupation"){
                //not supported for now so abort
            } else {
                //abort
            }

        } else if(queryType == "range") {
            // not supported for now so abort.
        } else{
            //abort
        }
    }
    return 0;
}

void findMax(SOC* arr, int arrSize, string workerType){
    ifstream occFile;
    occFile.open("Occupation-Dist-All-YYYY.csv");

    for(i = 0; i < 5; i++){ //this skips the first 5 lines
        occFile.getline();
    }

    for(string line; getline(occFile, line);){ //loop until there is nothing left
        SOC* obj = new SOC; //every loop, create a new object.
        string occName;
        string code;
        string totalString;
        string femaleString;
        string maleString;

        //occupation name
        if(line[0] == '\"'){
            int i = 1;
            while(line[i] != '\"'){
                i++;
            }
            occName = line.substr(0, i+1);
            line.erase(0,i+2); //to accomodate for the ,
        } else {
            int i = 0;
            while(line[i] != ','){
                i++;
            }
            occName = line.substr(0, i);
            line.erase(0,i+1); //to accomodate for the ,
        }

        //SOC Code
        if(line[0] == '\"'){
            int i = 1;
            while(line[i] != '\"'){
                i++;
            }
            code = line.substr(0, i+1);
            line.erase(0,i+2);
        } else {
            int i = 0;
            while(line[i] != ','){
                i++;
            }
            code = line.substr(0, i+1);
            line.erase(0,i+1);
        }

        //total workers
        int i = 0;
        while(line[i] != '\"'){
            i++;
        }
        totalString = line.substr(0,i+1);
        line.erase(0,i+2);

        //female workers
        int j = 0;
        while(line[j] != '\"'){
            j++;
        }
        femaleString = line.substr(0,i+1);
        line.erase(0,i+2);

        //male workers
        maleString = line;

        string newTotal;
        string newFemale;
        string newMale;

        for(int i = 1; i < totalString.length(); i++){ //I start at 1 to get rid of the quote. end at length to get rid of the other quote
            if (totalString[i] == ','){
                //left blank so it skips
            } else {
                newTotal += totalString[i];
            }
        }

        for(int i = 1; i < femaleString.length(); i++){
            if (femaleString[i] == ','){
                //left blank so it skips
            } else {
                newFemale += femaleString[i];
            }
        }

        for(int i = 1; i < maleString.length(); i++){
            if (maleString[i] == ','){
                //left blank so it skips
            } else {
                newMale += maleString[i];
            }
        }

        int totalInt = stoi(newTotal);
        int femaleInt = stoi(newFemale);
        int maleInt = stoi(newMale);
        
        //set object parameters
        obj->occupation = occName;
        obj->SOC_code = code;
        obj->total = totalInt;
        obj->female = femaleInt;
        obj->male = maleInt;

        //compare the object with all members of the array.
      for(int i = 0; i < arrSize; i++){
          if(workerType == "total"){
              if (obj->total > arr[i].total){ //if obj total > arr total, replace
                  arr[i] = obj; 
              }
          } else if(workerType == "female"){
              if (obj->female > arr[i]->female){ //if obj female > arr female, replace
                  arr[i] = obj; 
              }
          } else if(workerType == "male"){
              if (obj->male > arr[i]->male){ //if obj male > arr male, replace
                  arr[i] = obj; 
              }
          } else {
              //abort the program.
          }
      }
    }
    occFile.close();
}

void findRatio(earnings* arr, int size, int yearOne, int yearTwo){
    ifstream earningsFile;
    earningsFile.open("Earnings-1960-1999.csv");

    for(i = 0; i < 7; i++){ //this skips the first 7 lines
        earningsFile.getline();
    }

    for(string line; getline(earningsFile, line);){
        earnings* earn = new earnings;
        string yearString;
        string maleTotalString;
        string maleWithEarningsString;
        string maleEarningsString;
        string maleMOEString;
        string femaleTotalString;
        string femaleWithEarningsString;
        string femaleEarningsString;
        string femaleMOEString;

        int i = 0;
        while(line[i] != ','){
            i++;
        }
        yearString = line.substr(0, i);
        line.erase(0,i+1);

        int yearInt = stoi(yearString);

        if (yearInt == yearOne || yearInt == yearTwo){
            i = 0;
            while(line[i] != ','){
                i++;
            }
            maleTotalString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            maleWithEarningsString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            maleEarningsString = line.substr(0,pos);
            line.erase(0,pos+1);            
            
            i = 0;
            while(line[i] != ','){
                i++;
            }
            maleMOEString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            femaleTotalString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            femaleWithEarningsString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            femaleEarningsString = line.substr(0,pos);
            line.erase(0,pos+1);

            i = 0;
            while(line[i] != ','){
                i++;
            }
            femaleMOEString = line.substr(0,pos);
            line.erase(0,pos+1);

            string newMaleTotal;
            string newMaleWithEarnings;
            string newMaleEarnings;
            string newMaleMOE;
            string newFemaleTotal;
            string newFemaleWithEarnings;
            string newFemaleEarnings;
            string newFemaleMOE; 

            for (int i = 1; i < maleTotalString.length(); i++){
                if (maleTotalString[i] == ','){
                //left blank so it skips
                } else {
                    newMaleTotal += maleTotalString[i];
                }
            }

            for (int i = 1; i < maleWithEarningsString.length(); i++){
                if (maleWithEarningsString[i] == ','){
                //left blank so it skips
                } else {
                    newMaleWithEarnings += maleWithEarningsString[i];
                }
            }

            for (int i = 1; i < maleEarningsString.length(); i++){
                if (maleEarningsString[i] == ','){
                //left blank so it skips
                } else {
                    newMaleEarnings += maleEarningsString[i];
                }
            }
            if(maleMOEString[0] == '\"'){
                for (int i = 1; i < maleMOEString.length(); i++){
                    if (maleMOEString[i] == ','){
                    //left blank so it skips
                    } else {
                        newMaleMOE += maleMOEString[i];
                    }
                }
            }else {
                for (int i = 0; i < maleMOEString.length(); i++){
                    if (maleMOEString[i] == ','){
                    //left blank so it skips
                    } else {
                        newMaleMOE += maleMOEString[i];
                    }
                }
            }

            for (int i = 1; i < femaleTotalString.length(); i++){
                if (femaleTotalString[i] == ','){
                //left blank so it skips
                } else {
                    newFemaleTotal += femaleTotalString[i];
                }
            }

            for (int i = 1; i < femaleWithEarningsString.length(); i++){
                if (femaleWithEarningsString[i] == ','){
                //left blank so it skips
                } else {
                    newFemaleWithEarnings += femaleWithEarningsString[i];
                }
            }

            for (int i = 1; i < femaleEarningsString.length(); i++){
                if (femaleEarningsString[i] == ','){
                //left blank so it skips
                } else {
                    newFemaleEarnings += femaleEarningsString[i];
                }
            }

            if(femaleMOEString[0] == '\"'){
                for (int i = 1; i < femaleMOEString.length(); i++){
                    if (femaleMOEString[i] == ','){
                    //left blank so it skips
                    } else {
                        newFemaleMOE += femaleMOEString[i];
                    }
                }
            }else {
                for (int i = 0; i < femaleMOEString.length(); i++){
                    if (femaleMOEString[i] == ','){
                    //left blank so it skips
                    } else {
                        newFemaleMOE += femaleMOEString[i];
                    }
                }
            }

            int maleTotalInt = stoi(newMaleTotal);
            int maleWithEarningsInt = stoi(newMaleWithEarnings);
            int maleEarningsInt = stoi(newMaleEarnings);
            int maleMOEInt = stoi(newMaleMOE);
            int femaleTotalInt = stoi(newFemaleTotal;);
            int femaleWithEarningsInt = stoi(newFemaleWithEarnings);
            int femaleEarningsInt = stoi(newFemaleEarnings);
            int femaleMOEInt = stoi(newFemaleMOE);

            earn->year = yearInt;
            earn->male_total = maleTotalInt;
            earn->male_with_earnings = maleWithEarningsInt;
            earn->male_earnings_moe = maleMOEInt;
            earn->female_total = femaleTotalInt;
            earn->female_with_earnings = femaleWithEarningsInt;
            earn->female_earnings = femaleEarningsInt;
            earn->female_earnings_moe = femaleMOEInt;

            if(yearInt == yearOne){
                arr[0] = earn;
            } else {
                arr[1] = earn;
            }

        }
        //skip if there is no match

    }

}