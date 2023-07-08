#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA_HASH 6000
#define TAMANHO_MAX_STRING 256

// Struct Cidade 
typedef struct dadosCidade
{
  char uf[3];
  char codUf[3];
  int codMunic;
  char nomeMunic[33]; 
  char populacao[9];  
} Cidade;

// Definição da estrutura de um nó de cidade na tabela hash
typedef struct CidadeNo {
    Cidade* cidade;
    struct CidadeNo* prox;
} CidadeNo;

// Definição da estrutura da tabela hash
typedef struct CidadeHashTable {
    CidadeNo* tabela[TAMANHO_TABELA_HASH];
} CidadeHashTable;


// Definicoes das funcoes
CidadeHashTable* criaHashTable();
unsigned int calculaHash(const char* chave);
void insereCidade(CidadeHashTable* hashTable, Cidade* cidade);
Cidade* criaCidade(const char* uf, const char* codUf, int codMunic, const char* nomeMunic, const char* populacao);
int compare(const void* a, const void* b);
void lerCidades();
//void imprimeColisoes(CidadeHashTable *hashTable, unsigned int index);

