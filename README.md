#Edits by Robert Jones
The file that is run by the run.sh is located at src/main.cpp and is written for C++
     Additional files required to run the program are also located in the src/ directory

The configuration files are located in the configurations directory
     Valid configurations for each parameter are listed beneath each parameter
     Strings with quotation marks indicate valid strings; strings without quotation marks indicate valid data types
     Configuration 1 does not output files correctly and was not used for this assignment
     Configurations 2, 3, 4, default and small were used for this Assignment
     Test configuration was used for testing and debugging

The log files are located in the logs/ directory

The world log files are located in the worlds/ directory

The solution files are located in the solutions/ directory

The pdf with evaluation plots and statistical analysis is located at plots.pdf
The .tex and .ping files used to create the pdf are included in case you wish to recompile the .pdf

## Coding Standards
Code formatting and style for C, C++, C# and Java should roughly follow [MST's C++ coding guidelines.](http://web.mst.edu/~cpp/cpp_coding_standard_v1_1.pdf)
For python, [PEP8](https://www.python.org/dev/peps/pep-0008/) is ideal.

Because this course is more about the algorithms, we won't strictly hold you to thsese standards as long as your code is readable.
Having said that, we want you to comment and document the core algorithms very well so that it's clear you understand them. (Recombinations, Mutations, Selections, etc...)



## Submission Rules

This repo is your submission for CS5401, Assignment 2A. To submit, all you need to do is push your submission to the master branch on git-classes by the submission deadline.


In order for your submission to count, you **MUST** adhere to the following:

1. Add all of your configuration files in the *configurations* directory.
2. Add all of your algorithm run logs in the *logs* directory.
3. Add all of your highest-scoring game log files in the *games* directory.
4. Change the *run.sh* script to **compile and run** your submission. This script must take a configuration file as an argument.
    * Note: This script must run on the standard MST campus linux machines.
5. Commit and push the submission you wish to be graded to git-classes.mst.edu in the **master** branch, by the sumbmission deadline.
    * If for any reason you do/will miss this deadline, please e-mail the TA's ASAP.


Feel free to:
1. Add any files or folders you require.
2. Commit, branch, and clone this repo to your heart's desire. (We'll only look at *master* for grading)



## Comipiling and Running
As mentioned above, we will be using the *run.sh* bash script to compile and run your submissions. This script must work on campus linux machines. Therefore, when testing and running your submission, I suggest always using the command:
```
./run.sh <config file>

E.g:

./run.sh configurations/config1.cfg
```

I've also provided you with an example of what this script might look like for a simple C++ compilation and execution, HelloEC. Feel free to reuse parts of this for your own
project, though i suggest instead using a makefile for compilation.
