#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <climits>
#include <queue>

#include "ghostTree.h"

using namespace std;

float GhostTree::getNodeValue(int index, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, const string& validMoves, int myX, int myY)
{
	// Add the values of children nodes
	if (root[index] == "+")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, myX, myY) + getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, myX, myY));
	}
	// Subtract the value of children nodes
	else if (root[index] == "-")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, myX, myY) - getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, myX, myY));
	}
	// Multiply the value of children nodes
	else if (root[index] == "*")
	{
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, myX, myY) * getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, myX, myY));
	}
	// Divide the value of children nodes
	else if (root[index] == "/")
	{
		float temp = getNodeValue(2*(index) + 2, board, ghosts, pacPositions, validMoves, myX, myY);
		if (temp == 0)
		{
			temp = 0.000001;
		}
		return (getNodeValue(2*(index) + 1, board, ghosts, pacPositions, validMoves, myX, myY) / temp);
	}
	// the Manhattan distance between self and the nearest Ghost
	else if (root[index] == "ghost")
	{
		// node has not found itself
		bool foundGhostAtMe = false;
		// Finding the minimum distance between two ghosts, so set to largest value by default
		int ghostDistance = INT_MAX;
		// For all ghosts
		for (unsigned int i = 0; i < ghosts.size(); i++)
		{
			// Sum the x and y distances
			int thisDistance = abs(myX - get<0>(ghosts[i]));
			thisDistance += abs(myY - get<1>(ghosts[i]));
			// If there is at least one other ghost at position, it could be self
			if (thisDistance == 0)
			{
				// If a ghost has already been found at position, at least one isn't self, so it is valid
				if (foundGhostAtMe)
				{
					ghostDistance = thisDistance;
				}
				// Otherwise, it could be this ghost
				else
					foundGhostAtMe = true;
			}
			// If this distance is smaller, it is the new minimum
			else if (thisDistance < ghostDistance)
				ghostDistance = thisDistance;
		}

		// Return this value of node
		return static_cast<float>(ghostDistance);
	}
	// the Manhattan distance between Pac-Man and the nearest Ghost
	else if (root[index] == "pac")
	{
		// Set minimum distance to maximum by default
		int pacDistance = INT_MAX;
		// For all pac controllers
		for (unsigned int i = 0; i < pacPositions.size(); i++)
		{
			// Sum the x and y distances
			int thisDistance = abs(myX - get<0>(pacPositions[i]));
			thisDistance += abs(myY - get<1>(pacPositions[i]));
			// If this distance is smaller, it is the new minimum
			if (thisDistance < pacDistance)
				pacDistance = thisDistance;
		}

		// return minimum distance to a pac
		return static_cast<float>(pacDistance);
	}
	else if  (root[index] == "rand")
	{
		// get a random float between -10 and 10
		float leftNum = -10.00;
		float rightNum =10.00;
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = rightNum - leftNum;
		float r = random * diff;
		return leftNum + r;
	}
	else
	{
		// return float value of this node
		return stof(root[index]);
	}
}

void GhostTree::initialize(const ParsedList& parsedList)
{
	if (parsedList.ghostInitialization == "Ramped")
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

void GhostTree::grow(unsigned int index)
{
	// If this tree is too small to fit the entire tree, enlarge it to fit
	if (root.size() < index + 1)
	{
		root.resize(index + 1, "");
	}

	// If the tree hasn't gone over the height limit
	if (index < 511)
	{
		// This node can be any of sensors or operators
		int chosen = rand() % 8;

		switch (chosen)
		{
		case 0:
			// add operator
			root[index] = "+";
			// get children values
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 1:
			// subtraction operator
			root[index] = "-";
			// get children values
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 2:
			// Division operator
			root[index] = "/";
			// get children values
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 3:
			// Multiplication operator
			root[index] = "*";
			// get children values
			grow(2*index + 1);
			grow(2*index + 2);
			break;
		case 4:
			// Distance to ghost sensor
			root[index] = "ghost";
			break;
		case 5:
			// Distance to pac sensor
			root[index] = "pac";
			break;
		case 6:
			// Random number each time
			root[index] = "rand";
			break;
		case 7:
			// Static randomly pre-determined number between -10 and 10
			float leftNum = -10.00;
			float rightNum =10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
	else
	{
		// Only terminals are allowed
		int chosen = rand() % 4;

		switch (chosen)
		{
		case 0:
			// Distance to ghost sensor
			root[index] = "ghost";
			break;
		case 1:
			// Distance to pac sensor
			root[index] = "pac";
			break;
		case 2:
			// Random number each time
			root[index] = "rand";
			break;
		case 3:
			// Static randomly pre-determined number between -10 and 10
			float leftNum = -10.00;
			float rightNum =10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
}

void GhostTree::full(unsigned int index, int depth, int maxDepth)
{
	// Increase size if the current size won't fit
	if (root.size() < index + 1)
	{
		root.resize(index + 1, "");
	}

	// Unless the maximum depth has been reached, an operator is chosen
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
			// Subtraction operator is used
			root[index] =  "-";
			break;
		case 2:
			// Division operator is used
			root[index] =  "/";
			break;
		case 3:
			// Multiplication operator is used
			root[index] = "*";
			break;
		}
		// Get value of both children nodes
		full(2*index + 1, depth + 1, maxDepth);
		full(2*index + 2, depth + 1, maxDepth);
	}
	// A terminal is used at maximum depth
	else
	{
		int chosen = rand() % 4;
		switch (chosen)
		{
		case 0:
			// Minimum distance to other ghost sensor
			root[index] = "ghost";
			break;
		case 1:
			// Minimum distance to pac sensor
			root[index] = "pac";
			break;
		case 2:
			// Random number
			root[index] = "rand";
			break;
		case 3:
			// Randomly pre-determined number between -10 and 10
			float leftNum = -10.00;
			float rightNum =10.00;
			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = rightNum - leftNum;
			float r = random * diff;
			root[index] = to_string(leftNum + r);
			break;
		}
	}
}

void GhostTree::copy(unsigned int indexOfNew, int indexOfCopy, const vector <string>& toBeCopied)
{
	// Increase size of tree if it's too small to fit
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

void GhostTree::subTreeCross(GhostTree& parent1, GhostTree& parent2)
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
				int chosen = rand() % 4;
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
					// Random number
					root[i] = "rand";
					break;
				case 3:
					// Static Pre-determined number determined randomly
					float leftNum = -10.00;
					float rightNum =10.00;
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

void GhostTree::subTreeMutate(GhostTree& parent1)
{
	// Copy a parent
	copy(0, 0, parent1.root);

	// Choose a random point
	int chosen = breadthFirstRandom(parent1.root);

	// Mutate from crossover point onwards
	grow(chosen);
}

int GhostTree::breadthFirstRandom(vector <string>& finding)
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
