#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA_HASH 5581
#define TAMANHO_MAX_STRING 256
#define TAMANHO_MAX_CIDADES 6000

// Definição da Struct Cidade 
typedef struct dadosCidade
{
  char uf[3];
  int codUf;
  int codMunic;
  char nomeMunic[33]; 
  char populacao[9];  
} Cidade;

// Definição da struct de um nó de cidade na tabela hash
typedef struct CidadeNo {
    Cidade* cidade;
    struct CidadeNo* prox;
} CidadeNo;

// Definição da struct da tabela hash
typedef struct CidadeHashTable {
    CidadeNo* tabela[TAMANHO_TABELA_HASH];
} CidadeHashTable;

// Definicoes das funcoes
CidadeHashTable* criaHashTable();
int calculaHash(int codMunic);
void insereCidade(CidadeHashTable* hashTable, Cidade* cidade);
Cidade* criaCidade(const char* uf, int codUf, int codMunic, const char* nomeMunic, const char* populacao);
void dadosTabelaDispersa(CidadeHashTable* hashTable, int codMunic, int codUf);
void todosDadosTabelaDispersa(CidadeHashTable* hashTable);
int compare(const void* a, const void* b);
void buscaBinaria(int buscaCodigo, Cidade** cidadeArray, int count);
void buscaBinariaPorTodosDados(Cidade** cidadeArray, int count);
void lerCidades(CidadeHashTable* hashTable, Cidade** cidadeArray, int* countPtr);
void novosRegistros(CidadeHashTable* hashTable, Cidade** cidadeArray, int* countPtr);
void chamaMenu();
