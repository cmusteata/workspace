#!/bin/bash
BEGIN=$(date +%s)
g++ main.cpp -std=c++11 -O3 -Wall -Wextra -o intro
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
