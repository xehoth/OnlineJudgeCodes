#!/bin/bash
./build.sh
cd sample
cat /dev/null > diff.log
for i in $(ls | grep '\.in' | sed 's/\.in//g'); do 
    time ../main < $i.in > out.out; 
    if diff -Bb out.out $i.out >> diff.log; then
        echo Sample $i 'Accepted!';
    else
        echo 'WrongAnswer on test '$i; 
    fi
done
