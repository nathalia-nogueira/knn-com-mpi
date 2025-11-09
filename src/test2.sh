#!/bin/bash
#SBATCH --output=test2.out
#SBATCH --ntasks=8
#SBATCH --exclusive

# Calcula os 1024 vizinhos mais próximos de 128 pontos de 300 dimensoes em base de dados de 400mil pontos
#Experiencia 2:
# Rodar o programa para 8 processos MPI no mesmo host e medir o tempo da computaçao de knn

for i in {1..10}  # 10 vezes cada
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np 8 ./main nq=128 npp=400000 d=300 k=1024
done
