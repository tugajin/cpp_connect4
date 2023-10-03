#!/bin/sh

for i in `seq 0 31`
do
    ./cpp_connect4 4500
    python3 train_trans.py $i
    rm -rf data/resolved*.json
    rm -rf data/selfplay*.json
done
