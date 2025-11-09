#!/bin/bash
#SBATCH --output=test3.out
#SBATCH --nodes=4
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=2
#SBATCH --exclusive

# Calcula os 1024 vizinhos mais próximos de 128 pontos de 300 dimensoes em base de dados de 400mil pontos
#Experiencia 1:
# Rodar o programa para um total de 8 processos MPI em 4 hosts diferentes e medir o tempo da
#computaçao de knn (ou seja, serão 2 processo por host)

for i in {1..10}  # 10 vezes cada
do
    echo "-------- EXECUCAO $i --------"
    mpirun -np 8 ./main nq=128 npp=400000 d=300 k=1024
done
