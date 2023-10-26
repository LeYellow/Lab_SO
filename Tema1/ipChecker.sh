#!/bin/bash

while read line
do 
    echo $line | grep -E "^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$"
    #versiune corecta: grep -E "^((25[0-9]|2[0-9][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(25[0-9]|2[0-9][0-9]|1[0-9][0-9]|[1-9]?[0-9])$"
done
