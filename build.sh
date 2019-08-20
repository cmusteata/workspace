#!/bin/bash
BEGIN=$(date +%s)
g++ intro/main.cpp intro/algo.cpp introlib/yaml.cpp introlib/bridge.cpp -I introlib -std=c++11 -O3 -Wall -Wextra -o intro/intro
g++ intro2/main.cpp intro2/algo.cpp introlib/yaml.cpp introlib/bridge.cpp -I introlib -std=c++11 -O3 -Wall -Wextra -o intro2/intro
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
