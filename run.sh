#!/bin/bash

# Place your compile and execute script here.
# You can write any bash script that will run on campus linux machines.
# The below script will compile and execute the HelloEC program.
# HelloEC will also be passed the first argument to this script, a config file.
configFile=$1

# g++ -W -Wall -pedantic-errors src/main.cpp -o main
g++ -g -pg -std=c++11 -W -Wall -pedantic-errors src/*.cpp -O3 -o main # -g --enable-checking -v -da -Q

if [ $# -eq 0 ];
     then
          ./main configurations/default.cfg
          # ./main configurations/default.cfg

     else
          # valgrind ./main configurations/default.cfg
          ./main $configFile
fi
