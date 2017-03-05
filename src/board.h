#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class Board
{
public:
     // Initialize the number of pills on the board to 0
     int pillCount;
     // Initialize the number of empty spaces to 0
     int emptySpaces;
     // Whether a fruit is on the board or not
     bool fruitOnBoard;
     // store the x value of fruit
     int fruitX;
     // Store the y value of fruit
     int fruitY;
     // Keep track of board height
     int boardHeight;
     // Keep track of board width
     int boardWidth;

     // 2D Map of obstacles; F is fruit, P is pill, W is wall, E is empty
     // First at is y position, Second at is x position
     vector < vector <char> > map;

     // Initialize board to given height, width, wall Density and pill percentage
     Board(int height, int width, float wallDensity, float pillPercentage);

     // Removes walls to fit wall density parameter
     void shapeWalls(int height, int width, float wallDensity);
     // Place pills to fity pill percentage parameter
     void placePills(int height, int width, float pillPercentage);

     // Put a fruit on the board
     void setFruit(int x, int y);

     // Print the board for debugging purposes
     void printBoard();

};

#endif
