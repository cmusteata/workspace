#!/bin/bash
BEGIN=$(date +%s)
./intro 2> /dev/null
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
