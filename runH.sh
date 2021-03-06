#!/bin/bash

RED='\033[0;31m'
NC='\033[0m' # No Color
LightGreen='\033[1;32m'

echo -e "${LightGreen}start compile${NC}"
g++ -o main.out problem.cpp main.cpp
echo $?
if [ "$?" -ne 0 ]; # $rc not equal to zero
then
    echo -e "${RED}compile error${NC}"
else
    echo -e "${LightGreen}compile complete${NC}"
    echo -e "${LightGreen}start execute${NC}"
    echo -e "================================================================"
    ./main.out ./benchmark/case1.txt output.txt
    echo -e "================================================================"
fi
