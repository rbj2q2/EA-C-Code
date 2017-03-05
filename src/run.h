#ifndef RUN_H
#define RUN_H

#include <iostream>
#include <string>
#include <vector>
#include "game.h"

using namespace std;

class Run
{
public:

     // Initialize anything that needs initializing
     Run(bool timer, int rngSeed);
     // Address of pac solution file
     string solution;
     // Address of ghost solution file
     string ghostSolution;

     float bestGhost;
     // Stores the current evaluation being performed
     int evalNum;
     // Stores the world file string related to the run high score
     string bestWorld;
     string secondBest;
     string thirdBest;
     // Store the three highest Scores
     float globalBest;
     float globalSecond;
     float globalThird;
     // Keeps track of whether or not a score needs to be recorded
	float highScore;
	// Keeps track of whether any termination condition has been mentioned
	bool ended;
	// Initialize the world file to empty
	string textOutput;
     // Population of pac-men
     vector <PacController> population;
     // Population of ghosts
     vector <GhostController> ghostPopulation;

     // Perform the number of runs specified in configuration file
     void performRuns(const ParsedList& parsedList, const Log& output);
     // Perform the number of evaluations specified in configuration file
     void performEvals(const ParsedList& parsedList, const Log& output, int run);

     // Seed the random number generator
     void seedRandom(bool timer, int rngSeed);
     // Evaluate the entire population
     void evaluatePopulation(int run, const ParsedList&  parsedList);

     // Initialize the population at the beginning of a run
     void initializePopulation(const ParsedList&  parsedList);
     // Offspring generation
     void makeOffspring(const ParsedList&  parsedList, int run);
     // Survival Selection
     void competition(const ParsedList&  parsedList);
     // Return an index based on proportional fitness of pac controllers
     int fitnessProportionalIndex(const vector <PacController>& testPopulation);
     // Return an index based on proportional fitness of ghosts
     int ghostProportionalIndex(vector <GhostController>& testPopulation);
     // Determine whether recombination or mutation will be used for a pac controller
     void determineRecOrMut(PacController& child, const ParsedList& parsedList, vector <PacController>& parentPopulation, int childType);
     // Determine whether recombination or mutation will be used for a ghost controller
     void ghostRecOrMut(GhostController& child, const ParsedList& parsedList, vector <GhostController>& parentPopulation, int childType);
     // evaluate a given pool of individuals
     void evaluate(vector <tuple <int, bool> >& evaluationPacs, vector <tuple <int, bool> >& evaluationGhosts, int run, const ParsedList& parsedList);
     // Check if the run is over
     bool checkTermination(const ParsedList& parsedList, int& stagnant, float& maxScore);

};

#endif
