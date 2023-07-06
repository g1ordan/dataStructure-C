# dataStructure-C

1) Baixar o arquivo XLS no link abaixo e converter para CSV (Guia Municípios) 
https://www.ibge.gov.br/estatisticas/sociais/populacao/9103-estimativas-depopulacao.html?=&t=resultados

2) (Peso 2) Fazer um programa em C que carregue o arquivo CSV para uma lista dispersa, 
tratando as colisões.

3) (Peso 1) Fazer outra estrutura de dados (no mesmo programa), usando os mesmos dados da 
lista dispersa. O seu programa deve ordenar os dados usando a função qsort da biblioteca 
stdlib.h
• Obs. São duas Estruturas distintas no mesmo programa. 
• https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm

4) (Peso 1) Criar uma rotina para testar todos os acessos aos dados, usando o método de Tabela 
Dispersa e Busca Binária. Para facilitar, vamos sempre usar o código do município. No caso da 
Busca binária, utilizar a função bsearch da biblioteca stdlib.h

• https://www.tutorialspoint.com/c_standard_library/c_function_bsearch.htm

5) (Peso 3) Implementar o algoritmo de busca binária de forma a contar o número execuções da 
função. Execute a busca binária por todos os códigos de municípios. Apresente o código, nome 
do município e quantos acessos foram necessários.

6) (Peso 2) Execute a busca por todos os códigos de municípios na tabela de dispersão. 
Apresente o código, nome do município e quantos acessos foram necessários. Quantos acessos 
foram necessários.

7) (Peso 1) Insira os novos registros abaixo. Onde foi inserido cada registro e qual é a 
quantidade de acessos para esse novo registro com busca binária e tabela hash?
• Código 6001, Município "Albatroz", RS, População 5.000
• Código 6001, Município "Borussia", SC, População 4.000
• Código 6001, Município "Palmital ", PR, População 3.000
• Código 6001, Município "Laranjeiras ", MT, População 2.000
