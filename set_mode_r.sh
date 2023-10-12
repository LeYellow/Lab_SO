#!/bin/bash

dir=$1
mode=$2

for fisier in "$dir"/*.txt; do
    if [ ! -f $fisier ]; then
        echo "nu exista txt"
        exit 1
    fi
    chmod +$mode $fisier
done

for fisier2 in "$dir"/*; do
    if [ -d $fisier2 ]; then
        bash $0 $fisier2 $mode
    fi
done