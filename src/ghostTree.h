#ifndef GHOST_TREE_H
#define GHOST_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include "board.h"
#include "parsedList.h"
#include <tuple>

using namespace std;

class GhostTree
{
public:
     // Tree of value for a position is stored in a vector
     vector <string> root;

     // Return the value of a node recursively
     float getNodeValue(int index, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, const string& validMoves, int myX, int myY);
     // Initialize tree from configuration file
     void initialize(const ParsedList& parsedList);
     // Grow method for tree
     void grow(unsigned int index);
     // A full tree is made
     void full(unsigned int index, int depth, int maxDepth);
     // Copy a given tree
     void copy(unsigned int indexOfNew, int indexOfCopy, const vector <string>& toBeCopied);
     // Choose a random position from a tree by breadth
     int breadthFirstRandom(vector <string>& finding);

     // Crossover a subtree
     void subTreeCross(GhostTree& parent1, GhostTree& parent2);
     // Mutate a subtree
     void subTreeMutate(GhostTree& parent1);
};

#endif
