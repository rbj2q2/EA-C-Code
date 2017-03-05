#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "pacController.h"
#include "log.h"

using namespace std;

class Game
{
public:
     // Stored width of the board
     int width;
     // Stored height of the board
     int height;
     // Stored score of the fruit
     int fruitScore;
     // Maximum time alotted for gameplay
     int baseTime;
     // Stored rate of fruit spawning
     float fruitSpawning;

     // Current Time left
     int currentTime;
     // Current number of pills collected
     int currentPills;
     // Current score obtained from gathering fruit
     int currentFruitTotal;
     // Whether or not an ending condition has caused the game to end
     bool playing;
     // Keep track of Pac score without parsimony pressure
     float sumScore;

     // Initialize stored variables from parsed list, calculate derived values and add ghosts to vector
     Game(const ParsedList& parsedList);

     // Run the game and return the ending score
     void runGame(const ParsedList& parsedList, string& textOutput, vector <tuple <int, bool> >& evaluationPacs, vector <tuple <int, bool> >& evaluationGhosts, vector <PacController>& population, vector <GhostController>& ghostPopulation);
     // Update pac-man and ghost positions, check for ending criteria, re-calculate score
     void updateGame(int maxPills, Board& board, int currentTime, bool first, string& textOutput, vector < tuple<int,int,int,int,int,bool> >& vectorOfPacs, vector < tuple<int,int,int,int,int> >& vectorOfGhosts);

     // Returns string of valid moves available from given position
     string validPosition(string valid, int xPos, int yPos, Board board);

     // Record world state
     void worldFileSetting(bool first, int currentTime, bool fruitPlaced, int fruitX, int fruitY, Board& board, string& textOutput, vector < tuple<int,int,int,int,int,bool> >& vectorOfPacs, vector < tuple<int,int,int,int,int> >& vectorOfGhosts);

};

#endif
