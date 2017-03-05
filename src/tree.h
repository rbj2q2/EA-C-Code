#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <vector>
#include "ghostController.h"
#include <tuple>

using namespace std;

class Tree
{
public:
     // Tree of value for a position is stored in a vector
     vector <string> root;

     // Return the value of a node recursively
     float getNodeValue(int index, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, const string& validMoves, int pacX, int pacY);
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
     void subTreeCross(Tree& parent1, Tree& parent2);
     // Mutate a subtree
     void subTreeMutate(Tree& parent1);
};

#endif
