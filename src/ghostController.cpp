#include <iostream>
#include <vector>
#include <string>
#include <float.h>
#include "ghostController.h"

using namespace std;

GhostController::GhostController()
{
	// Set score to 0
	score = 0;

	// Set total scores of all games to 0
	sumAllGameScores = 0;
	// Set number of games to 0
     numberOfGames = 0;
}

GhostController::GhostController(const ParsedList& parsedList)
{
	// Make a tree for this controller
	validator.initialize(parsedList);
	// Set score to 0
	score = 0;

	// Set total scores of all games to 0
	sumAllGameScores = 0;
	// Set number of games to 0
     numberOfGames = 0;
}

GhostController::GhostController(const GhostController& toBeCopied)
{
	// Copy the tree
	validator.copy(0, 0, toBeCopied.validator.root);
	// Copy the score
	score = toBeCopied.score;

	// Copy the total scores of all games
	sumAllGameScores = toBeCopied.sumAllGameScores;
	// Copy the number of games played
     numberOfGames = toBeCopied.numberOfGames;
}

void GhostController::chooseMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index)
{
	// Set previous y position to current y position
	get<3>(ghosts.at(index)) = get<1>(ghosts.at(index));
	// Set previous x position to current y position
	get<2>(ghosts.at(index)) = get<0>(ghosts.at(index));

	treeMove(validMoves, board, ghosts, pacPositions, index);
}

void GhostController::randomMove(const string& validMoves, vector < tuple<int,int,int,int,int> >& ghosts, int index)
{
	// Set previous y position to current y position
	get<3>(ghosts.at(index)) = get<1>(ghosts.at(index));
	// Set previous x position to current y position
	get<2>(ghosts.at(index)) = get<0>(ghosts.at(index));
	// Choose a random position of the validmove string
	int chosenDigit = rand()%validMoves.length();
	// get the char of the position in the string that was chosen randomly
	char selectedMove = validMoves[chosenDigit];

	// perform the move action
	// u stands for up
	if (selectedMove == 'u')
		get<1>(ghosts.at(index)) -= 1;
	// d stands for down
	else if (selectedMove == 'd')
		get<1>(ghosts.at(index)) += 1;
	// l stands for left
	else if (selectedMove == 'l')
		get<0>(ghosts.at(index)) -= 1;
	// only other valid move is right
	else
		get<0>(ghosts.at(index)) += 1;
}

void GhostController::treeMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index)
{
	// Best value of move is the minimum value
	float valueOfBestMove = -1.00 * (FLT_MAX);
	// There is no best move
	char bestMove = '\0';

	for (unsigned int i = 0; i < validMoves.length(); i++)
	{
		// Store the value of a given sensor in this variable
		float temp = 0.00;

		// Calculates value of space above
		if (validMoves.at(i) == 'u')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(ghosts[index]), get<1>(ghosts[index]) - 1);
		// Calculates value of space below
		else if (validMoves.at(i) == 'd')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(ghosts[index]), get<1>(ghosts[index]) + 1);
		// Calculates value of space to the left
		else if (validMoves.at(i) == 'l')
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(ghosts[index]) - 1, get<1>(ghosts[index]));
		// Calculates value of space to the right
		else
			temp = validator.getNodeValue(0, board, ghosts, pacPositions, validMoves, get<0>(ghosts[index]) + 1, get<1>(ghosts[index]));

		// If a given value is better than the current best move
		if (temp >= valueOfBestMove)
		{
			// Set the given value as the best value
			valueOfBestMove = temp;
			// Set the given direction of the best move
			bestMove = validMoves.at(i);
		}
	}

	// u stands for up
	if (bestMove == 'u')
		get<1>(ghosts.at(index)) -= 1;
	// d stands for down
	else if (bestMove == 'd')
		get<1>(ghosts.at(index)) += 1;
	// l stands for left
	else if (bestMove == 'l')
		get<0>(ghosts.at(index)) -= 1;
		// r stands for right
	else if (bestMove == 'r')
		get<0>(ghosts.at(index)) += 1;
}

void GhostController::recombine(const ParsedList& parsedList, GhostTree& parent1, GhostTree& parent2)
{
	// Only one type of recombination is defined
	if (parsedList.ghostRecombination == "Sub-TreeCrossover")
	{
		// So do that kind of recombination
		validator.subTreeCross(parent1, parent2);
	}
}

void GhostController::mutate(const ParsedList& parsedList, GhostTree& parent1)
{
	// Only one type of mutation is defined
	if (parsedList.ghostMutation == "Sub-TreeMutation")
	{
		// So do that kind of mutation
		validator.subTreeMutate(parent1);
	}
}
