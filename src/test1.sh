#!/bin/bash
#SBATCH --output=test1.out


# Calcula os 1024 vizinhos mais próximos de 128 pontos de 300 dimensoes em base de dados de 400mil pontos
#Experiencia 1:
# Rodar o programa para APENAS 1 processo MPI e medir o tempo da computaçao de knn

for i in {1..10}  # 10 vezes cada
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np 1 ./main nq=128 npp=400000 d=300 k=1024
done