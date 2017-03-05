#ifndef PARSEDLIST_H
#define PARSEDLIST_H

#include <iostream>
#include <string.h>

using namespace std;

class ParsedList
{
public:
     // solution output location
     string solution;
     // solution ghost output location
     string ghostSolutionDir;
     // log output location
     string output;
     // world file output location
     string worldFileOutput;
     // bool for timer initialized random seed
     bool timerInitialized;
     // random number generator seed
     int rngSeed;
     // number of runs
     int runs;
     // number of evals per run
     int evals;
     // height of the board
     int height;
     // width of the board
     int width;
     // probability of a pill being placed in any empty grid space
     float pillDensity;
     // wall density
     float wallDensity;
     // probability of a fruit being placed in any empty grid space
     float fruitSpawning;
     // time multiplier used for calculating time
     int timeMultiplier;
     // amount score increases when a fruit is eaten
     int fruitScore;

     // Pac-Man Data Structure
     string representation;
     // Initialization Algorithm
     string initialization;
     // Parent Selection Strategy
     string parentSelection;
     // Recombination Algorithm
     string recombination;
     // Mutation Algorithm
     string mutation;
     // Survival Selection Strategy
     string survival;
     // Bloat Control Strategy
     string bloatControl;
     // termination condition
     string termination;

     // Population Size
     int pacMu;
     // Number of individual offspring produced per generation
     int pacLambdaValue;
     // Survival Selection tournament size
     int pacK;
     // Parsimony Pressure penalty coefficient
     float pacP;
     // Number of Evaluations until termination
     int maxEvals;
     // Number of Generations before Convergence
     int n;
     // Maximum Tree depth
     int treeDepth;
     // Chance of Recombination Happening
     float recomChance;

     float topChosenPercent;
     float percentFromTop;

     // generate a parsed list from a configuration file location
     ParsedList(const string& configFile);

     // Ghost Data Structure
     string ghostRepresentation;
     // Ghost Initialization Algorithm
     string ghostInitialization;
     // Ghost Parent Selection Strategy
     string ghostParentSelection;
     // Ghost Recombination Algorithm
     string ghostRecombination;
     // Ghost Mutation Algorithm
     string ghostMutation;
     // Ghost Survival Selection Strategy
     string ghostSurvival;

     // Ghost Population Size
     int ghostMu;
     // Ghost Number of individual offspring produced per generation
     int ghostLambdaValue;
     // Ghost Survival Selection tournament size
     int ghostK;
     // Ghost Parsimony Pressure penalty coefficient
     float ghostP;

     // Whether or not each ghost has a unique controller
     bool uniqueGhost;
     // Number of pac per game
     int pacPerGame;
     // Whether or not each pac has a unique controller
     bool uniquePac;
     // ratio of turns for pac to ghost
     float speedRatio;

};

#endif
