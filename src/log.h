#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Log
{
public:
     // holds location of log output file
     string logOut;
     // holds location of solution output file
	string solution;
     // holds location of world output file
	string worldFile;
     // holds location of ghost solution output file
     string ghostOut;

     // Constructor for setting up format of output files and clearing old data
     Log(const string& output, const string& solutionOut, const string& worldFileOutput, const string& ghostSol);

     // Update log with new high score
     void updateLog(int evals, int averageScore, int highScore) const;
     // update pac solution file
     void updateSolution(string key) const;
     // update ghost solution file
     void updateGhostSolution(string key) const;
     // Label run in log file
     void logLabelRun(int run) const;

     // End Line function used after blocks of evaluations
     void logEndLine() const;

     // Update World Format File
     void updateWorldFile(string key) const;

};

#endif
