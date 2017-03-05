#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

#include "parsedList.h"

ParsedList::ParsedList(const string& configFile)
{
	// Read from a file
	ifstream inFile;
	// Open the configuration file
	inFile.open(configFile.c_str());
	// If the open was successful
	if (inFile.is_open())
	{
		// String for holding a given line
		string line;
		// The line currently being read is the first line
		int i=1;

		// As long lines are still being read, store read line in line variable
		while(getline(inFile,line))
		{
			// Find the position of the colon in the line variable
			int position = line.find(":");
			// Erase everything in the line variable up to and including the colon
			line.erase(0,position+1);
			// Remove all whitespace
			line.erase(std::remove_if(line.begin(),
				line.end(),
				[](char c) { return std::isspace(c);}),
				line.end());
			// The line number determines which variable is being stored
			switch(i)
			{
    			case 1:
				// Store solution location as a string
				solution = line;
             		break;
    			case 2:
				// Store log location as a string
				output = line;
             		break;
			case 3:
				// Store world file location as a string
				worldFileOutput = line;
				break;
			case 4:
				// Store world file location as a string
				ghostSolutionDir = line;
				break;
			case 5:
				// Store whether or not we're using a timer initialized seed as a bool
				if (line == "True")
					// Change string to bool variable true if True is found in the configuration file
					timerInitialized = true;
				else
					// Otherwise, store false
					timerInitialized = false;
				break;
			case 7:
				// Store random number generator seed as an integer
				sscanf(line.c_str(), "%i", &rngSeed);
				break;
			case 10:
				// Store the number of runs as an integer
				sscanf(line.c_str(), "%i", &runs);
				break;
			case 12:
				// Store the number of evaluations per run as an integer
				sscanf(line.c_str(), "%i", &evals);
				break;
			case 14:
				// Store the height of the board as an integer
				sscanf(line.c_str(), "%i", &height);
				break;
			case 16:
				// Store the width of the board as an integer
				sscanf(line.c_str(), "%i", &width);
				break;
			case 18:
				// Store the probability of a pill spawning in any given empty square as a floating point number
			     pillDensity = atof(line.c_str());
				break;
			case 20:
				// Store the probability of any grid square being a wall as a floating point number
			     wallDensity = atof(line.c_str());
				break;
			case 22:
				// Store the probability of a fruit spawning in any given empty square as a floating point number
			     fruitSpawning = atof(line.c_str());
				break;
			case 24:
				// Store the multiplier of time allotted for completing a game as a floating point number
			     timeMultiplier = atof(line.c_str());
				break;
			case 26:
				// Store the score amount added for eating a fruit as an integer
				sscanf(line.c_str(), "%i", &fruitScore);
				break;
			case 30:
				// Store pac representation as a string
				representation = line;
             		break;
			case 32:
				// Store pac initialization as a string
				initialization = line;
             		break;
			case 34:
				// Store pac parent selection as a string
				parentSelection = line;
             		break;
			case 36:
				// Store pac recombination as a string
				recombination = line;
             		break;
			case 38:
				// Store pac mutation as a string
				mutation = line;
             		break;
			case 40:
				// Store pac survival selection as a string
				survival = line;
             		break;
			case 42:
				// Store pac bloat control mechanic as a string
				bloatControl = line;
             		break;
			case 44:
				// Store termination condition as a string
				termination = line;
             		break;
			case 47:
				// Store pac mu as an integer
				sscanf(line.c_str(), "%i", &pacMu);
				break;
			case 49:
				// Store pac lambda as an integer
				sscanf(line.c_str(), "%i", &pacLambdaValue);
				break;
			case 51:
				// Store pac K-Survival value as an integer
				sscanf(line.c_str(), "%i", &pacK);
				break;
			case 53:
				// Store pac parsimony pressure as a float
				pacP = atof(line.c_str());
				break;
			case 55:
				// Store pac K-Survival value as an integer
				sscanf(line.c_str(), "%i", &maxEvals);
				break;
			case 57:
				// Store pac n for termination convergence as an integer
				sscanf(line.c_str(), "%i", &n);
				break;
			case 59:
				// Store maximum tree depth as an integer
				sscanf(line.c_str(), "%i", &treeDepth);
				break;
			case 61:
				// Store recombination chance as a float
				recomChance = atof(line.c_str());
				break;
			case 63:
				// Store percentage that counts as top bracket as a float
				topChosenPercent = atof(line.c_str());
				break;
			case 65:
				// Store percentage chosen from top bracket as a float
				percentFromTop = atof(line.c_str());
				break;
			case 73:
				// Store ghost representation as a string
				ghostRepresentation = line;
             		break;
			case 75:
				// Store ghost initialization as a string
				ghostInitialization = line;
             		break;
			case 77:
				// Store ghost parent selection strategy as a string
				ghostParentSelection = line;
             		break;
			case 79:
				// Store ghost recombination strategy as a string
				ghostRecombination = line;
             		break;
			case 81:
				// Store ghost mutation strategy as a string
				ghostMutation = line;
             		break;
			case 83:
				// Store ghost survival selection strategy as a string
				ghostSurvival = line;
             		break;
			case 86:
				// Store ghost mu as an integer
				sscanf(line.c_str(), "%i", &ghostMu);
				break;
			case 88:
				// Store ghost lambda as an integer
				sscanf(line.c_str(), "%i", &ghostLambdaValue);
				break;
			case 90:
				// Store ghost K-survival value as an integer
				sscanf(line.c_str(), "%i", &ghostK);
				break;
			case 92:
				// Store ghost parsimony pressure coefficient as an integer
				ghostP = atof(line.c_str());
				break;
			case 95:
				// Store whether or not we're using a unique controllers for each ghost as a bool
				if (line == "True")
					// Change string to bool variable true if True is found in the configuration file
					uniqueGhost = true;
				else
					// Otherwise, store false
					uniqueGhost = false;
				break;
			case 97:
				sscanf(line.c_str(), "%i", &pacPerGame);
				break;
			case 99:
				// Store whether or not we're using unique controllers for each pac as a bool
				if (line == "True")
					// Change string to bool variable true if True is found in the configuration file
					uniquePac = true;
				else
					// Otherwise, store false
					uniquePac = false;
				break;
			case 101:
				// Store speed ratio as a float
				speedRatio = atof(line.c_str());
				break;
			default:
				// all other lines are ignored
				break;
			}
			// Increment line number
			i++;
		}
		// Close the file
		inFile.close();
	}
}
