#include "cidade.h"

#define TAMANHO_TABELA_HASH 10000
#define TAMANHO_MAX_STRING 256


// Função para criar uma nova tabela hash
CidadeHashTable* criaHashTable() {
    
    CidadeHashTable* hashTable = (CidadeHashTable*)calloc(1, sizeof(CidadeHashTable));
    if (hashTable != NULL) {
        
        int i;
        for (i = 0; i < TAMANHO_TABELA_HASH; i++) {
            hashTable->tabela[i] = NULL;
        }
    }
    return hashTable;
}

//O uso do tipo "unsigned int" é comum para garantir que o valor retornado seja sempre positivo.
unsigned int calculaHash(int codMunic) {
    unsigned int hash = 0;
    unsigned int chave = (unsigned int)codMunic;

    while (chave) {
        hash = ((hash << 5) + hash) ^ (chave & 0xFF);
        chave >>= 8;
    }

    return hash % TAMANHO_TABELA_HASH;
}

void insereCidade(CidadeHashTable* hashTable, Cidade* cidade) {
    unsigned int indice = calculaHash(cidade->codMunic);

    CidadeNo* novoNo = (CidadeNo*)malloc(sizeof(CidadeNo));

    if (novoNo != NULL) {
        // Atribui a cidade ao novo nó
        novoNo->cidade = cidade;
        novoNo->prox = NULL;

        // Verifica se a posição da tabela hash está vazia
        if (hashTable->tabela[indice] == NULL) {
            // Se estiver vazia, insere o novo nó diretamente na posição
            hashTable->tabela[indice] = novoNo;
        } else {
            // Caso contrário, ocorreu uma colisão
            printf("Colisão encontrada no índice %u:\n", indice);

            // Insere o novo nó como próximo do último nó da lista
            CidadeNo* ultimo = hashTable->tabela[indice];
            while (ultimo->prox != NULL) {
                ultimo = ultimo->prox;
            }
            ultimo->prox = novoNo;
        }
    }
}

Cidade* criaCidade(const char* uf, const char* codUf, int codMunic, const char* nomeMunic, const char* populacao) {
    
    Cidade* novaCidade = (Cidade*)malloc(sizeof(Cidade));
    if (novaCidade != NULL) {
        strcpy(novaCidade->uf, uf);
        strcpy(novaCidade->codUf, codUf);
        novaCidade->codMunic = codMunic;
        strcpy(novaCidade->nomeMunic, nomeMunic);
        strcpy(novaCidade->populacao, populacao);
    }
    return novaCidade;
}

void dadosTabelaDispersa(CidadeHashTable* hashTable, int codMunic) {
    unsigned int indice = calculaHash(codMunic);

    CidadeNo* atual = hashTable->tabela[indice];
    while (atual != NULL) {
        if (atual->cidade->codMunic == codMunic) {
            printf("\nDados encontrados através da tabela dispersa:\n");
            printf("Cod. Município: %d, Município: %s, Estado: %s\n\n", atual->cidade->codMunic, atual->cidade->nomeMunic, atual->cidade->uf);
            return; // Encontrou o resultado, encerra a função
        }
        atual = atual->prox;
    }

    printf("Falha ao acessar os dados.\n");
}

//Função de comparação para o qsort, usado para ordenar as cidades
/*Os parâmetros const void* a e const void* b são ponteiros genéricos (void*) para os elementos a serem comparados.
 Eles são declarados como "const" para indicar que o conteúdo apontado por eles não será modificado pela função de comparação*/
int compare(const void* a, const void* b) {
    const Cidade* cidadeA = (*(const Cidade**)a);
    const Cidade* cidadeB = (*(const Cidade**)b);
    return cidadeA->codMunic - cidadeB->codMunic;
}

void dadosBuscaBinaria(Cidade** cidadeArray, int count, int codMunic) {
    int inicio = 0;
    int fim = count - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int valorMeio = cidadeArray[meio]->codMunic;

        if (valorMeio == codMunic) {
            printf("Dados encontrados através da busca binária:\n");
            printf("Cod. Município: %d, Município: %s, Estado: %s\n\n", cidadeArray[meio]->codMunic, cidadeArray[meio]->nomeMunic, cidadeArray[meio]->uf);
            return;
        } else if (valorMeio < codMunic) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
	printf("Falha ao acessar os dados.\n");
}

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
    fgets(linha, TAMANHO_MAX_STRING, file); // Não acessa a primeira linha

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

    printf("\nLeitura e inserção de cidades concluídas.\n\n");

    // Ordena o array de cidades usando a função qsort e a função de comparação
    qsort(cidadeArray, count, sizeof(Cidade*), compare);

    int i;
    printf("\nCidades ordenadas pelo código do Município:\n\n");
    for (i = 0; i < count; i++) {
        printf("Codigo: %d, Estado: %s, Municipio: %s\n", cidadeArray[i]->codMunic, cidadeArray[i]->uf, cidadeArray[i]->nomeMunic);
    }
    
	// Chamar as funções de acesso de dados
   	dadosTabelaDispersa(hashTable, 72202);
    dadosBuscaBinaria(cidadeArray, count, 6873);
    

    // Libera a memória alocada para as cidades e o array de cidades
    for (i = 0; i < count; i++) {
        free(cidadeArray[i]);
    }
    free(cidadeArray);
    // Libera a memória alocada para a tabela hash
    free(hashTable);
}

/*
void imprimeCidades(CidadeHashTable *hashTable) { // Para imprimir as cidades com todos os dados
    int i;
    for (i = 0; i < TAMANHO_TABELA_HASH; i++) {
        CidadeNo *atual = hashTable->tabela[i];
        while (atual != NULL) {
            Cidade *cidade = atual->cidade;
            printf("UF: %s\n", cidade->uf);
            printf("Codigo UF: %s\n", cidade->codUf);
            printf("Codigo Municipio: %d\n", cidade->codMunic);
            printf("Nome Municipio: %s\n", cidade->nomeMunic);
            printf("Populacao: %s\n", cidade->populacao);
            printf("\n");
            atual = atual->prox;
        }
    }
}
*/
