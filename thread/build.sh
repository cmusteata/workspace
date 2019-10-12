#!/bin/bash
BEGIN=$(date +%s)
g++ -pthread -std=c++11 -Wall -Wextra -O3 -o $1 src/$1.cpp
END=$(date +%s)
ELAPSED=$((END-BEGIN))
echo "elapsed $ELAPSED seconds"
