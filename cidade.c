#include "cidade.h"

#define TAMANHO_TABELA_HASH 6000
#define TAMANHO_MAX_STRING 256


// Função para criar uma nova tabela hash
CidadeHashTable* criaHashTable() {
    // Aloca memória para a tabela hash
    CidadeHashTable* hashTable = (CidadeHashTable*)malloc(sizeof(CidadeHashTable));
    if (hashTable != NULL) {
        // Inicializa cada posição da tabela com NULL
        int i;
        for (i = 0; i < TAMANHO_TABELA_HASH; i++) {
            hashTable->tabela[i] = NULL;
        }
    }
    return hashTable;
}

// Função para calcular o valor hash de uma chave (nome da cidade)
unsigned int calculaHash(const char* chave) {
    unsigned int hash = 0;
    while (*chave) {
        // Fórmula simples de hash, multiplicação e soma dos caracteres
        hash = (hash * 31) + *chave;
        chave++;
    }
    return hash % TAMANHO_TABELA_HASH;
}

// Função para inserir uma cidade na tabela hash
void insereCidade(CidadeHashTable* hashTable, Cidade* cidade) {
    // Calcula o valor hash da chave (nome da cidade)
    unsigned int index = calculaHash(cidade->nomeMunic);

    // Cria um novo nó de cidade
    CidadeNo* novoNo = (CidadeNo*)malloc(sizeof(CidadeNo));
    if (novoNo != NULL) {
        // Atribui a cidade ao novo nó
        novoNo->cidade = cidade;
        // Insere o novo nó no início da lista encadeada correspondente ao valor hash
        novoNo->prox = hashTable->tabela[index];
        hashTable->tabela[index] = novoNo;
    }
}

// Função para criar uma nova cidade a partir dos dados fornecidos
Cidade* criaCidade(const char* uf, const char* codUf, int codMunic, const char* nomeMunic, const char* populacao) {
    // Aloca memória para uma nova cidade
    Cidade* novaCidade = (Cidade*)malloc(sizeof(Cidade));
    if (novaCidade != NULL) {
        // Copia as strings para os campos correspondentes da cidade
        strcpy(novaCidade->uf, uf);
        strcpy(novaCidade->codUf, codUf);
        novaCidade->codMunic = codMunic;
        strcpy(novaCidade->nomeMunic, nomeMunic);
        strcpy(novaCidade->populacao, populacao);
    }
    return novaCidade;
}

// Função de comparação para o qsort, usado para ordenar as cidades
int compare(const void* a, const void* b) {
    const Cidade* cidadeA = (*(const Cidade**)a);
    const Cidade* cidadeB = (*(const Cidade**)b);
    return cidadeA->codMunic - cidadeB->codMunic;
}

// Função para ler as cidades do arquivo CSV
void lerCidades() {
    FILE* file = fopen("cidadesUTF-8.csv", "r");
    if (file == NULL) {
        printf("Falha ao abrir o arquivo!\n");
        return;
    }

    // Cria uma nova tabela hash para armazenar as cidades
    CidadeHashTable* hashTable = criaHashTable();
    // Cria um array para armazenar as cidades e seus índices
    Cidade** cidadeArray = NULL;
    int count = 0;

    char linha[TAMANHO_MAX_STRING];
    fgets(linha, TAMANHO_MAX_STRING, file); // Ignora a primeira linha (cabeçalho)

    while (fgets(linha, TAMANHO_MAX_STRING, file) != NULL) {
        char uf[3], codUf[3], nomeMunic[33], populacao[9];
        int codMunic;
        sscanf(linha, "%2s,%2s,%d,%32[^,],%8s", uf, codUf, &codMunic, nomeMunic, populacao);

        // Cria uma nova cidade com os dados lidos do arquivo
        Cidade* novaCidade = criaCidade(uf, codUf, codMunic, nomeMunic, populacao);
        if (novaCidade != NULL) {
            // Insere a cidade na tabela hash
            insereCidade(hashTable, novaCidade);

            // Realoca o array de cidades para acomodar a nova cidade
            cidadeArray = (Cidade**)realloc(cidadeArray, (count + 1) * sizeof(Cidade*));
            if (cidadeArray != NULL) {
                // Adiciona a nova cidade ao array
                cidadeArray[count] = novaCidade;
                count++;
            } else {
                // Em caso de falha na realocação do array, libera a memória da nova cidade e interrompe o loop
                free(novaCidade);
                break;
            }
        }
    }
    fclose(file);

    // Ordena o array de cidades usando a função qsort e a função de comparação
    qsort(cidadeArray, count, sizeof(Cidade*), compare);

    // Imprime as cidades ordenadas
    int i;
    for (i = 0; i < count; i++) {
        printf("Codigo: %d, Estado: %s, Municipio: %s\n", cidadeArray[i]->codMunic, cidadeArray[i]->uf, cidadeArray[i]->nomeMunic);
    }

    // Libera a memória alocada para as cidades e o array de cidades
    for (i = 0; i < count; i++) {
        free(cidadeArray[i]);
    }
    free(cidadeArray);
    // Libera a memória alocada para a tabela hash
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
