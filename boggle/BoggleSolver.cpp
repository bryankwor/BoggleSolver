// boggle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <fstream>
#include <vector>

const std::string dictionaryFileName = "dictionary.txt";
const std::string boggleBoardFileName = "boggleBoard.txt";

enum BoardDimensions
{
	BOARD_WIDTH = 4,
	BOARD_HEIGHT = 4
};

struct LetterNode
{
public: 
	LetterNode() : letter(" "), visited(false), neighborsPushed(false), x(0), y(0) {}

public:
	std::string letter;
	bool visited;
	bool neighborsPushed;
	int x;
	int y;
	std::vector<LetterNode> adjacentLetters;
};

std::vector<std::string> findBoggleWords();
void findWords(const int &x, const int &y, LetterNode** gameBoard, Trie* dictionary, std::string &word, std::vector<std::string> &wordsOnBoard);
void getNeighbors(const int &xPos, const int &yPos, LetterNode** gameBoard);
void loadDictionary(Trie* dictionary);
void loadGameBoard(LetterNode** gameBoard);
void resetVisitedNodes(LetterNode** gameBoard);

int _tmain(int argc, _TCHAR* argv[])
{
	findBoggleWords();
	return 0;
}

std::vector<std::string> findBoggleWords()
{
	std::string word; 
	Trie* dictionary = new Trie();
	std::vector<std::string> wordsOnBoard;

	LetterNode** gameBoard = new LetterNode*[BOARD_WIDTH];
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		gameBoard[i] = new LetterNode[BOARD_HEIGHT];
	}

	// Load data into Trie and game board.
	loadDictionary(dictionary);
	printf("Dictionary Loaded.\n");

	loadGameBoard(gameBoard);
	printf("Boggle Board Loaded.\n");

	// Search for all words on the Boggle board.
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		for (int j = 0; j < BOARD_HEIGHT; ++j)
		{ 
			findWords(i, j, gameBoard, dictionary, word, wordsOnBoard);
		}
	}

	// Free memory.
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		delete [] gameBoard[i];
	}
	delete gameBoard;

	return wordsOnBoard;
}

void findWords(const int &x, const int &y, LetterNode** gameBoard, Trie* dictionary, std::string &word, std::vector<std::string> &wordsOnBoard)
{
	if (!gameBoard[x][y].visited)
	{
		word.append(gameBoard[x][y].letter);
		gameBoard[x][y].visited = true;
	}
	else
	{
		return;
	}

	// If the current word being built is not a valid path in the prefix tree, do not continue.
	if (!dictionary->isValidPrefix(word))
	{
		word.pop_back();
		gameBoard[x][y].visited = false;
		return;
	}


	// Only words of length greater than 2 are valid in Boggle.
	if (word.length() > 2 && dictionary->hasWord(word))
	{
		wordsOnBoard.push_back(word);
	}

	if (!gameBoard[x][y].neighborsPushed)
	{
		getNeighbors(x, y, gameBoard);
		gameBoard[x][y].neighborsPushed = true;
	}

	// Traverse neighbors first before continuing.
	for (auto iterator = gameBoard[x][y].adjacentLetters.begin(); iterator < gameBoard[x][y].adjacentLetters.end(); ++iterator)
	{
		findWords(iterator->x, iterator->y, gameBoard, dictionary, word, wordsOnBoard);
	}

	gameBoard[x][y].visited = false;
	word.pop_back();
}

void getNeighbors(const int &xPos, const int &yPos, LetterNode** gameBoard)
{
	int index = 0;

	// Push all adjacent neighbors.
	for (int i=-1; i<=1; ++i)
	{
		for (int j=-1; j<=1; ++j)
		{
			// Check to prevent array boundary errors.
			if (xPos+i >= 0 && xPos+i < BOARD_WIDTH && yPos+j >= 0 && yPos+j < BOARD_HEIGHT)
			{
				if (!gameBoard[xPos+i][yPos+j].visited)
				{
					gameBoard[xPos][yPos].adjacentLetters.push_back(gameBoard[xPos+i][yPos+j]);
					++index;
				}
			}
		}
	}
}

void loadDictionary(Trie* dictionary)
{
	std::string word;
	std::ifstream fin(dictionaryFileName.c_str());

	assert(fin.good() && printf("Loading Dictionary into Trie.\n", dictionaryFileName));

	while (fin >> word)
	{
		dictionary->addWord(word);
	}
}

void loadGameBoard(LetterNode** gameBoard)
{
	char letter;
	std::ifstream fin(boggleBoardFileName.c_str());

	assert(fin.good() && printf("Loading Boggle board.\n", boggleBoardFileName));

	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		for (int j = 0; j < BOARD_HEIGHT; ++j)
		{
			fin >> letter;
			gameBoard[i][j].letter = letter;
			gameBoard[i][j].x = i;
			gameBoard[i][j].y = j;
		}
	}
}