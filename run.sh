#!/bin/bash
export CPU="ryzen7900"
export MAXMEM=768
gcc -DUSENUMA -DUSEAFFINITY -O3 -Wall -pedantic pstream.c -o pstream -lnuma 
echo minimum
./pstream -l -t 1 -T 8 -M $MAXMEM -m 256 -z 64 -p 32768 -i 98 -r    -f results/$CPU
echo U
./pstream -l -t 1 -T 8 -M $MAXMEM -m 256 -z 64 -p 32768 -i 98 -r -U -f results/$CPU\U
echo a
./pstream -l -t 1 -T 8 -M $MAXMEM -m 256 -z 64 -p 32768 -i 98 -r -a -f results/$CPU\a
echo A
./pstream -l -t 1 -T 8 -M $MAXMEM -m 256 -z 64 -p 32768 -i 98 -r -A -f results/$CPU\A

