#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

#include "game.h"

using namespace std;

Game::Game(const ParsedList& parsedList)
{
	// Store height from parsed list
	height = parsedList.height;
	// Store width from parsed list
	width = parsedList.width;
	// Store fruitScore from parsed list
	fruitScore = parsedList.fruitScore;
	// Store fruitSpawning from parsed list
	fruitSpawning = parsedList.fruitSpawning;
	// Calculate the maximum allotted time for playing a game based on width, height, and the time multiplier
	baseTime = static_cast<int>(floor(static_cast<double>(width) * static_cast<double>(height) * static_cast<double>(parsedList.timeMultiplier)));

}

void Game::runGame(const ParsedList& parsedList, string& textOutput, vector <tuple <int, bool> >& evaluationPacs, vector <tuple <int, bool> >& evaluationGhosts, vector <PacController>& population, vector <GhostController>& ghostPopulation)
{
	// Initialize a board
	Board board(parsedList.height, parsedList.width, parsedList.pillDensity, parsedList.wallDensity);

	// Set number of pills obtained to 0
	currentPills = 0;
	// Initialize maximum number of pills to the board's number of pills
	int maxPills = board.pillCount;
	// Set current remaining time to the maximum amount of allotted time
	currentTime = baseTime;
	// There is no fruit on the board
	board.fruitOnBoard = false;
	// No score from fruit has been obtained
	currentFruitTotal = 0;
	// Set initial score to 0
	sumScore = 0.0;

	// The game is being played
	playing = true;

	// x,y,prevX,prevY,population position
	vector < tuple<int,int,int,int,int> > vectorOfGhosts;

	// Initialize vector of ghosts
	for (unsigned int i = 0; i < 3; i++)
	{
		// If not all controllers of ghost from the vector given are not in the new vector, add
		if (evaluationGhosts.size() > i)
			vectorOfGhosts.push_back(make_tuple(parsedList.width - 1, parsedList.height - 1, parsedList.width - 1, parsedList.height - 1, get<0>(evaluationGhosts[i])));
		else
		{
			// Otherwise, add a random controller to the vector of ghost controllers
			int randomInRange = rand() % evaluationGhosts.size();
			vectorOfGhosts.push_back(make_tuple(parsedList.width - 1, parsedList.height - 1, parsedList.width - 1, parsedList.height - 1, get<0>(evaluationGhosts[randomInRange])));
		}
	}

	// x,y,prevX,prevY,population position, stillGoing
	vector < tuple<int,int,int,int,int,bool> > vectorOfPacs;
	// Initialize vector of pac controllers
	for (int i = 0; i < parsedList.pacPerGame; i++)
	{
		int converted = evaluationPacs.size();
		// If not all controllers of pac from the vector given are not in the new vector, add
		if (converted > i)
			vectorOfPacs.push_back(make_tuple(0, 0, 0, 0, get<0>(evaluationPacs[i]), true));
		else
		{
			// Otherwise, add a random controller to the vector of pac controllers
			int randomInRange = rand() % evaluationPacs.size();
			vectorOfPacs.push_back(make_tuple(0, 0, 0, 0, true, get<0>(evaluationGhosts[randomInRange])));
		}
	}

	// Record the initial state of the game in the world file
	worldFileSetting(true, currentTime, false, 0, 0, board, textOutput, vectorOfPacs, vectorOfGhosts);

	// Offset is used for speed ratio
	float offset = 0.0;

	// While the game is being played
	while (playing == true)
	{
		// Add speed ratio to offset
		offset += parsedList.speedRatio;
		// Pac-Man has a valid move of hold position
		string valid;

		// Pac gets to make offset number of moves
		while (offset >= 1.00)
		{
			// Decrement offset for each move made
			offset -= 1.0;
			// Each pac gets to make a move
			for (unsigned int i = 0; i < vectorOfPacs.size(); i++)
			{
				// If this pac is still playing
				if (get<5>(vectorOfPacs[i]))
				{
					// All pac men can hold, hold is added to string of valid moves
					valid = "h";
					// All valid positions are found
					valid = validPosition(valid, get<0>(vectorOfPacs[i]), get<1>(vectorOfPacs[i]), board);

					// Choose a valid move
					population.at(get<4>(vectorOfPacs[i])).chooseMove(valid, board, vectorOfGhosts, vectorOfPacs, i);
				}
			}
		}

		// For all 3 ghosts, find and make their valid moves
		for (int i = 0; i < 3; i++)
		{
			// Contains all valid moves for the ghosts
			valid = "";
			// All valid moves for the ghosts are found
			valid = validPosition(valid, get<0>(vectorOfGhosts[i]), get<1>(vectorOfGhosts[i]), board);
			// Choose a valid move
			ghostPopulation.at(get<4>(vectorOfGhosts[i])).chooseMove(valid, board, vectorOfGhosts, vectorOfPacs, i);
		}

		// Decrement the amount of available time left for playing the game
		currentTime -= 1;

		// Update the score, update the board and check if the game is over
		updateGame(maxPills, board, currentTime, false, textOutput, vectorOfPacs, vectorOfGhosts);
	}

	// For all pac men
	for (unsigned int i = 0; i < evaluationPacs.size(); i++)
	{
		// increment sum of scores
		population[get<0>(evaluationPacs[i])].sumAllGameScores += sumScore - (population[get<0>(evaluationPacs[i])].validator.root.size() * parsedList.pacP);
		// Increment number of games
		population[get<0>(evaluationPacs[i])].numberOfGames += 1;
		// Set score to average score
		population[get<0>(evaluationPacs[i])].score = (population[get<0>(evaluationPacs[i])].sumAllGameScores / static_cast<float>(population[get<0>(evaluationPacs[i])].numberOfGames));
	}

	// For all ghosts
	for (unsigned int i = 0; i < evaluationGhosts.size(); i++)
	{
		// Increment sum of scores
		ghostPopulation[get<0>(evaluationGhosts[i])].sumAllGameScores += (200.00 - (sumScore / static_cast<float>(evaluationPacs.size()))) - (ghostPopulation[get<0>(evaluationGhosts[i])].validator.root.size() * parsedList.ghostP);
		// Increment number of games
		ghostPopulation[get<0>(evaluationGhosts[i])].numberOfGames += 1;
		// Set score to average score
		ghostPopulation[get<0>(evaluationGhosts[i])].score = (ghostPopulation[get<0>(evaluationGhosts[i])].sumAllGameScores / static_cast<float>(ghostPopulation[get<0>(evaluationGhosts[i])].numberOfGames));
	}
}

void Game::updateGame(int maxPills, Board& board, int currentTime, bool first, string& textOutput, vector < tuple<int,int,int,int,int,bool> >& vectorOfPacs, vector < tuple<int,int,int,int,int> >& vectorOfGhosts)
{
	// Initialize whether or not a fruit has been placed this round to false
	bool fruitPlaced = false;
	// Initialize the x position of a fruit that may be placed to 0
	int fruitX = 0;
	// Initialize the y position of a fruit that may be placed to 0
	int fruitY = 0;

	// For all pacs
	for (unsigned int i = 0; i < vectorOfPacs.size(); i++)
	{
		// If pac-man is on a pill square
		if (board.map.at(get<1>(vectorOfPacs[i])).at(get<0>(vectorOfPacs[i])) == 'P')
		{
			// The square is set to empty
			board.map.at(get<1>(vectorOfPacs[i])).at(get<0>(vectorOfPacs[i])) = 'E';
			// The number of pills Pac-man has eaten is incremented
			currentPills++;
			// The number of empty squares is incremented
			board.emptySpaces += 1;
		}
		// If pac-man is on a fruit square
		else if (board.map.at(get<1>(vectorOfPacs[i])).at(get<0>(vectorOfPacs[i])) == 'F')
		{
			// The square is set to empty
			board.map.at(get<1>(vectorOfPacs[i])).at(get<0>(vectorOfPacs[i])) = 'E';
			// There is no fruit on the board
			board.fruitOnBoard = false;
			// The score gained from eating fruit is increased by the value of the fruit
			currentFruitTotal += fruitScore;
		}
	}

	// If there is no fruit on the board and there is at least one space without pills that isn't Pac-Man's starting square
	if ((board.fruitOnBoard == false) && (board.emptySpaces > 1))
	{
		// Generate a random number between 1 and 99
		int randomChance = rand() % 100;
		// If the chance that a fruit spawns is greater than the randomly generated number
		if (100.00 * fruitSpawning > randomChance)
		{
			// Choose a random empty square
			int position = rand() % board.emptySpaces;
			// While we have not found the chosen square
			while (position > 0)
			{
				// Default add this fruit to the board
				bool goodToGo = true;
				// For all pac on the board
				for (unsigned int i = 0; i < vectorOfPacs.size(); i++)
				{
					// If the fruit is on a pac position, this position is not good
					if ((fruitY == get<1>(vectorOfPacs[i])) && (fruitX == get<0>(vectorOfPacs[i])))
						goodToGo = false;
				}
				// If the current square is empty (without pac, wall or pill)
				if ((board.map.at(fruitY).at(fruitX) == 'E') && (goodToGo))
					// Decrement the number of empty squares until the randomly chosen one is found
					position -= 1;
				// If this isn't the square randomly chosen
				if (position > 0)
				{
					// Look at the next square to the right
					fruitX += 1;
					// If there is no square to the right
					if (fruitX >= width)
					{
						// Move to the beginning of the column
						fruitX = 0;
						// Move to the next row
						fruitY += 1;
						// If there is no next row
						if (fruitY >= height)
							// Move to the first row
							fruitY = 0;
					}
				}
			}
			// Mark the randomly chosen empty square as now containing a fruit
			board.setFruit(fruitX, fruitY);

			// A fruit was placed this round
			fruitPlaced = true;
		}
	}

	// For all 3 ghosts
	for (int i = 0; i < 3; i++)
	{
		// for each pac
		for (unsigned int pacIndex = 0; pacIndex < vectorOfPacs.size(); pacIndex++)
		{
			// If this pac hasn't been killed
			if (get<5>(vectorOfPacs[pacIndex]))
			{
				// If pac-man and this ghost are in the same square
				if ((get<1>(vectorOfPacs[pacIndex]) == get<1>(vectorOfGhosts[i])) && (get<0>(vectorOfPacs[pacIndex]) == get<0>(vectorOfGhosts[i])))
				{
					// This pac is dead
					get<5>(vectorOfPacs[pacIndex]) = false;

					// End the game by default
					playing = false;
					// For all pac
					for (unsigned int checkCount = 0; checkCount < vectorOfPacs.size(); checkCount++)
					{
						// If any pac is still alive, the game is still going
						if (get<5>(vectorOfPacs[checkCount]))
							playing = true;
					}
				}
				// If pac-man and this ghost collided
				else if ((get<1>(vectorOfPacs[pacIndex]) == get<3>(vectorOfGhosts[i])) && (get<0>(vectorOfPacs[pacIndex]) == get<2>(vectorOfGhosts[i])))
				{
					if ((get<3>(vectorOfPacs[pacIndex]) == get<1>(vectorOfGhosts[i])) && (get<2>(vectorOfPacs[pacIndex]) == get<0>(vectorOfGhosts[i])))
					{
						// This pac is dead
						get<5>(vectorOfPacs[pacIndex]) = false;

						// End the game by default
						playing = false;
						// For all pac
						for (unsigned int checkCount = 0; checkCount < vectorOfPacs.size(); checkCount++)
						{
							// If any pac is still alive, the game is still going
							if (get<5>(vectorOfPacs[checkCount]))
								playing = true;
						}
					}
				}
			}
		}
	}

	// If all the pills are eaten or there is no remaining time
	if ((currentPills == maxPills) || (currentTime == 0))
		// End the game
		playing = false;

	// If all of the pills are eaten
	if (currentPills == maxPills)
		// Calculate score with a bonus
		sumScore = (currentFruitTotal + 100 + static_cast<int>(floor(100.00 * (static_cast<double>(currentTime)/static_cast<double>(baseTime)))));
	else
		// Otherwise, calculate the score normally
		sumScore = (currentFruitTotal + static_cast<int>(floor(100.00 * static_cast<double>(currentPills) / static_cast<double>(maxPills))));

	// Record the board format
	worldFileSetting(first, currentTime, fruitPlaced, fruitX, fruitY, board, textOutput, vectorOfPacs, vectorOfGhosts);
}

string Game::validPosition(string valid, int xPos, int yPos, Board board)
{
	// If the grid square above is valid
	if (yPos - 1 >= 0)
	{
		// If the grid square above is not a wall
		if (board.map.at(yPos-1).at(xPos) != 'W')
			// Add up as a valid move
			valid.append("u");
	}
	// If the grid square below is valid
	if (yPos + 1 < height)
	{
		// If the grid square below is not a wall
		if (board.map.at(yPos+1).at(xPos) != 'W')
			// Add down as a valid move
			valid.append("d");
	}
	// If the grid square to the left is valid
	if (xPos - 1 >= 0)
	{
		// If the grid square to the left is not a wall
		if (board.map.at(yPos).at(xPos-1) != 'W')
			// Add left as a valid move
			valid.append("l");
	}
	// If the grid square to the right is valid
	if (xPos + 1 < width)
	{
		// If the grid square to the right is not a wall
		if (board.map.at(yPos).at(xPos+1) != 'W')
			// Add right as a valid move
			valid.append("r");
	}
	// Return a complete string of all valid moves
	return valid;
}

void Game::worldFileSetting(bool first, int currentTime, bool fruitPlaced, int fruitX, int fruitY, Board& board, string& textOutput, vector < tuple<int,int,int,int,int,bool> >& vectorOfPacs, vector < tuple<int,int,int,int,int> >& vectorOfGhosts)
{
	// If this is the first round of a game
	if (first == true)
	{
		// Label the height and width
		textOutput.append(to_string(width) + "\n" + to_string(height) + "\n");
	}
	// Record position of all pac
	for (unsigned int i = 0; i < vectorOfPacs.size(); i++)
	{
		// Add pac-man stats to output
		textOutput.append("m " + to_string(get<0>(vectorOfPacs[i])) + " " + to_string(get<1>(vectorOfPacs[i])) + "\n");
	}
	// Add 3...
	for (int i = 0; i < 3; i++)
		// ...ghost statistics to output
		textOutput.append(to_string(i + 1) + " " + to_string(get<0>(vectorOfGhosts[i])) + " " + to_string(get<1>(vectorOfGhosts[i])) + "\n");
	// The first round, we also output the board state
	if (first == true)
	{
		// Output all rows
		for (int i = 0; i < width; i++)
			//output each index of a given row
			for (int j = 0; j < height; j++)
				// If this grid location has a pill
				if (board.map.at(j).at(i) == 'P')
					// Add pill location to output
					textOutput.append("p " + to_string(i) + " " + to_string(j) + "\n");
				// If this grid location has a wall
				else if (board.map.at(j).at(i) == 'W')
					// Add wall location to output
					textOutput.append("w " + to_string(i) + " " + to_string(j) + "\n");
	}
	// If a fruit was placed this round
	if (fruitPlaced == true)
		// Add the fruit location to output
		textOutput.append("f " + to_string(fruitX) + " " + to_string(fruitY) + "\n");

	// Add time and score to output, mark end of round
	textOutput.append("t " + to_string(currentTime) + " " + to_string(sumScore) + "\n");
}
