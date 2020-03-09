#!/bin/bash
BEGIN=$(date +%s)
make
END=$(date +%s)
echo "elapsed "$((END-BEGIN))" seconds"
