//Boggle Game

#include "genlib.h"
#include "gboggle.h"
#include "sound.h"
#include "extgraph.h"
#include "simpio.h"
#include "lexicon.h"
#include "random.h"
#include "grid.h"
#include <iostream>

const int MIN_WORD_COUNT = 4;

struct cell{
	int row, col;
};

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute" << endl
	 << "dice.  These 6-sided dice have letters rather than numbers on the faces, " << endl
	 << "creating a grid of letters on which you try to form words.  You go first, " << endl
	 << "entering the words you find that are formed by tracing adjoining " << endl
	 << "letters.  Two letters adjoin if they are next to each other horizontally, " << endl
	 << "vertically, or diagonally. A letter can only be used once in the word. Words" << endl
	 << "must be at least 4 letters long and can only be counted once.  You score points" << endl
	 << "based on word length, a 4-letter word is worth one, 5-letters two, etc.  After your " << endl
	 << "tiny brain is exhausted, I, the brilliant computer, will find all the remaining " << endl
	 << "words in the puzzle and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
		
    GetLine();
}

void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle." << endl
	 << "The good news is that you might improve your vocabulary a bit.  The" << endl
	 << "bad news is that you're probably going to lose miserably to this little" << endl
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM...\n" << endl;
}

string readLineFromFile(ifstream &infile);
bool openFile(string filename, ifstream& infile);
bool readInFile(string filename, Vector<string> &theDice, int dimension = 4);
string buildDiceList(Vector<string> theDice);
Grid<char> buildBoardFromDice(string theDice, int dimension = 4);
void drawOnBoard(Grid<char> &theBoard, int dimension=4);
bool findUsersWord(string findThis, Grid<char> &theBoard, Vector<cell> &theRoute, string alreadyFound="", int placeY=0, int placeX=0);
string findComputersWord(Grid<char> &theBoard, Lexicon alreadyFoundWords, Lexicon dictionary, Vector<cell> &theRoute, int startY=0, int startX=0, string alreadyFound="", int placeY=0, int placeX=0);
void highLightLettersInCube(Vector<cell> route);
string getResponse(string question, bool convertToUpperCase);
char getMouseWord(Grid<char> &theBoard, int dimension);
string addQU(string theStr);
void drawNumberAt(cell theCell, int num);
bool placeAlreadyUsed(int row, int col,Vector<cell> &theRoute);

int main()
{
	Vector<string> theDice;
	int dimension = 4;
	Grid<char> theBoard(dimension, dimension);
	string theDiceList, input, findThis;
	bool mouseBut = false;
	Vector<cell> theWordRoute;

	Welcome();
	Lexicon lex("lexicon.dat");
	// turn on sound
	SetSoundOn(true);

	while (true)
	{
		InitGraphics();
		input = getResponse("Do you wish to enter a grid ? Y/N :", true);
		if (input == "Y")
			while (theDiceList.length() < (dimension * dimension))
				theDiceList = getResponse("Please enter the grid of size " + IntegerToString(dimension * dimension) + " : ", true);
		else
		{
			readInFile("cubes16.txt", theDice);
			theDiceList = buildDiceList(theDice);
		}
		input = getResponse("Do you want to use the mouse ? ", true);
		if (input == "Y") 
			mouseBut=true;
		/*
	//	testing :)
		theDiceList = "REDSROADTOADTIME";
		mouseBut = true;*/
		theBoard = buildBoardFromDice(theDiceList, dimension);
		
		drawOnBoard(theBoard, dimension);
		Lexicon humanFoundWords;

		while (true)
		{
			cout << endl << "enter word (please note Q  = Qu) : ";
			if (mouseBut)
			{
				findThis ="";
				cout << endl << "click outside of the grid to stop" << endl;
				cout << "word : ";
				while (true)
				{
					WaitForMouseDown();
					WaitForMouseUp();
					char newMouseClick = getMouseWord(theBoard, dimension);
					if (newMouseClick == NULL)
						break;
					else
					{
						findThis += newMouseClick;		
						cout << newMouseClick;
					}
				}
			}
			else
				findThis = GetLine();
			if (findThis == "") break;
			// any the U to the Q if there is any in the string findThis
			findThis = addQU(findThis);
			theWordRoute.clear();
			if (lex.containsWord(findThis) && findThis.length() >= MIN_WORD_COUNT)
			{
				if (!humanFoundWords.containsWord(findThis))
				{
					findThis = ConvertToUpperCase(findThis);				
					// need to create a wrapper for this call
					if (findUsersWord(findThis,theBoard, theWordRoute))
					{
						PlayNamedSound("excellent.wav");
						humanFoundWords.add(findThis);
						// write a function that displays the word on the grid.
						highLightLettersInCube(theWordRoute);
						RecordWordForPlayer(findThis, Human);
					}
					else
					{
						cout << "Cannot find that word in the grid : " << findThis << endl;
						PlayNamedSound("denied.wav");
					}
				}else
				{
					cout << "Already found that word \"" << findThis << "\"" << endl;
					PlayNamedSound("not fooling anyone.wav");
				}
			}
			else
			{
				cout << "Sorry that does not appear to be a valid word (also word has to be bigger than "<< MIN_WORD_COUNT<<" in size ) : " << findThis << endl;
				PlayNamedSound("yeah right.wav");
			}
		}

		// the computers turn.. 
		PlayNamedSound("moo.wav");
		cout << endl << "Computers turn" << endl;
		Lexicon computersFoundWords;
		// do not reuse the words already found in the human found words
		computersFoundWords = humanFoundWords;
		cell startingPos;
		startingPos.row = 0;
		startingPos.col =0;
		while (true)
		{
			// and start from the pervious position on the grid, so that we are not searching already places on the grid
			if (theWordRoute.size() > 0)
			{
				// restart from the colum position to 0 if we are at the end of the grid and are have moved down in rows.
				startingPos.col = ((theWordRoute[0].col == dimension-1) && (startingPos.row != theWordRoute[0].row) ? 0 : theWordRoute[0].col);
				startingPos.row = theWordRoute[0].row;
			}
			theWordRoute.clear();
			string computerFound = findComputersWord(theBoard, computersFoundWords, lex,theWordRoute,startingPos.row, startingPos.col);
			if (computerFound =="") break;
			computersFoundWords.add(computerFound);
			RecordWordForPlayer(computerFound, Computer);
			//highLightLettersInCube(theWordRoute);
		}
		cout << "Computer has finished" << endl;
		input = getResponse("Do you wish to play again ? Y / N ", true);
		if (input == "N") break;
	}
	return 0;
}

// add QU to any Q in the input string.
string addQU(string theStr)
{
	int findI =0;
	while (true)
	{
		findI = theStr.find("Q", findI);
		if (findI >= 0)
			theStr = theStr.replace(findI,1,"QU");
		if (findI == -1)
			return theStr;
		findI++;
	}
}

char getMouseWord(Grid<char> &theBoard, int dimension)
{
	double x = GetMouseX();
	double y = GetMouseY();
	// get the start point
	x -= 1.93;
	y -= 3.1;
	y *= -1;
	int row = (int)(y / 0.5);
	int col = (int)(x / 0.5);
	if ((row < dimension) && (col < dimension))
		return theBoard[row][col];
	else
		return NULL;
}

// get a response from the user, convertToUpperCase is if you want to have the returned string converted to uppercase
string getResponse(string question, bool convertToUpperCase)
{
	string returnString;
	while (true)
	{
		cout << question;
		returnString = GetLine();
		if (convertToUpperCase)
			returnString = ConvertToUpperCase(returnString);
		if (returnString != "")
			return returnString;
	}
}

// this version will highlight the words and say where the character is within the word 
void highLightLettersInCube(Vector<cell> route)
{
	int num =1;
	foreach (cell theCell in route)
	{
		HighlightCube(theCell.row, theCell.col, true);
		drawNumberAt(theCell,num++);
	}
	Pause(3);
	foreach (cell theCell in route)
	{
		HighlightCube(theCell.row, theCell.col, false);
	}
}

void drawNumberAt(cell theCell,int num)
{
	double dx, dy;
	dx = 2 + (theCell.col * 0.5);
	dy = 3.1 - (theCell.row * 0.5);
	dy -=0.15;
	MovePen(dx,dy);
	SetPointSize(10);
	DrawTextString(IntegerToString(num));
}

// returns a new word found on the grid, if able to 
string findComputersWord(Grid<char> &theBoard, Lexicon alreadyFoundWords, Lexicon dictionary, Vector<cell> &theRoute, int startY, int startX, string alreadyFound, int placeY, int placeX)
{	
	if (alreadyFound.length() >= MIN_WORD_COUNT)
		if (dictionary.containsWord(alreadyFound))
			if (!alreadyFoundWords.containsWord(alreadyFound))
				return alreadyFound;
	// need to find the first letter within the board and then progress around that.
	if (alreadyFound.empty())
	{
		for (int rows = startY; rows < theBoard.numRows(); rows++)
			for (int cols = startX; cols < theBoard.numCols(); cols++)
				// find the each character within the 
				{
					alreadyFound = theBoard[rows][cols];
					cell newR;
					newR.row = rows;
					newR.col = cols;
					theRoute.add(newR);
					string newWord = findComputersWord(theBoard,alreadyFoundWords, dictionary,theRoute, startY, startX, alreadyFound, rows, cols);
					if (newWord.length() > 0)
						return newWord;
					else
					{
						// clear out the found Board 
						theRoute.clear();
					}
				}
	}
	else
	{
		// try and find the next letters within the area around the base letter
		// spin around the letter 3 * 3 grid
		for (int y= (placeY > 0 ? placeY-1: placeY); y <=(placeY == (theBoard.numRows()-1) ? placeY : placeY+1);y++)
			for (int x=(placeX > 0 ? placeX-1: placeX); x<=(placeX == (theBoard.numCols()-1) ? placeX : placeX+1); x++)
			{
				if (!(y==placeY && x ==placeX))
				{
					// already used letter
					if (!placeAlreadyUsed(y,x,theRoute))
					{
						alreadyFound += theBoard[y][x];
						cell newR;
						newR.row = y;
						newR.col = x;
						theRoute.add(newR);
						if (dictionary.containsPrefix(alreadyFound))
						{
							string newWord = findComputersWord(theBoard, alreadyFoundWords, dictionary,theRoute,startY, startX, alreadyFound, y, x);
							if (newWord.length() > 1)
								return newWord;
							else
							{
								if (theRoute.size() > 0)
									theRoute.removeAt(theRoute.size()-1);
							}
						}else
						{
							if (theRoute.size() > 0)
								theRoute.removeAt(theRoute.size()-1);
						}
						// if this new line of attack does not work out, clear the foundboard and also the alreadyfound variables.
						if (alreadyFound.length() > 0)
							alreadyFound = alreadyFound.substr(0, alreadyFound.length()-1);
					}
				}
			}
	}
	return "";
}

// return true if find the word (findThis) on the boad 
bool findUsersWord(string findThis, Grid<char> &theBoard, Vector<cell> &theRoute, string alreadyFound, int placeY, int placeX)
{	
	// need to find the findThis  base case
	if (findThis == alreadyFound)
		return true;
	// need to find the first letter within the board and then progress around that.
	if (alreadyFound.empty())
	{
		for (int rows = 0; rows < theBoard.numRows(); rows++)
			for (int cols = 0; cols < theBoard.numCols(); cols++)
				// find the each character within the 
				if (theBoard[rows][cols] == findThis[0])
				{
					alreadyFound = findThis[0];
					cell newR;
					newR.row = rows;
					newR.col = cols;
					theRoute.add(newR);
					if (findUsersWord(findThis, theBoard, theRoute, alreadyFound, rows, cols))
						return true;
					else
						// clear out the found Board 
						theRoute.clear();
				}
	}
	else
	{
		// try and find the next letters within the area around the base letter
		// spin around the letter 3 * 3 grid
		for (int y= (placeY > 0 ? placeY-1: placeY); y <=(placeY == (theBoard.numRows()-1) ? placeY : placeY+1);y++)
			for (int x=(placeX > 0 ? placeX-1: placeX); x<=(placeX == (theBoard.numCols()-1) ? placeX : placeX+1); x++)
				if ((theBoard[y][x] == findThis[alreadyFound.length()]) && (!(y==placeY && x ==placeX)))
					// already used letter
					if (!placeAlreadyUsed(y,x,theRoute))
					{
						alreadyFound += findThis[alreadyFound.length()];
						cell newR;
						newR.row = y;
						newR.col = x;
						theRoute.add(newR);
						if (findUsersWord(findThis, theBoard,theRoute, alreadyFound, y, x))
							return true;
						else
						{
							if (alreadyFound.length() > 1)
								alreadyFound = alreadyFound.substr(0, alreadyFound.length()-1);
							theRoute.removeAt(theRoute.size()-1);
						}
					}
		return false;
	}
	return false;
}

// returns if already used, else false.
bool placeAlreadyUsed(int row, int col, Vector<cell> &theRoute)
{
	cell checkCell;
	for (int i =0; i < theRoute.size(); i++)
	{
		checkCell = theRoute[i];
		if (checkCell.row == row && checkCell.col == col) 
			return true;
	}
	return false;
}

void drawOnBoard(Grid<char> &theBoard, int dimension)
{
	DrawBoard(dimension, dimension);
	for (int y =0; y < dimension; y++)
		for (int x = 0; x < dimension; x++)
			LabelCube(y, x, theBoard[y][x]);
}

// build the grid from the input string
Grid<char> buildBoardFromDice(string theDice, int dimension)
{
	Grid<char> returnGrid(dimension, dimension);
	int theDiceI=0;
	for (int y = 0; y < dimension; y++)
		for (int x = 0; x < dimension; x++)
			returnGrid[y][x] = theDice[theDiceI++];
	return returnGrid;
}

// basically rebuild another list that is randomized 
string buildDiceList(Vector<string> theDice)
{	
	string returnList;
	int insert;
	Randomize();
	while (theDice.size() > 0)
	{
		insert = RandomInteger(0, theDice.size()-1);
		returnList += theDice[insert][RandomInteger(0, theDice[insert].length()-1)];
		theDice.removeAt(insert);
	}
	return returnList;
}

//cubes16.txt
bool readInFile(string filename, Vector<string> &theDice, int dimension)
{
	ifstream infile;
	// read in the dimension size of string values
	if (openFile(filename, infile))
	{
		for (int dimI = 0; dimI < (dimension*dimension); dimI++)
			theDice.add(readLineFromFile(infile));
		infile.close();
	}
	else
		return false;
	return true;
}

bool openFile(string filename, ifstream& infile)
{
	infile.open(filename.c_str());
	if (infile.fail()) 
		return false;
	return true;
}

string readLineFromFile(ifstream &infile)
{
	string returnStr;
	getline(infile, returnStr, '\n');
	return returnStr;
}
