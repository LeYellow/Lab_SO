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