#include <iostream>
#include <ctime>
#include <algorithm>
#include <stdlib.h>
#include <float.h>

#include "run.h"

using namespace std;

Run::Run(bool timer, int rngSeed)
{
	// Randomly generate the seed based on configuration file
	seedRandom(timer, rngSeed);
}

void Run::performRuns(const ParsedList& parsedList, const Log& output)
{
	bestWorld = "";
     secondBest = "";
     thirdBest = "";
     // Store the three highest Scores
     globalBest = -1.00 * (FLT_MAX);
     globalSecond = -1.00 * (FLT_MAX);
     globalThird  = -1.00 * (FLT_MAX);
	// Perform the number of runs specified in the configuration file
	for (int runNum=1; runNum <= parsedList.runs; runNum++)
	{
		// Perform evaluations for each run
		performEvals(parsedList, output, runNum);
		// Send best pac controller to be logged
		output.updateSolution(solution);
		// Send best ghost controller to be logged
		output.updateGhostSolution(ghostSolution);
	}
	// Log the world file of the 3 best scores
	output.updateWorldFile(bestWorld);
	output.updateWorldFile(secondBest);
	output.updateWorldFile(thirdBest);
}

void Run::seedRandom(bool timer, int rngSeed)
{
	// If we're using time initialized seed
	if (timer == true)
		// Seed the random number generator with the current time
		srand(time(0));
	else
		// Otherwise, seed the random number generator with the seed specified in the configuration file
		srand(rngSeed);
}

void Run::performEvals(const ParsedList& parsedList, const Log& output, int run)
{
	// Label the run
	output.logLabelRun(run);
	// Keeps track of whether or not a pac score needs to be recorded
	highScore = -1.00 * (FLT_MAX);
	// Keeps track of whether or not a ghost score needs to be recorded
	bestGhost = -1.00 * (FLT_MAX);
	// Keeps track of whether any termination condition has been mentioned
	ended = false;
	// Set Evaluation to first evaluation
	evalNum = 1;
	// Initialize the world file to empty
	textOutput = "";
	// Initialize best pac to empty string
     solution = "";
	// Initialize best ghost to empty string
     ghostSolution = "";
	// Initialize best world to empty
	bestWorld = "";
	// Number of stagnant generations set to 0
	int stagnantGenerations = 0;
	// Initialize the population
	initializePopulation(parsedList);

	// While an ending condition hasn't been reached
	while (!ended)
	{
		// Evaluate current population
		evaluatePopulation(run, parsedList);
		// Sum of population score is set to 0
		float sum = 0;

		// Scores of all population members are summed
		for (unsigned int i = 0; i < population.size(); i++)
		{
			sum += population[i].score;
		}

		// Average score is calculated
		float average = sum / population.size();

		// Update the log
		output.updateLog(evalNum - 1, average, highScore);

		// highest score population is set to minimum floating point number
		highScore = -1.00 * (FLT_MAX);

		// Offspring are created
		makeOffspring(parsedList, run);
		// Survival Selection
		competition(parsedList);
		// Check if a termination condition has been reached
		ended = checkTermination(parsedList, stagnantGenerations, highScore);
	}

	// Clear the pac population
	while (population.size() > 0)
	{
		population.clear();
	}

	// Clear the ghost population
	while (ghostPopulation.size() > 0)
	{
		ghostPopulation.clear();
	}

	// Output an endline for style
	output.logEndLine();
}

void Run::evaluatePopulation(int run, const ParsedList& parsedList)
{
	// Default set numbers for tracking number of individuals that need to be evaluated
	int numGhostsEvaluated = 0;
	int numPacsEvaluated = 0;
	int ghostPerRun = 1;
	int pacsPerRun = 1;

	// Set number of ghost controllers evaluated per game
	if (parsedList.uniqueGhost == true)
	{
		ghostPerRun = 3;
	}
	// Set number of pac controllers evaluated per game
	if (parsedList.uniquePac == true)
	{
		pacsPerRun = parsedList.pacPerGame;
	}

	// While the number of individuals evaluated has not reached its quota
	while ((numPacsEvaluated < parsedList.pacMu) || (numGhostsEvaluated < parsedList.ghostMu))
	{
		// Store vectors with integers as references to positions in vectors, bool is used by the game object
		vector <tuple <int, bool> > evaluationGhosts;
		vector <tuple <int, bool> > evaluationPacs;

		// for each ghost controller per run
		for (int i = 0; i < ghostPerRun; i++)
		{
			// If we haven't reached the ghost controller quota
			if (numGhostsEvaluated < parsedList.ghostMu)
			{
				// Set score sum and number of games to 0
				ghostPopulation[numGhostsEvaluated].sumAllGameScores = 0;
				ghostPopulation[numGhostsEvaluated].numberOfGames = 0;
				// Increment number of unique ghost controllers evaluated after adding this ghost to pool for evaluation
				evaluationGhosts.push_back(make_tuple(numGhostsEvaluated, true));
				numGhostsEvaluated += 1;
			}
			// Otherwise
			else
			{
				// choose a random ghost and use it for evaluation
				int randomGhost = rand() % parsedList.ghostMu;
				evaluationGhosts.push_back(make_tuple(randomGhost, true));
			}
		}

		// For the number of pac controllers per game
		for (int i = 0; i < pacsPerRun; i++)
		{
			// If the quota for pac controllers hasn't been reached
			if (numPacsEvaluated < parsedList.pacMu)
			{
				// Set this sum score and number of games to 0
				population[numPacsEvaluated].sumAllGameScores = 0;
				population[numPacsEvaluated].numberOfGames = 0;
				// Add this controller to the pool for evaluation and increment number evaluated
				evaluationPacs.push_back(make_tuple(numPacsEvaluated, true));
				numPacsEvaluated += 1;
			}
			// Otherwise
			else
			{
				// choose a random controller and add it to pool for evaluation
				int randomPac = rand() % parsedList.pacMu;
				evaluationPacs.push_back(make_tuple(randomPac, true));
			}
		}

		//evaluate
		evaluate(evaluationPacs, evaluationGhosts, run, parsedList);
	}
}

void Run::initializePopulation(const ParsedList& parsedList)
{
	// Set number of common controllers
	int baseInit = parsedList.ghostMu;
	// Default smaller number of ghost controllers
	bool lowerIsGhost = true;
	// If number of pac controllers per generation
	if (parsedList.pacMu < parsedList.ghostMu)
	{
		// Set base to lowest number and set flag to false
		baseInit = parsedList.pacMu;
		lowerIsGhost = false;
	}

	// Append mu pac-men to the vector of pac-men controller
	for (int numIndividuals = 0; numIndividuals < baseInit; numIndividuals++)
	{
		// Make a new ghost
		GhostController newGhost(parsedList);
		// make a new pac
		PacController newPac(parsedList);
		// add pac to population
		population.push_back(newPac);
		// add ghost to population
		ghostPopulation.push_back(newGhost);
	}
	// If smaller number of ghosts are used
	if (lowerIsGhost)
	{
		unsigned int usedMu = parsedList.pacMu;

		// until number of population members for pac is reached
		while (population.size() < usedMu)
		{
			// Make a new pac and add it to population
			PacController newPac(parsedList);
			population.push_back(newPac);
		}
	}
	// otherwise
	else
	{
		unsigned int usedMu = parsedList.ghostMu;

		// until number of population members for ghost is reached
		while (ghostPopulation.size() < usedMu)
		{
			// Make a new ghost and add it to population
			GhostController newGhost(parsedList);
			ghostPopulation.push_back(newGhost);
		}
	}
}

void Run::makeOffspring(const ParsedList& parsedList, int run)
{
	// Bracket created for both populations
	vector <PacController> topBracket;
	vector <PacController> lowerBracket;
	vector <GhostController> ghostTopBracket;
	vector <GhostController> ghostLowerBracket;

	// Set number of individuals evaluated to be only parents
	int numGhostsEvaluated = parsedList.pacMu;
	int numPacsEvaluated = parsedList.ghostMu;
	// Default, set number of controllers per game to 1 each
	int ghostPerRun = 1;
	int pacsPerRun = 1;

	// If unique ghost controllers are used
	if (parsedList.uniqueGhost == true)
	{
		// Set number of ghost controllers per run to 3
		ghostPerRun = 3;
	}
	// If unique pac controllers are used
	if (parsedList.uniquePac == true)
	{
		// Set number of pac controllers per run to 3
		pacsPerRun = parsedList.pacPerGame;
	}

	// If overselection is used for pac controllers
	if (parsedList.parentSelection == "Over-Selection")
	{
		// Sort the population of pac controllers by score
		sort(population.begin(), population.end(),  [](PacController s1, PacController s2){ return s1.score > s2.score;});
		// for all pac population members
		for (unsigned int i = 0; i < population.size(); i++)
		{
			// If this member belongs in the top bracket
			if (i < (parsedList.topChosenPercent * population.size()))
			{
				// put it in the top bracket
				topBracket.push_back(population.at(i));
			}
			// Otherwise put it in the bottom bracket
			else
			{
				lowerBracket.push_back(population.at(i));
			}
		}
	}
	// If overselection is used for ghost controllers
	if (parsedList.ghostParentSelection == "Over-Selection")
	{
		// Sort the population of ghost controllers by score
		sort(ghostPopulation.begin(), ghostPopulation.end(),  [](GhostController s1, GhostController s2){ return s1.score > s2.score;});
		// for all ghost population members
		for (unsigned int i = 0; i < ghostPopulation.size(); i++)
		{
			// If this member belongs in the top bracket
			if (i < (parsedList.topChosenPercent * ghostPopulation.size()))
			{
				// put it in the top bracket
				ghostTopBracket.push_back(ghostPopulation.at(i));
			}
			// Otherwise put it in the bottom bracket
			else
			{
				ghostLowerBracket.push_back(ghostPopulation.at(i));
			}
		}
	}

	// While the number of evaluations hasn't been reached
	while ((numPacsEvaluated < (parsedList.pacLambdaValue + parsedList.pacMu)) || (numGhostsEvaluated < (parsedList.ghostLambdaValue + parsedList.ghostMu)))
	{
		// Vectors to be passed into game are created
		vector <tuple <int, bool> > evaluationGhosts;
		vector <tuple <int, bool> > evaluationPacs;

		// for the number of ghost controllers per run
		for (int i = 0; i < ghostPerRun; i++)
		{
			// If the number of ghosts needed to be evaluated hasn't been reached
			if (numGhostsEvaluated < (parsedList.ghostMu + parsedList.ghostLambdaValue))
			{
				// Create a new ghost
				GhostController babyGhost;
				// Determine what percentile the ghost will have
				int ghostType = rand() % 100;

				// If fitness proportional selection is used for ghosts
				if (parsedList.ghostParentSelection == "FitnessProportionalSelection")
				{
					// Determine recombination or mutation for this ghost
					ghostRecOrMut(babyGhost, parsedList, ghostPopulation, ghostType);
				}
				// Otherwise over-selection is the only other option
				else if (parsedList.ghostParentSelection == "Over-Selection")
				{
					// Determine what percentile the ghost will have
					int whichGhost = rand() % 100;
					// If this ghost will get a parent from lower bracket
					if (whichGhost > static_cast<int>(100.00 * parsedList.percentFromTop))
					{
						// Determine recombination or recombination from lower bracket
						ghostRecOrMut(babyGhost, parsedList, ghostLowerBracket, ghostType);
					}
					// Otherwise determine recombination or recombination from upper bracket
					else
					{
						ghostRecOrMut(babyGhost, parsedList, ghostTopBracket, ghostType);
					}
				}
				// add this ghost to the population
				ghostPopulation.push_back(babyGhost);
				// Add this ghost to the evaluation pool
				evaluationGhosts.push_back(make_tuple(numGhostsEvaluated, true));
				// Increment number of ghosts evaluated
				numGhostsEvaluated += 1;
			}
			// Otherwise
			else
			{
				// Get a random member from the population and add to the evaluation pool
				int randomGhost = rand() % parsedList.ghostMu;
				evaluationGhosts.push_back(make_tuple(randomGhost, true));
			}
		}

		// for the number of pac controllers per run
		for (int i = 0; i < pacsPerRun; i++)
		{
			// If the number of pacs needed to be evaluated hasn't been reached
			if (numPacsEvaluated < (parsedList.pacLambdaValue + parsedList.pacMu))
			{
				// Create a new pac
				PacController child;
				// Determine what percentile the pac will have
				int childType = rand() % 100;

				// If fitness proportional selection is used for pacs
				if (parsedList.parentSelection == "FitnessProportionalSelection")
				{
					// Determine recombination or mutation for this pac
					determineRecOrMut(child, parsedList, population, childType);
				}
				// Otherwise over-selection is the only other option
				else if (parsedList.parentSelection == "Over-Selection")
				{
					// Determine what percentile the pac will have
					int whichPopulation = rand() % 100;
					// If this pac will get a parent from lower bracket
					if (whichPopulation > static_cast<int>(100.00 * parsedList.percentFromTop))
					{
						// Determine recombination or recombination from lower bracket
						determineRecOrMut(child, parsedList, lowerBracket, childType);
					}
					// Otherwise determine recombination or recombination from upper bracket
					else
					{
						determineRecOrMut(child, parsedList, topBracket, childType);
					}
				}
				// add this pac to the population
				population.push_back(child);
				// Add this pac to the evaluation pool
				evaluationPacs.push_back(make_tuple(numPacsEvaluated, true));
				// Increment number of pacs evaluated
				numPacsEvaluated += 1;
			}
			// Otherwise
			else
			{
				// Get a random member from the population and add to the evaluation pool
				int randomPac = rand() % (parsedList.pacLambdaValue + parsedList.pacMu);
				evaluationPacs.push_back(make_tuple(randomPac, true));
			}
		}

		//evaluate
		evaluate(evaluationPacs, evaluationGhosts, run, parsedList);
	}
}

void Run::competition(const ParsedList& parsedList)
{
	unsigned int usedMu = parsedList.pacMu;
	// If pac competition is truncation
	if (parsedList.survival == "Truncation")
	{
		// Until the population is reduced to its natural number
		while (population.size() > usedMu)
		{
			// Current lowest fitness member is index 0
			int getsKilled = 0;
			// go through all of the individuals after the first one
			for (unsigned int i = 1; i < population.size(); i++)
			{
				// If this member has a fitness lower than the current lowest
				if (population[i].score < population[getsKilled].score)
				{
					// store its index as the index with the lowesst fitness
					getsKilled = i;
				}
			}
			// Erase the member with the index of lowest fitness
			population.erase(population.begin() + getsKilled);
		}
	}
	// Otherwise pac k-Tournament selection is used
	else if (parsedList.survival == "k-Tournament")
	{
		// A vector of new population members is made
		vector <PacController> newPopulation;
		// Until the new population size reaches natural size
		while (newPopulation.size() < usedMu)
		{
			// Make a vector of tournament competitors
			vector <PacController> gladiators;
			// Get K individuals
			for (int i = 0; i < parsedList.pacK; i++)
			{
				// Choose a random position
				int chosen =  rand() % population.size();
				// add the competitor to the competitor pool
				gladiators.push_back(population.at(chosen));
				// Remove the individual from the general population
				population.erase(population.begin() + chosen);
			}
			// Get the individual with maximum fitness
			auto winner = std::max_element(gladiators.begin(), gladiators.end(), [](PacController s1, PacController s2) {return s1.score > s2.score;});
			// Make a new individual
			PacController addable;
			// Copy the winners score
			addable.score = gladiators.at(winner - gladiators.begin()).score;
			// Copy the winners tree
			addable.validator.copy(0, 0, gladiators.at(winner - gladiators.begin()).validator.root);
			// Place the copy of the winner in the new population
			newPopulation.push_back(addable);
			// Remove the winner from existance
			gladiators.erase(winner);
			// Until all gladiators are removed
			while (gladiators.size() > 0)
			{
				// Make a new individual
				PacController newOne;
				// Copy an individuals score to the new individual
				newOne.score = gladiators.back().score;
				// Copy the same individuals tree to the new individual
				newOne.validator.copy(0, 0, gladiators.back().validator.root);
				// Add the new individual to the original population for further competition
				population.push_back(newOne);
				// Remove the individual from the tournament pool
				gladiators.pop_back();
			}
		}
		// Clear the pac population
		while (population.size() > 0)
		{
			population.clear();
		}
		// The current population is now the replacement generation
		population = newPopulation;
	}

	usedMu = parsedList.ghostMu;

	// If ghost competition is truncation
	if (parsedList.ghostSurvival == "Truncation")
	{
		// Until the population is reduced to its natural number
		while (ghostPopulation.size() > usedMu)
		{
			// Current lowest fitness member is index 0
			int getsKilled = 0;
			// go through all of the individuals after the first one
			for (unsigned int i = 1; i < ghostPopulation.size(); i++)
			{
				// If this member has a fitness lower than the current lowest
				if (ghostPopulation[i].score < ghostPopulation[getsKilled].score)
				{
					// store its index as the index with the lowesst fitness
					getsKilled = i;
				}
			}
			// Erase the member with the index of lowest fitness
			ghostPopulation.erase(ghostPopulation.begin() + getsKilled);
		}
	}
	// Otherwise pac k-Tournament selection is used
	else if (parsedList.ghostSurvival == "k-Tournament")
	{
		// A vector of new population members is made
		vector <GhostController> newPopulation;
		// Until the new population size reaches natural size
		while (newPopulation.size() < usedMu)
		{
			// Make a vector of tournament competitors
			vector <GhostController> gladiators;
			// Get K individuals
			for (int i = 0; i < parsedList.ghostK; i++)
			{
				// Choose a random position
				int chosen =  rand() % ghostPopulation.size();
				// add the competitor to the competitor pool
				gladiators.push_back(ghostPopulation.at(chosen));
				// Remove the individual from the general population
				ghostPopulation.erase(ghostPopulation.begin() + chosen);
			}
			// Get the individual with maximum fitness
			auto winner = std::max_element(gladiators.begin(), gladiators.end(), [](GhostController s1, GhostController s2) {return s1.score > s2.score;});
			// Make a new individual
			GhostController addable;
			// Copy the winners score
			addable.score = gladiators.at(winner - gladiators.begin()).score;
			// Copy the winners tree
			addable.validator.copy(0, 0, gladiators.at(winner - gladiators.begin()).validator.root);
			// Place the copy of the winner in the new population
			newPopulation.push_back(addable);
			// Remove the winner from existance
			gladiators.erase(winner);
			// Until all gladiators are removed
			while (gladiators.size() > 0)
			{
				// Make a new individual
				GhostController newOne;
				// Copy an individuals score to the new individual
				newOne.score = gladiators.back().score;
				// Copy the same individuals tree to the new individual
				newOne.validator.copy(0, 0, gladiators.back().validator.root);
				// Add the new individual to the original population for further competition
				ghostPopulation.push_back(newOne);
				// Remove the individual from the tournament pool
				gladiators.pop_back();
			}
		}
		// Clear the ghost population
		while (ghostPopulation.size() > 0)
		{
			ghostPopulation.clear();
		}
		// The current population is now the replacement generation
		ghostPopulation = newPopulation;
	}
}

bool Run::checkTermination(const ParsedList& parsedList, int& stagnant, float& maxScore)
{
	// If the evaluation limit has been reached end the run
	if (evalNum >= parsedList.evals)
		return true;
	// If a termination condition is no change in best fitness
	if (parsedList.termination == "noChangeInBestFitnessForNGenerations")
	{
		// Find the highest fitness pac
		auto index = std::max_element(population.begin(), population.end(), [](PacController s1, PacController s2) {return s1.score > s2.score;});
		// If the highest fitness pac has the same fitness as the previous generation
		if (population.at(index - population.begin()).score == maxScore)
		{
			// Increment the number of stagnant generations
			stagnant += 1;
		}
		// Otherwise
		else
		{
			// Reset the number of stagnant generations
			stagnant = 0;
		}
		// Max fitness is the highest fitness individual's score
		maxScore = population.at(index - population.begin()).score;
		// If the number of stagnant generations has reached the limit end the run
		if (stagnant >= parsedList.n)
		{
			return true;
		}
	}
	// Otherwise, keep the run going
	return false;
}

int Run::ghostProportionalIndex(vector <GhostController>& testPopulation)
{
	int sumScore = 0;
	for (unsigned int i = 0; i < testPopulation.size(); i++)
	{
	    // Add all of the population members fitnesses
	    sumScore += abs(testPopulation.at(i).score);
	}
	int decision = -1;
	// Randomly choose a floating point number in between 0 and max fitness
	int chosen;
	if (sumScore == 0)
	{
		chosen = 0;
	}
	else
	{
		chosen = rand() % sumScore;
	}
	// Subtract each individuals fitness until an individual is reached
	// Each individual takes away from every other individuals chance of reproduction in this way
	while (chosen > decision)
	{
		decision += 1;
		chosen -= abs(testPopulation.at(decision).score);
	}
	// In the unlikely event 0 is chosen, choose the first individual
	if (decision == -1)
	    decision = 0;

	return decision;
}

int Run::fitnessProportionalIndex(const vector <PacController>& testPopulation)
{
	int sumScore = 0;
	for (unsigned int i = 0; i < testPopulation.size(); i++)
	{
	    // Add all of the population members fitnesses
	    sumScore += abs(testPopulation.at(i).score);
	}
	int decision = -1;
	// Randomly choose a floating point number in between 0 and max fitness
	int chosen;
	if (sumScore == 0)
	{
		chosen = 0;
	}
	else
	{
		chosen = rand() % sumScore;
	}
	// Subtract each individuals fitness until an individual is reached
	// Each individual takes away from every other individuals chance of reproduction in this way
	while (chosen > decision)
	{
		decision += 1;
		chosen -= abs(testPopulation.at(decision).score);
	}
	// In the unlikely event 0 is chosen, choose the first individual
	if (decision == -1)
	    decision = 0;

	return decision;
}

void Run::ghostRecOrMut(GhostController& child, const ParsedList& parsedList, vector <GhostController>& parentPopulation, int childType)
{
	// Get the fitness proportional chance
	int parent1 = ghostProportionalIndex(parentPopulation);
	// If the number is within the range of mutation
	if (childType > static_cast<int>(100.00 * parsedList.recomChance))
	{
		// Mutate
		child.mutate(parsedList, parentPopulation[parent1].validator);
	}
	// Otherwise
	else
	{
		// choose another population member
		int parent2 = ghostProportionalIndex(parentPopulation);
		// Recombine the two
		child.recombine(parsedList, parentPopulation[parent1].validator, parentPopulation[parent2].validator);
	}
}

void Run::determineRecOrMut(PacController& child, const ParsedList& parsedList, vector <PacController>& parentPopulation, int childType)
{
	// Get the fitness proportional chance
	int parent1 = fitnessProportionalIndex(parentPopulation);
	// If the number is within the range of mutation
	if (childType > static_cast<int>(100.00 * parsedList.recomChance))
	{
		// Mutate
		child.mutate(parsedList, parentPopulation[parent1].validator);
	}
	// Otherwise
	else
	{
		// choose another population member
		int parent2 = fitnessProportionalIndex(parentPopulation);
		// Recombine the two
		child.recombine(parsedList, parentPopulation[parent1].validator, parentPopulation[parent2].validator);
	}
}

void Run::evaluate(vector <tuple <int, bool> >& evaluationPacs, vector <tuple <int, bool> >& evaluationGhosts, int run, const ParsedList& parsedList)
{
	// Initialize a new game
	Game game(parsedList);

	// Run a game
	game.runGame(parsedList, textOutput, evaluationPacs, evaluationGhosts, population, ghostPopulation);

	// For all pacs evaluated
	for (unsigned int i = 0; i < evaluationPacs.size(); i++)
	{
		// If the score from most recent game is the new high score
		if (population.at(get<0>(evaluationPacs[i])).score > highScore)
		{

			if (population.at(get<0>(evaluationPacs[i])).score > globalBest)
			{
				globalThird = globalSecond;
				thirdBest = secondBest;

				globalSecond = globalBest;
				secondBest = bestWorld;


				globalBest = population.at(get<0>(evaluationPacs[i])).score;
				// Save this world
				bestWorld = textOutput;
			}
			else if (population.at(get<0>(evaluationPacs[i])).score > globalSecond)
			{
				globalThird = globalSecond;
				thirdBest = secondBest;

				globalSecond = population.at(get<0>(evaluationPacs[i])).score;
				// Save this world
				secondBest = textOutput;
			}
			else if (population.at(get<0>(evaluationPacs[i])).score > globalThird)
			{
				globalThird = population.at(get<0>(evaluationPacs[i])).score;
				// Save this world
				thirdBest = textOutput;
			}

			// Set best score to this score
			highScore = population.at(get<0>(evaluationPacs[i])).score;

			//empty the pac solution string
			solution = "";
			// for the entire length of the pac population
			for (unsigned int middleIndex = 0; middleIndex < evaluationPacs.size(); middleIndex++)
			{
				// for the entire length of the evaluted pac
				for (unsigned int index = 0; index < population.at(get<0>(evaluationPacs[middleIndex])).validator.root.size(); index++)
				{
					// add the value of a node to the string
					solution.append(population.at(get<0>(evaluationPacs[middleIndex])).validator.root[index] + " ");
				}
				// Newlines for readability
				solution.append("\n");
			}
			// Newlines for readability
			solution.append("\n");
		}
	}

	// For all ghosts evaluated
	for (unsigned int i = 0; i < evaluationGhosts.size(); i++)
	{
		// If the score from most recent game is the new high score
		if (ghostPopulation.at(get<0>(evaluationGhosts[i])).score > bestGhost)
		{
			// Set best score to this score
			bestGhost = ghostPopulation.at(get<0>(evaluationGhosts[i])).score;
			//empty the ghost solution string
			ghostSolution = "";
			// for the entire length of the ghost population
			for (unsigned int middleIndex = 0; middleIndex < evaluationGhosts.size(); middleIndex++)
			{
				// for the entire length of the evaluted ghost
				for (unsigned int index = 0; index < ghostPopulation.at(get<0>(evaluationGhosts[middleIndex])).validator.root.size(); index++)
				{
					// add the value of a node to the string
					ghostSolution.append(ghostPopulation.at(get<0>(evaluationGhosts[middleIndex])).validator.root[index] + " ");
				}
				// Newlines for readability
				ghostSolution.append("\n");
			}
			// Newlines for readability
			ghostSolution.append("\n");
		}
	}
	// Print completion of current evaluation for tracking progress
	cout << "Run: " << run << " Eval: " << evalNum << " Completed\n";

	// Increment the number of evaluations
	evalNum++;
}
