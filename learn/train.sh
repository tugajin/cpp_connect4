#!/bin/sh

for i in `seq 0 31`
do
    ./cpp_connect4 4500
    rm -rf data/resolved*.json
    python3 train.py $i
    rm -rf data/selfplay*.json
done
