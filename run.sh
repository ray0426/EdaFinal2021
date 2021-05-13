#!/bin/bash

RED='\033[0;31m'
NC='\033[0m' # No Color
LightGreen='\033[1;32m'

echo -e "${LightGreen}start compile${NC}"
g++ -o main.out problem.cpp main.cpp
echo -e "${LightGreen}compile complete${NC}"
./main.out ./benchmark/case1.txt output.txt
