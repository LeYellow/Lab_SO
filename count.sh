#!/bin/bash

dir=$1
file=$2

sum=0
count=0

for fisier in "$dir"/*.txt; do
    if [ ! -f $fisier ]; then
        echo "nu exista txt"
        exit 1
    fi
    count=$(wc -L < $fisier)
    echo "nume fisier: $fisier; nr caractere: $count" >> $file;
    sum=`expr $sum + $count`;
done

echo "Total: $sum" >> $file;