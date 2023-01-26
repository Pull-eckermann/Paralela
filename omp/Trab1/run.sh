#!/bin/bash

rm *.log

make

for a in {2..8}; do
    echo "" > sequencial.log
    echo "" > paralelo.log

    export OMP_NUM_THREADS=$a

    echo -n "EXECUTANDO COM " 
    echo -n $a  
    echo " THREADS"

    echo "IMPLEMENTAÇÃO SEQUENCIAL x50"
    for i in {1..50}; do
        ./cavalo_serial >> sequencial.log
    done
    awk '{ sum += $0 } END { print sum/NR }' sequencial.log

    echo "IMPLEMENTAÇÃO PARALELO x50"
    for i in {1..50}; do
        ./cavalo_paralelo >> paralelo.log
    done
    awk '{ sum += $0 } END { print sum/NR }' paralelo.log
done

make clean