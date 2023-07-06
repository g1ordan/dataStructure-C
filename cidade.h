#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 255     // limite de caracteres de cada linha do arquivo
#define SIZE_HASH_TABLE 6000 // Define o 'size' da tabela hash

// Struct Cidade 
typedef struct dadosCidade
{
  char uf[3];
  char codUf[3];
  int codMunic;
  char nomeMunic[33]; // 'Vila Bela da Santissima Trindade' eh a cidade brasileira com o maior nome, tem 32 caracteres contando os espacos.
  char populacao[9];  // Sao Paulo tem 15 meloes de habitantes, sao 8 digitos
} Cidade;

// Estrutura para armazenar as cidades e seus índices
typedef struct {
    Cidade *cidade;
    int index;
} CidadeArrayItem;


typedef struct
{ 
  Cidade *cidade;
  struct cidadeNo *prox;
} CidadeNo;

typedef struct
{ // Tabela Hash
  CidadeNo *tabela[SIZE_HASH_TABLE];
} CidadeHashTable;

// Definicoes das funcoes
void LerCidades();
int InsereCidade(Cidade *cid);
void imprimeColisoes(CidadeHashTable *hashTable, unsigned int index);
void salvarColisoes(CidadeHashTable *hashTable);
Cidade *CriaCidade(char *uf, char *codUf, int codMunic, char *nomeMunic, char *populacao);
