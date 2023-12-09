#!/bin/bash

letter=$1
#file=$2
content=""
count_litera=0

if [ $# -ne 1 ];
    then
    echo "Not enough arguments"
    exit 1
fi

while read linie
do
    content=$(cat $linie | grep -E "$letter" | grep -E "^[A-Z][a-zA-Z0-9 ,]+\.$" | grep -E -v "si[ ]*\," | grep -E -v "n[pb]")            
    if [ ! -z "$content" ]; then
        echo "Litera '$letter' apare in '$linie'."
        content=""
        count_litera=$((count_litera+1))
    else
        echo "Litera nu apare"
    fi
    done
    exit $count_litera