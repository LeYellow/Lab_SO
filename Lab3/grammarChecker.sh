#!/bin/bash

while read line
do
    echo $line | grep -E "^[A-Z][a-zA-Z0-9 ,]+\.$" | grep -E -v "si[ ]*\," | grep -E -v "n[pb]"
done