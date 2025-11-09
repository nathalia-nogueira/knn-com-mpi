# KNN com MPI
Este projeto foi desenvolvido como trabalho da disciplina "Programação Paralela", ofertada na Universidade Federal do Paraná em 2025/2.

## Estrutura de pastas
O projeto está dividido em quatro pastas principais:
- espec: especificação do trabalho fornecida pelo professor da disciplina
- docs: documentação criada e utilizada no desenvolvimento
- resultados: resultados das experiências realizadas
- src: código-fonte completo

## Compilação
A compilação pode ser feita com o comando ` make `, que criará o executável _main_. 

## Execução
Um exemplo de execução é: 
    `mpirun -np n1 ./main nq=n2 npp=n3 d=n4 k=n5 `

Em que:
*-np indica quantos processos MPI serão utilizados
*nq=n2 indica a quantidade de pontos no conjunto Q (conjunto principal)
*npp=n3 indica a quantidade de pontos no conjunto P (conjunto de consulta)
*d=n4 indica a dimensionalidade de cada um dos pontos
*k=n5 indica quantos vizinhos devem ser retornados

Além disso, em /src/, existem scripts de teste prontos, que podem ser executados em ambiente linux com:
`bash <nome_do_script>`