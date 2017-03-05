#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include "board.h"

using namespace std;

Board::Board(int height, int width, float wallDensity, float pillPercentage)
{
	// Store board height
	boardHeight = height;
	// Store board width
     boardWidth = width;

	// Set pill count to 0
	pillCount = 0;
	// All spaces are currently walls
     emptySpaces = 0;

	// For all rows
	for (int i = 0; i < height; i++)
	{
		// Make a column for obstacles
		vector <char> newColumn;
		// Make a column for available moves
		map.push_back(newColumn);
		// Add the column to the moves map
		for (int j = 0; j < width; j++)
		{
			// Generate a wall in that index
			map.at(i).push_back('W');
		}
	}

	// Initialize the starting position of pac-man to be empty
	map.at(0).at(0) = 'E';
	// Initialize the starting position of the ghosts to be empty
	map.at(height-1).at(width-1) = 'E';

	// Carve paths
	shapeWalls(height, width, wallDensity);
	// Place pills
	placePills(height, width, pillPercentage);

	// There is no initial fruit on the board
	fruitOnBoard = false;
}

void Board::shapeWalls(int height, int width, float wallDensity)
{
	// Calculate the number of walls a board should have
	int numWalls = static_cast<int>(floor(height * width * (1 - wallDensity)));

	// Carve a path from pac-man starting position to the bottom left corner
	for (int i = 0; i < height; i++)
	{
		// Mark the current grid space as empty
		map.at(i).at(0) = 'E';
		// Decrement the number of walls
		numWalls -= 1;
		// Increment the number of empty spaces
		emptySpaces += 1;
	}

	// Carve a path from the lower left corner to the ghosts' starting position
	for (int i = 0; i < width; i++)
	{
		// Mark the current grid space as empty
		map.at(height - 1).at(i) = 'E';
		// Decrement the number of walls
		numWalls -= 1;
		// Increment the number of empty spaces
		emptySpaces += 1;
	}

	// While the number of walls is less than the expected number of walls
	while (numWalls > 0)
	{
		// Choose a random x and y position
		int randomX = rand()%width;
		int randomY = rand()%height;
		// Choose a direction to go from, the left or bottom
		int direction = rand()%2;
		// Count position away from wall that is being tunneled
		int count = 1;

		// While the number of walls is less than the expected number of walls and the chosen target grid square is not emptySpaces
		// Carve a path from the randomly chosen direction to the randomly chosen grid square
		while ((numWalls > 0) && (map.at(randomY).at(randomX) != 'E'))
		{
			// If we're carving from the bottom of the board
			if (direction == 0)
			{
				// Make a connected square empty towards the randomly chosen grid square
				map.at(randomY).at(count) = 'E';
			}
			// Otherwise, we're carving from the left of the board
			else
			{
				// Make a connected square empty towards the randomly chosen grid square
				map.at(height - 1 - count).at(randomX) = 'E';
			}
			// Increment the distance from the wall we're carving from
			count += 1;
			// Decrement the number of walls
			numWalls -= 1;
			// Increment the number of empty spaces
			emptySpaces += 1;
		}
	}
}

void Board::placePills(int height, int width, float pillPercentage)
{
	// A minimum of one pill is required
	while (pillCount == 0)
	{
		// for all columns
		for (int i = 0; i < width; i++)
		{
			// for all indices of a given column
			for (int j = 0; j < height; j++)
			{
				// Pac-Man starting square is not allowed to have a pill
				if ((i != 0) || (j != 0))
				{
					// If the grid position is empty
					if (map.at(j).at(i) == 'E')
					{
						// The percentage chance of a pill spawning
						int chance = static_cast<int>(floor(100 * pillPercentage));
						// A random percentage is generated
						int randomNum = rand()%100;
						// If the random percentage generated is less than the chance of a pill spawning
						if (randomNum <= chance)
						{
							// Place a pill in the grid position
							map.at(j).at(i) = 'P';
							// Increment the number of pills
							pillCount +=1;
							// Decrement the number of empty spaces
							emptySpaces -= 1;
						}
					}
				}
			}
		}
	}
}

void Board::setFruit(int x, int y)
{
	// Board position contains a fruit
	map.at(y).at(x) = 'F';
	// Store value of fruit x
	fruitX = x;
	// Store value of fruit y
	fruitY = y;
	// The fruit is on the board
	fruitOnBoard = true;
}

void Board::printBoard()
{
	// Print each row in order
	for (unsigned int i = 0; i < map.size(); i++)
	{
		// Mark the beginning of the row with a [
		cout << "[\t";
		// For each index of a given row
		for (unsigned int j = 0; j < map.at(i).size(); j++)
		{
			// Print the status of the current grid cell
			cout << map.at(i).at(j) << "\t";
		}
		// Mark the end of a row with a ]
		cout << "]\n";
	}
}
