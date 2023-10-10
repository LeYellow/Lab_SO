#!/bin/bash

#1
if [ $# -lt 3 ]; then
    echo "nr prea mic de argumente"
    exit 1
fi

count=0
sum=0

file=$1
dir=$2
shift 2

#2+3
for arg in "$@"
do
    if [ $arg -gt 10 ]; then
        count=`expr $count + 1`
    fi
    sum=`expr $sum + $arg`
done

#4
echo "nr de digit in suma"
echo $sum | wc -L

#5
if [ ! -e $file ]; then   #verifica daca exista
    echo "count=$count; sum=$sum" > $file
else
    if [ -f $file ]; then #verifica daca e bun fisieru
    echo "count=$count; sum=$sum" > $file
    else
    echo "nem fisier"
    exit 1
    fi
fi

#6
for fisier in "$dir"/*.txt; do #cauta daca sunt fiesiere text
    if [ ! -f $fisier ]; then  
        echo "nu avem txt"
        exit 1
    fi
    cat $fisier
done