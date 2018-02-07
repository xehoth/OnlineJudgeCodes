#!/bin/bash
red='\033[0;31m'
green='\033[0;32m'
reset='\033[0m'
cyan='\033[0;36m'

./build.sh
cd sample
cat /dev/null > diff.log
for i in $(ls | grep '\.in' | sed 's/\.in//g'); do 
    time ../main < $i.in > out.out; 
    if diff -Bb out.out $i.out >> diff.log; then
        echo -e ${green}"Sample $i Accepted!"${reset};
    else
        echo -e ${red}"WrongAnswer on test $i"${reset}; 
    fi
done