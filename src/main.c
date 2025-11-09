/* Função principal de KNN paralelizado com MPI
 * Autoras:
 * - Bianca Mendes Francisco
 * - Nathália Nogueira Alves
 *  
 * Exemplo de linha de comando:
 * mpirun -np 4 ./main nq=n1 npp=n2 d=n3 k=n4
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "knn.h"
#include "arrayUtils.h"

#define PRINT_DATA 0
#define PRINT_RESULT 0

int main(int argc, char** argv) {

    int nProc, procID;
    int root = 0;
    int params[4]; // Parametros da main
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);

    // Verifica se os parametros estao todos corretos
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

    // Envia os parametros para todos os ranks por broadcast
    MPI_Bcast(params, 4, MPI_INT, root, MPI_COMM_WORLD);

    int* sendBuf_counts = NULL; // Quantidade de pontos que cada rank vai receber (somente no root)
    int* rankSizes_Q = NULL; // Quantidade de elementos que cada rank vai receber do conjunto Q (somente no root)
    int* rankSizes_R = NULL; // Quantidade de elementos do conjunto R de cada rank (somente no root)
    int* displacements_Q = NULL; // Array de deslocamentos do Q (somente no root)
    int* displacements_R = NULL; // Array de deslocamentos do r (somente no root)

    // Processo root define quantos pontos cada rank vai receber
    if (procID == root) {

        sendBuf_counts = (int*)malloc(nProc * sizeof(int));
        rankSizes_Q = (int*)malloc(nProc * sizeof(int));
        rankSizes_R = (int*)malloc(nProc * sizeof(int));
        if (!sendBuf_counts || !rankSizes_Q || !rankSizes_R) {
            printf("[allocateArray] error in malloc.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int qnt = params[0] / nProc; // Divisão inteira
        for (int i = 0; i < nProc; i++) 
            sendBuf_counts[i] = qnt;
        
        int mod = params[0] % nProc; 
        for (int i = 0; i < mod; i++) // Distribui o resto
            sendBuf_counts[i]++;

        // Define vetores rankSizes
        for (int i = 0; i < nProc; i++) {
            rankSizes_Q[i] = sendBuf_counts[i] * params[2];
            rankSizes_R[i] = sendBuf_counts[i] * params[3];
        }

        // for (int i = 0; i < nProc; i++) {
        //    printf("Processo %d = %d pontos\n", i, sendBuf_counts[i]);
        //    printf("Processo %d = %d tam vetor Q\n", i, rankSizes_Q[i]);
        //    printf("Processo %d = %d tam vetor R\n", i, rankSizes_R[i]);
        // }
    
    }

    int totalSize_P = params[1] * params[2]; // numero total de elementos do conjunto P
    int totalSize_Q = params[0] * params[2]; // numero total de elementos do conjunto Q
    int totalSize_R = params[0] * params[3]; // numero total de elementos do conjunto R

    int numPoints; // Quantidade de pontos do rank

    // Distribui as quantidades de pontos para cada rank
    MPI_Scatter(sendBuf_counts, 1, MPI_INT,
                &numPoints, 1, MPI_INT,
                root, MPI_COMM_WORLD);

    int localSize_Q = numPoints * params[2]; // Número de elementos do conjunto Q do rank
    int localSize_R = numPoints * params[3]; // Número de elementos do conjunto R do rank

    // Buffers de dados
    float* sendBuf_Q = NULL; // Conjunto Q enviado pelo scatter (somente no root)
    float* sendBuf_P = (float*)malloc(totalSize_P * sizeof(float)); // Conjunto P enviado por broadcast
    float* recvBufScatter_Q = (float*)malloc(localSize_Q * sizeof(float)); // Conjunto Q recebido pelo scatter
    int* recvBufGather_R = NULL; // Conjunto R recebido pelo gather (somente no root)

    // Processo root inicializa os buffers de envio com dados
    if (procID == root) {

        srand(time(NULL)); // Semente do rand

        sendBuf_Q = (float*) malloc(totalSize_Q * sizeof(float));
        if (!sendBuf_Q) {
            printf("[allocateArray] error in malloc.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        // Preenche conjunto Q com dados aleatórios
        geraConjuntoDeDados(sendBuf_Q, params[0], params[2]); 
        // Preenche conjunto P com dados aleatórios
        geraConjuntoDeDados(sendBuf_P, params[1], params[2]);  
    
        #if PRINT_DATA
            printf("Processo raiz (rank %d) enviando o buffer: [", procID);
            for (int i = 0; i < totalSize_Q; i++) {
                printf("%.2f ", sendBuf_Q[i]);
            }
            printf("]\n");
        #endif

        
        recvBufGather_R = (int*)malloc(totalSize_R * sizeof(int));
        displacements_Q = (int*)malloc(nProc * sizeof(int));
        displacements_R = (int*)malloc(nProc * sizeof(int));
        if (!recvBufGather_R || !displacements_Q || !displacements_R) {
            printf("[allocateArray] error in malloc.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        // Calcula os deslocamentos
        // Proc 0 começa no índice 0
        displacements_Q[0] = 0; 
        displacements_R[0] = 0; 
        for (int i = 1; i < nProc; i++) {
            // O próximo começa onde o anterior terminou
            displacements_Q[i] = displacements_Q[i-1] + rankSizes_Q[i-1];
            displacements_R[i] = displacements_R[i-1] + rankSizes_R[i-1];
        }
    }

    // Sincroniza ranks
    MPI_Barrier(MPI_COMM_WORLD);

    // Mede o tempo de transmissão do conjunto P
    double sendT0 = 0;
    if (procID == root) {
        sendT0 = MPI_Wtime();
    }

    // Envia o conjunto P para todos os ranks por broadcast
    MPI_Bcast(sendBuf_P, totalSize_P, MPI_FLOAT, root, MPI_COMM_WORLD);
    
    if (procID == root) {
        double sendT1 = MPI_Wtime();
        printf("Rank %d -> tempo de transmissão do conjunto P via MPI_Bcast = %lf\n", procID, sendT1 - sendT0);
    }

    // Mede o tempo de transmissão do conjunto Q
    if (procID == root) {
        sendT0 = MPI_Wtime();
    }
    
    // Distribui o conjunto Q para cada rank
    MPI_Scatterv(sendBuf_Q,         // Buffer de envio (somente no root)
                rankSizes_Q,        // Array com número de elementos a enviar para cada processo (somente no root)
                displacements_Q,    // Array de deslocamentos (somente no root)
                MPI_FLOAT,          // Tipo de dado
                recvBufScatter_Q,   // Buffer de recebimento (em todos os processos)
                localSize_Q,        // Número de elementos a receber
                MPI_FLOAT,          // Tipo de dado
                root,               // Rank do processo root
                MPI_COMM_WORLD);
    
    if (procID == root) {
        double sendT1 = MPI_Wtime();
        printf("Rank %d -> tempo de transmissão do conjunto Q via MPI_Scatter: %lf s\n", procID, sendT1 - sendT0);
    }

    #if PRINT_DATA 
        // imprime dados recebidos
        printf("Processo %d recebeu: parametros: nq= %d , npp= %d , d= %d , k= %d \n", procID, params[0], params[1], params[2], params[3]);

        printf("Processo %d recebeu: Q: \n", procID);
        for (int i = 0; i < numPoints; i++) {
            for (int j = 0; j < params[2]; j++) 
                printf("%.2f ", recvBufScatter_Q[i * params[2] + j]);
            printf("\n");
        }
        printf("\n");

        printf("Processo %d recebeu: P: \n", procID);
        for (int i = 0; i < params[1]; i++) {
            for (int j = 0; j < params[2]; j++) 
                printf("%.2f ", sendBuf_P[i * params[2] + j]);
            printf("\n");
        }
        printf("\n");
    #endif

    // Vetor de resultados do knn
    int *result = allocateZeroedIntArray(localSize_R);

    // Mede o tempo do knn
    double knnT0 = MPI_Wtime();
    
    result = knn(recvBufScatter_Q, numPoints, sendBuf_P, params[1], params[2], params[3]);
    
    double knnT1 = MPI_Wtime();
    printf("Rank %d -> totalTimeInSeconds: %lf s\n", procID, knnT1 - knnT0);

    #if PRINT_RESULT
        printf("Processo %d: k vizinhos mais proximos (indices in P):\n", procID);
        for (int i = 0; i < numPoints; i++) {
            printf("Q[%d]: ", i);
            for (int j = 0; j < params[3]; j++)
                printf("%d ", result[i * params[3] + j]);
            printf("\n");
        }
        printf("\n");
    #endif
    
    // Mede o tempo do gather dos resultados
    double receiveT0 = 0;
    receiveT0 = MPI_Wtime();
    
    // Coleta os resultados de todos os ranks para o root
    MPI_Gatherv(result,         // Buffer de envio (em todos os processos)
               localSize_R,     // Número de elementos a enviar
               MPI_INT,         // Tipo de dado
               recvBufGather_R, // Buffer de recebimento (somente no root)
               rankSizes_R,     // Array com número de elementos a receber de cada processo (somente no root)
               displacements_R, // Array de deslocamentos (somente no root)
               MPI_INT,         // Tipo de dado
               root,            // Rank do processo root
               MPI_COMM_WORLD);
    
    double receiveT1 = MPI_Wtime();
    printf("Rank %d -> tempo de transmissão de result via MPI_Gather: %lf s\n", procID, receiveT1 - receiveT0);
        
    #if PRINT_RESULT
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
        free(sendBuf_counts);
        free(rankSizes_Q);
        free(rankSizes_R);
        free(displacements_Q);
        free(displacements_R);
    }
    free(sendBuf_P);
    free(recvBufScatter_Q);

    MPI_Finalize();
}