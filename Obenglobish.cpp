//Obenglobish

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

string translate(string word);

int main() {
   while (true) {
      string word = getLine("Enter a word: ");
      if (word == "") break;
      string translated = translate(word);
        cout << word << " -> " << translated << endl;
    }
    

/*
 * Function: translate(word)
 * Usage: string translatedword = translate(word)
 * ---------------------------------------------
 * Returns the Obenglobish translation of a user input word.
 */
string translate(string word){
	int i = 0;
    while(i < word.length()) {
		for(word[i] == 'a' ||  word[i] == 'i' || word[i] == 'o' || word[i] == 'u'){
			if(word[i-1] !== 'a' && word[i-1] !== 'e' && word[i-1] == 'i' && word[i-1] !== 'o' && word[i-1] !== 'u');
			word.insert(i, "ob");
            i= i+3;
		}
		else if(i==word.length && word[word.length()-1]=='e'){
			break;
		}
		else if(word[i] == 'e' && prev==false) {
			word.insert(i, "ob");
            i= i+3;
        }
		else i++;
    }
    return word;
}