#ifndef GHOST_CONTROLLER_H
#define GHOST_CONTROLLER_H

#include <iostream>
#include <vector>
#include <string>
#include "ghostTree.h"

using namespace std;

class GhostController
{
public:
     // Default Constructor
     GhostController();
     // Initialize ghost to position of lower right corner of the map
     GhostController(const ParsedList& parsedList);
     //Copy Constructor
     GhostController(const GhostController& toBeCopied);

     // This controller has its own tree
     GhostTree validator;

     // Sum of scores from all games
     float sumAllGameScores;
     // Total number of games for a given generation
     float numberOfGames;
     // Score for a given generation
     float score;

     // From a set of valid moves, determine next move based on chosen algorithm
     void chooseMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index);

     // Choose a random valid move
     void randomMove(const string& validMoves, vector < tuple<int,int,int,int,int> >& ghosts, int index);
     // Choose the best move based on the validator tree
     void treeMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index);
     // Recombine to form own tree
     void recombine(const ParsedList& parsedList, GhostTree& parent1, GhostTree& parent2);
     // Mutate a parent to find own tree
     void mutate(const ParsedList& parsedList, GhostTree& parent1);
};

#endif
