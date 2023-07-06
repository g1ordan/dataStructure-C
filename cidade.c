#include "cidade.h"
#define SIZE_HASH_TABLE 6000 // Define o 'size' da tabela hash

CidadeHashTable *criaHashTable() 
{
    CidadeHashTable *hashTable = (CidadeHashTable *)malloc(sizeof(CidadeHashTable));
    int i;
    for (i = 0; i < SIZE_HASH_TABLE; i++)
    {
        hashTable->tabela[i] = NULL;
    }
    return hashTable;
}

unsigned int calculaHash(char *chave) // Calcula o valor da hash, percorrendo cada caractere
{
    unsigned int hash = 5381; // Numero primo mais comum
    int c;

    while ((c = *chave++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % SIZE_HASH_TABLE;
}

void insereCidade(CidadeHashTable *hashTable, Cidade *cidade)
{
    unsigned int index = calculaHash(cidade->nomeMunic);

    CidadeNo *novoNo = (CidadeNo *)malloc(sizeof(CidadeNo));
    novoNo->cidade = cidade;
    novoNo->prox = NULL;

    if (hashTable->tabela[index] == NULL)
    {
        // Insere no inicio caso não haja no na posiçao da tabela hash
        hashTable->tabela[index] = novoNo;
    }
    else
    {
        // Percorre a lista e insere no final caso exista um no na posição da tabela hash
        CidadeNo *atual = hashTable->tabela[index];
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }
}

// Pega cada linha do csv e transforma em um objeto dentro da memoria, provando que C pode ser orietado a objetos
Cidade *CriaCidade(char *uf, char *codUf, int codMunic, char *nomeMunic, char *populacao)
{
    Cidade *novaCidade = (Cidade *)malloc(sizeof(Cidade));
    strcpy(novaCidade->uf, uf);
    strcpy(novaCidade->codUf, codUf);
    novaCidade->codMunic = codMunic;
    strcpy(novaCidade->nomeMunic, nomeMunic);
    strcpy(novaCidade->populacao, populacao);
    return novaCidade;
}

//void LerCidades()
//{
//    FILE *file = fopen("cidades.csv", "r");
//    if (file == NULL)
//    {
//        printf("Erro ao abrir o arquivo!\n");
//        return;
//    }
//
//    CidadeHashTable *hashTable = criaHashTable();
//
//    char buf[SIZE_HASH_TABLE];
//    char *tmp;
//    int count = 0;
//
//    while (fgets(buf, MAX_STR_LEN, file) != NULL)
//    {
//        if ((strlen(buf) > 0) && (buf[strlen(buf) - 1] == '\n'))
//        {
//            buf[strlen(buf) - 1] = '\0';
//        }
//
//        tmp = strtok(buf, ",");
//        char uf[3];
//        strcpy(uf, tmp);
//
//        tmp = strtok(NULL, ",");
//        char codUf[3];
//        strcpy(codUf, tmp);
//
//        tmp = strtok(NULL, ",");
//        char codMunic[6];
//        strcpy(codMunic, tmp);
//
//        tmp = strtok(NULL, ",");
//        char nomeMunic[33];
//        strcpy(nomeMunic, tmp);
//
//        tmp = strtok(NULL, ",");
//        char populacao[9];
//        strcpy(populacao, tmp);
//
//        if (count > 0) // Ignora a primeira linha do arquivo (cabecalho)
//        {
//            Cidade *novaCidade = CriaCidade(uf, codUf, codMunic, nomeMunic, populacao);
//            insereCidade(hashTable, novaCidade);
//        }
//        count++;
//    }
//    fclose(file);
//    // imprimeCidades(hashTable); Chamada da funcao para imprimir as cidades
//
//    int i;
//    for (i = 0; i < SIZE_HASH_TABLE; i++)
//    {
//        if (hashTable->tabela[i] != NULL)
//        {
//            imprimeColisoes(hashTable, i);
//            printf("\n");
//        }
//    }
//}

 //Função de comparação para qsort
int compare(const void *a, const void *b) {
    CidadeArrayItem *cidadeA = (CidadeArrayItem *)a;
    CidadeArrayItem *cidadeB = (CidadeArrayItem *)b;
    
    if(cidadeA->cidade->codMunic < cidadeB->cidade->codMunic)
    return -1;
    if(cidadeB->cidade->codMunic > cidadeA->cidade->codMunic)
    return -1;
}


void LerCidades() {
    FILE *file = fopen("cidadesUTF-8.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    CidadeHashTable *hashTable = criaHashTable();

    char buf[MAX_STR_LEN];
    char *tmp;
    int count = 0;

    // Array para armazenar as cidades e seus índices
    CidadeArrayItem cidadeArray[SIZE_HASH_TABLE];

    while (fgets(buf, MAX_STR_LEN, file) != NULL) {
        if ((strlen(buf) > 0) && (buf[strlen(buf) - 1] == '\n')) {
            buf[strlen(buf) - 1] = '\0';
        }

        tmp = strtok(buf, ",");
        char uf[3];
        strcpy(uf, tmp);

        tmp = strtok(NULL, ",");
        char codUf[3];
        strcpy(codUf, tmp);

        tmp = strtok(NULL, ",");
        int codMunic;
        codMunic = atoi(tmp);

        tmp = strtok(NULL, ",");
        char nomeMunic[33];
        strcpy(nomeMunic, tmp);

        tmp = strtok(NULL, ",");
        char populacao[9];
        strcpy(populacao, tmp);

        if (count > 0) // Ignora a primeira linha do arquivo (cabeçalho)
        {
            Cidade *novaCidade = CriaCidade(uf, codUf, codMunic, nomeMunic, populacao);
            insereCidade(hashTable, novaCidade);

            // Preenche o array de CidadeArray
            cidadeArray[count - 1].cidade = novaCidade;
            cidadeArray[count - 1].index = count - 1;
        }
        count++;
    }
    fclose(file);

    // Ordena o array de cidades usando a função qsort
    qsort(cidadeArray, count - 1, sizeof(CidadeArrayItem), compare);

    // Imprime as cidades ordenadas
    int i;
    for (i = 0; i < count - 1; i++) {
        printf("Codigo: %d, Estado: %s, Municipio: %s,\n", cidadeArray[i].cidade->codMunic, cidadeArray[i].cidade->uf, cidadeArray[i].cidade->nomeMunic);
    }

    // Liberar a memória alocada
    for (i = 0; i < count - 1; i++) {
        free(cidadeArray[i].cidade);
    }
    free(hashTable);
}




//void imprimeColisoes(CidadeHashTable *hashTable, unsigned int index) // Para imprimir colisÃµes e verificar se o valor do hash corresponde
//{
//    printf("COLISOES IDENTIFICADAS NO INDICE %u:\n", index);
//
//    CidadeNo *atual = hashTable->tabela[index];
//
//    while (atual != NULL)
//    {
//        Cidade *cidade = atual->cidade;
//        unsigned int hash = calculaHash(cidade->nomeMunic);
//
//        if (hash == index)
//        {
//            printf("Municipio: %s, Estado: %s\n", cidade->nomeMunic, cidade->uf);
//        }
//        else
//        {
//            printf("Municipio com hash incorreto - indice esperado: %u, indice real: %u\n", index, hash);
//        }
//
//        atual = atual->prox;
//    }
//}

int InsereCidade(Cidade *cid)
{
    printf("\n %s %s %d %s %s", cid->uf, cid->codUf, cid->codMunic, cid->nomeMunic, cid->populacao);

    return 1;
}

/*
void imprimeCidades(CidadeHashTable *hashTable) { // Para imprimir as cidades com todos os dados
    int i;
    for (i = 0; i < TAM_HASH_TABLE; i++) {
        CidadeNo *atual = hashTable->tabela[i];
        while (atual != NULL) {
            Cidade *cidade = atual->cidade;
            printf("UF: %s\n", cidade->uf);
            printf("Codigo UF: %s\n", cidade->codUf);
            printf("Codigo Municipio: %s\n", cidade->codMunic);
            printf("Nome Municipio: %s\n", cidade->nomeMunic);
            printf("Populacao: %s\n", cidade->populacao);
            printf("----------------------\n");
            atual = atual->prox;
        }
    }
}
*/
