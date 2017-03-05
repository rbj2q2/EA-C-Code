#include <iostream>

#include "log.h"

using namespace std;

Log::Log(const string& output, const string& solutionOut, const string& worldFileOutput, const string& ghostSol)
{
	// set ofstream operator
	ofstream out;
	// Set log file address
	logOut = output;
	// set solution file address
	solution = solutionOut;
	// set world file address
	worldFile = worldFileOutput;
	// set ghost solution address
	ghostOut = ghostSol;

	// Open and Clear Old Data
	out.open(logOut, std::ofstream::out | std::ofstream::trunc);
	// Label New Data
	out << "Result Log\n";
	// Close File
	out.close();

	// Open and Clear Old Data
	out.open(solution, std::ofstream::out | std::ofstream::trunc);
	// Close File
	out.close();

	// Open and Clear Old Data
	out.open(ghostSol, std::ofstream::out | std::ofstream::trunc);
	// Close File
	out.close();

	// Open and Clear Old Data
	out.open(worldFile, std::ofstream::out | std::ofstream::trunc);
	// Close File
	out.close();
}

void Log::updateLog(int evals, int averageScore, int highScore) const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(logOut, std::ios_base::app);
	// Print data to log file
	out << evals << "\t" << averageScore << "\t" << highScore << "\n";
	// Close File
	out.close();
}

void Log::updateSolution(string key) const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(solution, std::ios_base::app);
	// Print data to log file
	out << key << "\n";
	// Close File
	out.close();
}

void Log::updateGhostSolution(string key) const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(ghostOut, std::ios_base::app);
	// Print data to log file
	out << key << "\n";
	// Close File
	out.close();
}

void Log::logLabelRun(int run) const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(logOut, std::ios_base::app);
	// Print data to log file
	out << "Run " << run << "\n";
	// Close File
	out.close();
}

void Log::logEndLine() const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(logOut, std::ios_base::app);
	// Print Endline
	out << "\n";
	// Close File
	out.close();
}

void Log::updateWorldFile(string key) const
{
	// set ofstream operator
	ofstream out;
	// Open File in append mode
	out.open(worldFile, std::ios_base::app);
	// Print data to log file
	out << key;
	// Close File
	out.close();
}
