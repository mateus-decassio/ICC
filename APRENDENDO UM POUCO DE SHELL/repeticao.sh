#!/bin/bash

for i in 1 2 3;
do
	echo "O número da vez será: ${i}";
done




for (( i = 0; i < 5; i++ )); do
	echo "A outra forma do laço é: ${i}"
done


echo "USANDO O RANGE:"

for numero in {1..10}
do
	echo "NÚMERO: ${numero}"
done

for letra in {A..F}
do
	echo "LETRA: ${letra}"
done


echo "USO DO WHILE:"
COUNTER=1;
while [[ ${COUNTER} -le 10 ]]; do
	echo "VOLTA DE NÚMERO ${COUNTER}"
	((COUNTER++));
done