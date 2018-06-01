#!/bin/bash
timeLimit=2
red='\033[0;31m'
green='\033[0;32m'
reset='\033[0m'
cyan='\033[0;36m'

cd sample
cat /dev/null > diff.log
for i in $(ls | grep '\.in' | sed 's/\.in//g'); do 
# if ! timeout $timeLimit time -v ../main < $i.in > out.out; then
    if ! timeout $timeLimit time -f "%Es %Mkb\n" ../main < $i.in > out.out; then
        echo -e ${cyan}"TimeLimitExceeded on test $i.1"${reset};
        if ! timeout $timeLimit time -f "%Es %Mkb\n" ../main < $i.in > out.out; then
            echo -e ${cyan}"TimeLimitExceeded on test $i.2"${reset};
            if ! timeout $timeLimit time -f "%Es %Mkb\n" ../main < $i.in > out.out; then
                echo -e ${cyan}"TimeLimitExceeded on test $i.3"${reset};
                echo -e ${cyan}"TimeLimitExceeded on test $i"${reset};
                continue;
            fi
        fi
    fi
    if diff -Bb out.out $i.out >> diff.log; then
        echo -e ${green}"Sample $i Accepted!"${reset};
    else
        echo -e ${red}"WrongAnswer on test $i"${reset}; 
    fi
done
