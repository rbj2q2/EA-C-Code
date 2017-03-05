#include <iostream>
#include <stdlib.h>
#include <string>
#include "run.h"

using namespace std;

int main(int argc, char* argv[])
{
     // Make sure the appropriate number of arguments were given
     if (argc != 2)
     {
          cout << "Wrong Number of Arguments: " << argc << " provided, 2 expected" << endl;
          // Return something other than 0, because it's incorrect usage
          return -1;
     }
     // Parse the List
     ParsedList parsedList(argv[1]);

     // Clear any old files
     Log output(parsedList.output, parsedList.solution, parsedList.worldFileOutput, parsedList.ghostSolutionDir);

     // Initialize anything needed
     Run run(parsedList.timerInitialized, parsedList.rngSeed);

     // Perform Experiments
     run.performRuns(parsedList, output);

     return 0;
}
