#!/bin/bash

dir=$1
out=$2
content=""
count_legaturi=0
count_legaturi_aux=0

if [ $# -ne 2 ];
then
    echo "Not enough arguments"
fi

for file in "$dir"/*; do
    if [ -f "$file" ] && [[ "$file" == *.txt ]]; then
        content=$(cat $file | grep -E "^[A-Z][a-zA-Z0-9 ,]+\.$" | grep -E -v "si[ ]*\," | grep -E -v "n[pb]")
        if [ ! -z "$content" ]; then
            echo "File : $file" >> "$out"
            content=""
        fi
    elif [ -L "$file" ]; then
        count_legaturi=$((count_legaturi + 1))
    elif [ -d "$file" ]; then
        count_legaturi_aux=`bash $0 $file $out`
        count_legaturi=$((count_legaturi + $count_legaturi_aux))
    fi
done

echo $count_legaturi