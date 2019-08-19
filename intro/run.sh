#!/bin/bash
BEGIN=$(date +%s)
./intro intro.yaml 2> /dev/null
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
