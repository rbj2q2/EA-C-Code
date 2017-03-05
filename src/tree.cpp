#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <climits>
#include <queue>
#include <vector>

#include "tree.h"

using namespace std;

float Tree::getNodeValue(int index, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, const string& validMoves, int pacX, int pacY)
{
	// Add the values of children nodes
	if (root[index] == "+")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, pacX, pacY) + getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, pacX, pacY));
	}
	// Subtract the value of children nodes
	else if (root[index] == "-")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, pacX, pacY) - getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, pacX, pacY));
	}
	// Multiply the value of children nodes
	else if (root[index] == "*")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, pacX, pacY) * getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, pacX, pacY));
	}
	// Divide the value of children nodes
	else if (root[index] == "/")
	{
		float temp = getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, pacX, pacY);
		if (temp == 0)
		{
			temp = 0.000001;
		}
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, pacX, pacY) / temp);
	}
	// the Manhattan distance between Pac-Man and the nearest Ghost
	else if (root[index] == "ghost")
	{
		// Set minimum distance to maximum by default
		int ghostDistance = INT_MAX;
		// For all ghost controllers
		for (unsigned int i =0; i < ghosts.size(); i++)
		{
			// Sum the x and y distances
			int thisDistance = abs(pacX - get<0>(ghosts[i]));
			thisDistance += abs(pacY - get<1>(ghosts[i]));
			// If this distance is smaller, it is the new minimum
			if (thisDistance < ghostDistance)
				ghostDistance = thisDistance;
		}

		return static_cast<float>(ghostDistance);
	}
	// the Manhattan distance between Pac-Man and the nearest other pac
	else if (root[index] == "pac")
	{
		// node has not found itself
		bool foundSelf = false;
		// Finding the minimum distance between two pac, so set to largest value by default
		int pacDistance = INT_MAX;
		// For all pac
		for (unsigned int i = 0; i < pacPositions.size(); i++)
		{
			// Sum the x and y distances
			int thisDistance = abs(pacX - get<0>(pacPositions[i]));
			thisDistance += abs(pacY - get<1>(pacPositions[i]));
			// If there is at least one other pac at position, it could be self
			if (thisDistance == 0)
			{
				// If a pac has already been found at position, at least one isn't self, so it is valid
				if (foundSelf)
				{
					pacDistance = thisDistance;
				}
				// Otherwise, it could be this pac
				else
					foundSelf = true;
			}
			// If this distance is smaller, it is the new minimum
			else if (thisDistance < pacDistance)
				pacDistance = thisDistance;
		}

		// Return this value of node
		return static_cast<float>(pacDistance);
	}
	// the Manhattan distance between PacMan and nearest Pill
	else if (root[index] == "pill")
	{
		// If this position is on a pill, return 0
		if (board.map.at(pacY).at(pacX) == 'P')
			return 0;
		else
		{
			// 1st int is x, 2nd int is y, 3rd is narrow spread, 4th is direction
			vector < tuple <int, int, bool, char> > toBeChecked;
			// minimum distance when not on a pill is 1
			float distance = 1.00;
			// Check all four directions. Up and down will only recursively check up or down
			toBeChecked.push_back(make_tuple(pacX - 1, pacY, false, 'l'));
			toBeChecked.push_back(make_tuple(pacX + 1, pacY, false, 'r'));
			toBeChecked.push_back(make_tuple(pacX, pacY - 1, true, 'u'));
			toBeChecked.push_back(make_tuple(pacX, pacY + 1, true, 'd'));

			// while a pill hasn't been found
			while (true)
			{
				// Next generation of places to be checked
				vector < tuple <int, int, bool, char> > replacement;

				// Check all vectors of points
				for (unsigned int i = 0; i < toBeChecked.size(); i++)
				{
					// If we're not going off the board
					if ((get<0>(toBeChecked[i]) >= 0) && (get<0>(toBeChecked[i]) < board.boardWidth)
						&& (get<1>(toBeChecked[i]) >= 0) && (get<1>(toBeChecked[i]) < board.boardHeight))
					{
						// If this is a pill, return distance
						if (board.map.at(get<1>(toBeChecked[i])).at(get<0>(toBeChecked[i])) == 'P')
							return distance;
						// Otherwise
						else
						{
							// If this node only spreads vertically, add the next vertical vector in the given direction
							if (get<2>(toBeChecked[i]))
							{
								if (get<3>(toBeChecked[i]) == 'u')
								{
									replacement.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) - 1, true, 'u'));
								}
								else
								{
									replacement.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) + 1, true, 'd'));
								}
							}
							// Otherwise, add all positions away from the vertex. Hoizontal positions will continue to spead all directions away from vertex, vertical will only spread vertically
							else
							{
								if (get<3>(toBeChecked[i]) == 'l')
								{
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]) - 1, get<1>(toBeChecked[i]), false, 'l'));
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) - 1, true, 'u'));
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) + 1, true, 'd'));
								}
								else
								{
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]) + 1, get<1>(toBeChecked[i]), false, 'r'));
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) - 1, true, 'u'));
									toBeChecked.push_back(make_tuple(get<0>(toBeChecked[i]), get<1>(toBeChecked[i]) + 1, true, 'd'));
								}
							}
						}
					}
				}
				// If a pill isn't found, replace current generation with next
				toBeChecked = replacement;
				// Increment distance
				distance += 1.00;
			}
		}
	}
	// the Manhattan distance between PacMan and the fruit
	else if (root[index] == "fruit")
	{
		// If a fruit is on the board
		if (board.fruitOnBoard)
		{
			// Return absolute value distance
			int dist = abs(pacX - board.fruitX);
			dist += abs(pacX - board.fruitY);
			return static_cast<float>(dist);
		}
		// Default to 0 if no fruit is on the board
		else
			return 0.00;
	}
	// the number of walls immediately adjacent to Pac-Man
	else if (root[index] == "walls")
	{
		// Return number of max valid number of moves minus valid moves
		return static_cast<float>(5 - validMoves.length());
	}
	// Get a random number between -10 and 10
	else if  (root[index] == "rand")
	{
		float leftNum = -10.00;
		float rightNum =10.00;
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = rightNum - leftNum;
		float r = random * diff;
		return leftNum + r;
	}
	else
	{
		// Return floating point value of node
		return stof(root[index]);
	}
}

void Tree::initialize(const ParsedList& parsedList)
{
	// Initialize with Ramped Half-and-Hald method
	if (parsedList.initialization == "Ramped")
	{
		// Flip a coin
		int decision = rand() % 2;
		if (decision == 0)
		{
			//grow
			grow(0);
		}
		else
		{
			//full
			full(0, 1, parsedList.treeDepth);
		}
	}
}

void Tree::grow(unsigned int index)
{
	// Increase size if vector is too small
	if (root.size() < index + 1)
	{
		root.resize(index + 1, "");
	}

	// If maximum depth hasn't been reached
	if (index < 511)
	{
		// Choose a random sensor or operator
		int chosen = rand() % 11;

		switch (chosen)
		{
		case 0:
			// Add operator
			root[index] = "+";
			// get value for children
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 1:
			// Subtraction operator
			root[index] = "-";
			// get value for children
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 2:
			// Division of operator
			root[index] = "/";
			// get value for children
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 3:
			// Multiplication of operator
			root[index] = "*";
			// get value for children
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 4:
			// Minimum distance to ghost sensor
			root[index] = "ghost";
			break;
		case 5:
			// Minimum distance to other pac
			root[index] = "pac";
			break;
		case 6:
			// Distance to nearest pill sensor
			root[index] = "pill";
			break;
		case 7:
			// Distance to fruit sensor
			root[index] = "fruit";
			break;
		case 8:
			// Number of walls sensor
			root[index] = "walls";
			break;
		case 9:
			// Random number
			root[index] = "rand";
			break;
		case 10:
			// Static pre-determined number generated randomly between -10 and 10
			float leftNum = -10.00;
			float rightNum =10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
	// Choose a random sensor
	else
	{
		int chosen = rand() % 7;

		switch (chosen)
		{
		case 0:
			// Minimum distance to ghost sensor
			root[index] = "ghost";
			break;
		case 1:
			// Minimum distance to other pac
			root[index] = "pac";
			break;
		case 2:
			// Distance to nearest pill sensor
			root[index] = "pill";
			break;
		case 3:
			// Distance to fruit sensor
			root[index] = "fruit";
			break;
		case 4:
			// Number of walls sensor
			root[index] = "walls";
			break;
		case 5:
			// Random number
			root[index] = "rand";
			break;
		case 6:
			// Static pre-determined number generated randomly between -10 and 10
			float leftNum = -10.00;
			float rightNum = 10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
}

void Tree::full(unsigned int index, int depth, int maxDepth)
{
	// Increase size if vector is too small
	if (root.size() < index + 1)
	{
		root.resize(index + 1, "");
	}

	// If maximum depth hasn't been reached, this node is an operator
	if (depth < maxDepth)
	{
		int chosen = rand() % 4;
		switch (chosen)
		{
		case 0:
			// Add operator
			root[index] = "+";
			break;
		case 1:
			// Subtraction operator
			root[index] =  "-";
			break;
		case 2:
			// Division operator
			root[index] =  "/";
			break;
		case 3:
			// Multiplication operator
			root[index] = "*";
			break;
		}
		// get value for children
		full(2*index + 1, depth + 1, maxDepth);
		full(2*index + 2, depth + 1, maxDepth);
	}
	// Otherwise, it is a sensor node
	else
	{
		int chosen = rand() % 7;
		switch (chosen)
		{
		case 0:
			// Distance to nearest ghost
			root[index] = "ghost";
			break;
		case 1:
			// Distance to nearest other pac
			root[index] = "pac";
			break;
		case 2:
			// Distance to nearest pill
			root[index] = "pill";
			break;
		case 3:
			// Distance to fruit
			root[index] = "fruit";
			break;
		case 4:
			// Number of walls
			root[index] = "walls";
			break;
		case 5:
			// Random number
			root[index] = "rand";
			break;
		case 6:
			// Static pre-determined number generated randomly between -10 and 10
			float leftNum = -10.00;
			float rightNum = 10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
}

void Tree::copy(unsigned int indexOfNew, int indexOfCopy, const vector <string>& toBeCopied)
{
	// If this vector is too small, it is enlarged
	if (root.size() < indexOfNew + 1)
	{
		root.resize(indexOfNew + 1, "");
	}

	// Copy the node at given value to value of chosen position in current tree
	root.at(indexOfNew) = toBeCopied.at(indexOfCopy);

	// If this node isn't a terminal
	if ((root.at(indexOfNew) == "+") || (root.at(indexOfNew) == "-") || (root.at(indexOfNew) == "/") || (root.at(indexOfNew) == "*"))
	{
		// Recursively copy children nodes
		copy(2*indexOfNew + 1, 2*indexOfCopy + 1, toBeCopied);
		copy(2*indexOfNew + 2, 2*indexOfCopy + 2, toBeCopied);
	}
}

void Tree::subTreeCross(Tree& parent1, Tree& parent2)
{
	// Choose two random indices in parents, aka crossover points
	int chosen1 = breadthFirstRandom(parent1.root);
	int chosen2 = breadthFirstRandom(parent2.root);

	// Choose which parent is the base via coin flip
	int coinFlip = rand() % 2;
	if (coinFlip == 0)
	{
		// Copy parent 1 as base, copy, parent 2 at crossover points
		copy(0, 0, parent1.root);
		copy(chosen1, chosen2, parent2.root);
	}
	else
	{
		// Copy parent 2 as base, copy, parent 1 at crossover points
		copy(0, 0, parent2.root);
		copy(chosen2, chosen1, parent1.root);
	}

	////////////////////////////////////////////////////////////////////////////
	// The size must be manually controlled due to memory constraints
	// Parsimony Pressure will not necessarily control the size of populations
	// early on in the evolutionary process
	////////////////////////////////////////////////////////////////////////////
	if (root.size() > 1023)
	{
		root.resize(1023);
		for (int i = 511; i < 1023; i++)
		{
			// Change all non-terminals at max-depth to terminals
			if ((root.at(i) == "+") || (root.at(i) == "-") || (root.at(i) == "/") || (root.at(i) == "*"))
			{
				// Random non-terminal
				int chosen = rand() % 7;
				switch (chosen)
				{
				case 0:
					// Minimum distance to other ghost sensor
					root[i] = "ghost";
					break;
				case 1:
					// Minimum distance to pac sensor
					root[i] = "pac";
					break;
				case 2:
					// Minimum distance to pill sensor
					root[i] = "pill";
					break;
				case 3:
					// Minimum distance to fruit sensor
					root[i] = "fruit";
					break;
				case 4:
					// Number of walls sensor
					root[i] = "walls";
					break;
				case 5:
					// Random number
					root[i] = "rand";
					break;
				case 6:
					// Static Pre-determined number determined randomly
					float leftNum = -10.00;
					float rightNum = 10.00;
					float random = ((float) rand()) / (float) RAND_MAX;
					float diff = rightNum - leftNum;
					float r = random * diff;
					root[i] = to_string(leftNum + r);
					break;
				}
			}
		}
	}
}

void Tree::subTreeMutate(Tree& parent1)
{
	// Copy a parent
	copy(0, 0, parent1.root);

	// Choose a random point
	int chosen = breadthFirstRandom(parent1.root);

	// Mutate from crossover point onwards
	grow(chosen);
}

int Tree::breadthFirstRandom(vector <string>& finding)
{
	// First, clear all garbage. If parent of a node isn't a non-terminal, it's garbage
	for (unsigned int i = 1; i < finding.size(); i++)
	{
		if ((finding[floor((i - 1) / 2)] != "+") && (finding[floor((i - 1) / 2)] != "-") && (finding[floor((i - 1) / 2)] != "/") && (finding[floor((i - 1) / 2)] != "*"))
		{
			finding[i] = "";
		}
	}

	// Choose a random position within size
	int randomChosen = rand() % finding.size();
	while (finding[randomChosen] == "")
	{
		// If the chosen index is garbage, try again
		randomChosen = rand() % finding.size();
	}

	return randomChosen;
}
