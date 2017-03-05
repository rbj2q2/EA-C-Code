#include <iostream>
#include <vector>
#include <string>
#include <float.h>
#include "pacController.h"

using namespace std;

PacController::PacController()
{
	// Set score to 0
	score = 0;

	// Set sum of all scores from all games to 0
	sumAllGameScores = 0;
	// Set number of games played to 0
     numberOfGames = 0;
}

PacController::PacController(const ParsedList& parsedList)
{
	// Initialize this tree from a configuration file
	validator.initialize(parsedList);
	// Set score to 0
	score = 0;

	// Set sum of all scores from all games to 0
	sumAllGameScores = 0;
	// Set number of games played to 0
     numberOfGames = 0;
}

PacController::PacController(const PacController& toBeCopied)
{
	// Copy this tree from another
	validator.copy(0, 0, toBeCopied.validator.root);
	// Copy this score from another
	score = toBeCopied.score;

	// Copy sum of all game score from another
	sumAllGameScores = toBeCopied.sumAllGameScores;
	// Copy number of games from another
     numberOfGames = toBeCopied.numberOfGames;
}

void PacController::chooseMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index)
{
	// Set previous y position to current y position
	get<3>(pacPositions.at(index)) = get<1>(pacPositions.at(index));
	// Set previous x position to current y position
	get<2>(pacPositions.at(index)) = get<0>(pacPositions.at(index));

	// Determines move from tree
	treeMove(validMoves, board, ghosts, pacPositions, index);
}

void PacController::randomMove(const string& validMoves, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index)
{
	// Choose a random position of the validmove string
	int chosenDigit = rand()%validMoves.length();
	// get the char of the position in the string that was chosen randomly
	char selectedMove = validMoves[chosenDigit];

	// perform the move action
	// u stands for up
	if (selectedMove == 'u')
		get<1>(pacPositions.at(index)) -= 1;
	// d stands for down
	else if (selectedMove == 'd')
		get<1>(pacPositions.at(index)) += 1;
	// l stands for left
	else if (selectedMove == 'l')
		get<0>(pacPositions.at(index)) -= 1;
		// r stands for right
	else if (selectedMove == 'r')
		get<0>(pacPositions.at(index)) += 1;

// otherwise, we hold. no position change is required
}

void PacController::treeMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index)
{
	// Set value of best move to minimum value
	float valueOfBestMove = -1.00 * (FLT_MAX);
	// All pac controllers can hold
	char bestMove = 'h';

	// For all characters in valid string
	for (unsigned int i = 0; i < validMoves.length(); i++)
	{
		// Store the value of a given move in temp
		float temp;

		// get value of position above
		if (validMoves.at(i) == 'u')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(pacPositions[index]), get<1>(pacPositions[index]) - 1);
		// get value of position below
		else if (validMoves.at(i) == 'd')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(pacPositions[index]), get<1>(pacPositions[index]) + 1);
		// get value of position to the left
		else if (validMoves.at(i) == 'l')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(pacPositions[index]) - 1, get<1>(pacPositions[index]));
		// get value of position to the right
		else if (validMoves.at(i) == 'r')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(pacPositions[index]) + 1, get<1>(pacPositions[index]));
		// get value of current position
		else
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(pacPositions[index]), get<1>(pacPositions[index]));

		// If this move is better
		if (temp > valueOfBestMove)
		{
			// Store the value and the direction of the move
			valueOfBestMove = temp;
			bestMove = validMoves.at(i);
		}
	}

	// u stands for up
	if (bestMove == 'u')
		get<1>(pacPositions.at(index)) -= 1;
	// d stands for down
	else if (bestMove == 'd')
		get<1>(pacPositions.at(index)) += 1;
	// l stands for left
	else if (bestMove == 'l')
		get<0>(pacPositions.at(index)) -= 1;
		// r stands for right
	else if (bestMove == 'r')
		get<0>(pacPositions.at(index)) += 1;
}

void PacController::recombine(const ParsedList& parsedList, Tree& parent1, Tree& parent2)
{
	// Sub-TreeCrossover is the only option
	if (parsedList.recombination == "Sub-TreeCrossover")
	{
		// so do Sub-TreeCrossover
		validator.subTreeCross(parent1, parent2);
	}
}

void PacController::mutate(const ParsedList& parsedList, Tree& parent1)
{
	// Sub-TreeMutation is the only option
	if (parsedList.mutation == "Sub-TreeMutation")
	{
		// so do ub-TreeMutation
		validator.subTreeMutate(parent1);
	}
}
