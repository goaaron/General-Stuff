/*
 * File: RandomWriter.cpp
 * ----------------------
 * Implements the random writer
 * Name: Aaron Goebel
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "filelib.h"
#include "map.h"
#include "simpio.h"
#include "strlib.h"
#include "random.h"
#include "vector.h"
#include "console.h"
using namespace std;

//Prototypes

void parse(string file, int k, Map<string, Vector<char> > &rmap);
void seedCount(Map<string, Vector<char> > &rmap);
void randomWrite(string seed, Map<string, Vector<char> > &rmap);

//Main
    
int main() {
    Map<string, Vector<char> > rmap;
    string file = getLine("Enter the name of the file you wish to choose : ");
    int k = getInteger("Enter the order of the analysis you wish to complete : ");
    while(true){
    if(k>10 | k<1) {
        cout << "Invalid input. Please enter a value between 1 and 10 : ";
        k = getInteger();
    }
    else break;
    }
    parse(file, k, rmap);
    return 0;
}

/*
void parse(string file, int k, Map<string, Vector<char> >&rmap)
--------------------------------------------------------------
Reads input file with error checking and generates a map for seed values and their characters. 
The function ends with a call to the seedCount function.
*/

void parse(string file, int k, Map<string, Vector<char> > &rmap) {
    ifstream sFile;                 //  Must first open the file stream and check for errors
    Vector<char> characvec;
    char ch;
    while(true) {
        sFile.open(file.c_str());
        if(!sFile.fail()) break;
        cout << "Invalid file name." << endl << "Please reenter a valid file name : " ;
        sFile.clear();
        file = getLine();
		cout << "Please reenter the order of analysis : ";
		k = getInteger();
    }
    string seed = "";               // initialize the "seed" and read the file
    while(seed.length() < k && !sFile.fail()) {
        ch = sFile.get();
        seed += ch;                 //creates a seed of length k according to the given order
    }
    while(!sFile.fail()) {        //for the rest of the file (not concerning generating the seed),    
        ch = sFile.get();                     //appends values to the vector in the map                                                                          
        if(rmap.containsKey(seed)) {
			characvec = rmap.get(seed);		 //we need to map a seed to a vector of characters
        }
        characvec.add(ch);
        rmap.put(seed, characvec);       //adds more characters to a seed if it is recurring so that we can better account for its prominence
        seed = seed.substr(1) + ch;		 //iterates the seed by dropping off the first letter and adding on the next
        characvec.clear();				
    }                               
    seedCount(rmap);
}

/*
 void seedCount(Map<string, Vector<char> > &rmap)
 ------------------------------------------------------------------------
 Determines what seed is most prominent in its representation of the text by determining its relative frequency
 (by vector length) to other seeds.
 */

void seedCount(Map<string, Vector<char> > &rmap) {
    Vector<char> large;
    int veclen = 0;							//vector length paramater
    string mostfreqseed;
    foreach (string seed in rmap) {
        large = rmap.get(seed);              //extracts each vector from a seed
        if(large.size() > veclen) {
            mostfreqseed = seed;            //updates for every seed and checks the inequality
            veclen = large.size();
        }
    }
    randomWrite(mostfreqseed, rmap);
}

/*
 void randomWrite(string mostfreqseed, Map<string, Vector<char> > &rmap)
 --------------------------------------------------------------
 Outputs 2000 random chracters from a vector related to a seed.
 */

void randomWrite(string mostfreqseed, Map<string, Vector<char> > &rmap) {
    const int characterOutput = 2000;
    Vector<char> randvec;
    for(int i=0; i<characterOutput; i++) {
        randvec = rmap.get(mostfreqseed);
		mostfreqseed = mostfreqseed.substr(1) + letter;
        char letter = randvec[randomInteger(0, randvec.size()-1)];
        cout << letter;   
    }
    cout << endl;
}

    
    
    
    

