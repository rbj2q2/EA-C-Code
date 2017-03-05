#ifndef PAC_CONTROLLER_H
#define PAC_CONTROLLER_H

#include <iostream>
#include <vector>
#include "tree.h"

using namespace std;

class PacController
{
public:
     // Default pac controller constructor
     PacController();
     // Pac Controller created from configuration file
     PacController(const ParsedList& parsedList);
     // Pac Controller copy constructor
     PacController(const PacController& toBeCopied);
     // Tree repsentation of controller
     Tree validator;
     // Sum of all scores from all games this generation
     float sumAllGameScores;
     // Number of games participated in this generation
     float numberOfGames;

     // Store the score of this Pac-Man controller
     float score;

     // From a set of valid moves, determine next move based on chosen algorithm
     void chooseMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index);

     // Choose a random valid move
     void randomMove(const string& validMoves, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index);
     // Choose move from tree
     void treeMove(const string& validMoves, Board& board, vector < tuple<int,int,int,int,int> >& ghosts, vector < tuple<int,int,int,int,int,bool> >& pacPositions, int index);
     // Create this tree from recombination of two parents
     void recombine(const ParsedList& parsedList, Tree& parent1, Tree& parent2);
     // Create this tree by mutating a parent
     void mutate(const ParsedList& parsedList, Tree& parent1);

};

#endif
