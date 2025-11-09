// mpirun -np 4 ./main nq=n1 npp=n2 d=n3 k=n4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "knn.h"
#include "arrayUtils.h"

#define PRINT_DATA 0

int main(int argc, char** argv) {
    int nProc, procID;
    int root = 0;
    int params[4];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);

    if (procID == root) {
        if (argc != 5) {
            printf("Uso correto: mpirun -np 4 ./main nq=n1 npp=n2 d=n3 k=n4\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        params[0] = params[1] = params[2] = params[3] = 0;    
    
        char parameterName[1024];  
        for (int i = 1; i < argc; i++) {
            char* eq_pointer = strchr(argv[i], '=');
            if (eq_pointer) {
                strcpy(parameterName, argv[i]);
                parameterName[eq_pointer - argv[i]] = '\0';
                int value = atoi(++eq_pointer);
                if (!strcmp(parameterName, "nq")) 
                    params[0] = value;
                else if (!strcmp(parameterName, "npp"))
                    params[1] = value;
                else if (!strcmp(parameterName, "d"))
                    params[2] = value;
                else if (!strcmp(parameterName, "k"))
                    params[3] = value;
            }
            
        }

        if (!params[0] || !params[1] || !params[2] || !params[3]) {
            printf("Uso correto: mpirun -np 4 ./main nq=n1 npp=n2 d=n3 k=n4\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(params, 4, MPI_INT, root, MPI_COMM_WORLD);

    int totalSize_P = params[1] * params[2]; // numero total de elementos do conjunto P
    int totalSize_Q = params[0] * params[2]; // numero total de elementos do conjunto Q
    int totalSize_R = params[0] * params[3]; // numero total de elementos do conjunto R
    int numPoints = params[0] / nProc; 
    int localSize_Q = numPoints * params[2]; // Tamanho de cada sub-array a ser distribuído
    int localSize_R = numPoints * params[3]; // numero de elementos do conjunto R de cada rank

    // Buffers
    float* sendBuf_Q = NULL; // dados enviados pelo scatter
    float* sendBuf_P = (float*)malloc(totalSize_P * sizeof(float)); // dados enviados por broadcast
    float* recvBufScatter_Q = (float*)malloc(localSize_Q * sizeof(float)); // dados recebidos pelo scatter
    int* recvBufGather_R = NULL; // dados recebidos pelo gather

    // O processo raiz inicializa o buffer de envio com dados
    if (procID == root) {

        srand(time(NULL));

        sendBuf_Q = (float*) malloc(totalSize_Q * sizeof(float));
        if (!sendBuf_Q) {
            printf("[allocateArray] error in malloc.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        geraConjuntoDeDados(sendBuf_Q, params[0], params[2]);  
        
        geraConjuntoDeDados(sendBuf_P, params[1], params[2]);  
    
        #if PRINT_DATA
            printf("Processo raiz (rank %d) enviando o buffer: [", procID);
            for (int i = 0; i < totalSize_Q; i++) {
                printf("%.2f ", sendBuf_Q[i]);
            }
            printf("]\n");
        #endif

        // O buffer de coleta só é necessário no processo raiz
        recvBufGather_R = (int*)malloc(totalSize_R * sizeof(int));
    }

    double sendT0 = 0;
    if (procID == root) {
        sendT0 = MPI_Wtime();
    }

    MPI_Bcast(sendBuf_P, totalSize_P, MPI_FLOAT, root, MPI_COMM_WORLD);
    
    if (procID == root) {
        double sendT1 = MPI_Wtime();
        printf("Rank %d -> tempo de transmissão do conjunto P via MPI_Bcast = %.2f\n", procID, sendT1 - sendT0);
    }

    if (procID == root) {
        sendT0 = MPI_Wtime();
    }
    // enviando dados
    // ---- MPI_Scatter ----
    // Distribui o sendbuf do root para os buffers recvbuf_scatter de cada processo
    MPI_Scatter(sendBuf_Q, // Buffer de envio (somente no root)
                localSize_Q, // Número de elementos a enviar para cada processo
                MPI_FLOAT,    // Tipo de dado
                recvBufScatter_Q, // Buffer de recebimento (em todos os processos)
                localSize_Q, // Número de elementos a receber
                MPI_FLOAT,    // Tipo de dado
                root,       // Rank do processo root
                MPI_COMM_WORLD);
    
    if (procID == root) {
        double sendT1 = MPI_Wtime();
        printf("Rank %d -> tempo de transmissão do conjunto Q via MPI_Scatter: %.2f s\n", procID, sendT1 - sendT0);
    }

    #if PRINT_DATA 
        // imprime dados recebidos
        printf("Processo %d recebeu: paraemtros: nq= %d , npp= %d , d= %d , k= %d \n", procID, params[0], params[1], params[2], params[3]);

        printf("Processo %d recebeu: Q: \n", procID);
        for (int i = 0; i < numPoints; i++) {
            for (int j = 0; j < params[2]; j++) 
                printf("%.2f ", recvBufScatter_Q[i * params[2] + j]);
            printf("\n");
        }

        printf("Processo %d recebeu: P: \n", procID);
        for (int i = 0; i < params[1]; i++) {
            for (int j = 0; j < params[2]; j++) 
                printf("%.2f ", sendBuf_P[i * params[2] + j]);
            printf("\n");
        }
    #endif


    int *result = allocateZeroedIntArray(localSize_R);

    MPI_Barrier(MPI_COMM_WORLD);

    double knnT0 = MPI_Wtime();
    result = knn(recvBufScatter_Q, numPoints, sendBuf_P, params[1], params[2], params[3]);
    double knnT1 = MPI_Wtime();
    printf("Rank %d -> totalTimeInSeconds: %lf s\n", procID, knnT1 - knnT0);

    #if PRINT_DATA
        printf("Processo %d: k nearest neighbors (indices in P):\n", procID);
        for (int i = 0; i < numPoints; i++) {
            printf("Q[%d]: ", i);
            for (int j = 0; j < params[3]; j++)
                printf("%d ", result[i * params[3] + j]);
            printf("\n");
        }
        printf("\n");
    #endif
    
    double receiveT0 = 0;
    if (procID != root) {
        receiveT0 = MPI_Wtime();
    }
    // depois de processados, envia os dados para o root
    // ---- MPI_Gather ----
    // Coleta os resultados de todos os processos para o recvBufGather_R no processo raiz
    MPI_Gather(result, // Buffer de envio (em todos os processos)
               localSize_R,      // Número de elementos a enviar
               MPI_FLOAT,         // Tipo de dado
               recvBufGather_R,  // Buffer de recebimento (somente no root)
               localSize_R,      // Número de elementos a receber
               MPI_INT,         // Tipo de dado
               root,            // Rank do processo root
               MPI_COMM_WORLD);
    
    if (procID != root) {
        double receiveT1 = MPI_Wtime();
        printf("Rank %d -> tempo de transmissão de result via MPI_Gather: %lf s\n", procID, receiveT1 - receiveT0);
    }
    
    #if PRINT_DATA
        // ---- Resultados ----
        if (procID == root) {
            printf("\nProcesso raiz (rank %d) após gather, buffer: [", procID);
            for (int i = 0; i < totalSize_R; i++) {
                printf("%d ", recvBufGather_R[i]);
            }
            printf("]\n\n");
        }
    #endif

    // Libera a memória
    if (procID == root) {
        free(sendBuf_Q);
        free(recvBufGather_R);
    }
    free(sendBuf_P);
    free(recvBufScatter_Q);

    MPI_Finalize();
}