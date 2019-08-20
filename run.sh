#!/bin/bash
BEGIN=$(date +%s)
./intro/intro intro/intro.yaml 2> /dev/null
./intro2/intro intro2/intro.yaml 2> /dev/null
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
