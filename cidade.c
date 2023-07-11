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

/*Função de comparação para o qsort e bsearch.
Os parâmetros const void* a e const void* b são ponteiros genéricos (void*) para os elementos a serem comparados.
Eles são declarados como "const" para indicar que o conteúdo apontado por eles não será modificado pela função de comparação*/
int compare(const void* a, const void* b) {
    const Cidade* cidadeA = *(const Cidade**)a;
    const Cidade* cidadeB = *(const Cidade**)b;
    return cidadeA->codMunic - cidadeB->codMunic;
}

void buscaBinaria(int buscaCodigo, Cidade** cidadeArray, int count) {
    Cidade* cidadeProcurada = malloc(sizeof(Cidade));
    if (cidadeProcurada == NULL) {
        printf("Erro na alocação de memória.\n");
        return;
    }
    cidadeProcurada->codMunic = buscaCodigo;

    Cidade** resultado = bsearch(&cidadeProcurada, cidadeArray, count, sizeof(Cidade*), compare);

    if (resultado != NULL) {
        printf("\nDados encontrados através da busca binária:\n");
        printf("Cod. Município: %d, Município: %s, Estado: %s\n\n", (*resultado)->codMunic, (*resultado)->nomeMunic, (*resultado)->uf);
    } else {
        printf("Dados não encontrados.\n");
    }

    free(cidadeProcurada);
}

void buscaBinariaPorTodosDados(Cidade** cidadeArray, int count) {
    int i;
    for (i = 0; i < count; i++) {
        int numAcessos = 0; // Inicializa o contador para cada busca

        int codMunicBusca = cidadeArray[i]->codMunic;
        int esquerda = 0;
        int direita = count - 1;
        Cidade* resultado = NULL;

        while (esquerda <= direita) {
            int meio = (esquerda + direita) / 2;
            numAcessos++; // Incrementa o contador a cada acesso ao elemento

            if (cidadeArray[meio]->codMunic == codMunicBusca) {
                resultado = cidadeArray[meio];
                break; // Elemento encontrado, interrompe o loop
            } else if (cidadeArray[meio]->codMunic < codMunicBusca) {
                esquerda = meio + 1; // Atualiza a posição da esquerda
            } else {
                direita = meio - 1; // Atualiza a posição da direita
            }
        }

        if (resultado != NULL) {
            printf("Dados encontrados para Código do Município %d:\n", codMunicBusca);
            printf("Nome do Município: %s\n", resultado->nomeMunic);
            printf("Número de acessos necessários: %d\n\n", numAcessos);
        } else {
            printf("Não foram encontrados dados para o codMunic %d.\n\n", codMunicBusca);
        }
    }
}

void lerCidades(CidadeHashTable* hashTable, Cidade** cidadeArray, int* countPtr) {
    FILE* file = fopen("cidadesUTF-8.csv", "r");
    if (file == NULL) {
        printf("Falha ao abrir o arquivo!\n");
        return;
    }

    int count = 0;

    char linha[TAMANHO_MAX_STRING];
    fgets(linha, TAMANHO_MAX_STRING, file); // Não acessa a primeira linha

    while (fgets(linha, TAMANHO_MAX_STRING, file) != NULL) {
        char uf[3], codUf[3], nomeMunic[33], populacao[9];
        int codMunic;
        sscanf(linha, "%2s,%2s,%d,%32[^,],%8s", uf, codUf, &codMunic, nomeMunic, populacao); 

        Cidade* novaCidade = criaCidade(uf, codUf, codMunic, nomeMunic, populacao);
        if (novaCidade != NULL) {
            insereCidade(hashTable, novaCidade);
            cidadeArray[count] = novaCidade;
            count++;
        }
    }
    fclose(file);

    printf("\nLeitura e inserção de cidades concluídas.\n\n");

    qsort(cidadeArray, count, sizeof(Cidade*), compare);

    int i;
    printf("\nCidades ordenadas pelo código do Município:\n\n");
    for (i = 0; i < count; i++) {
        printf("Codigo: %d, Estado: %s, Municipio: %s\n", cidadeArray[i]->codMunic, cidadeArray[i]->uf, cidadeArray[i]->nomeMunic);
    }

    *countPtr = count;
}


void chamaMenu() {
    printf("\nSelecione uma opção:\n");
    printf("1 - Buscar dados na tabela dispersa.\n");
    printf("2 - Buscar dados através da busca binária.\n");
    printf("3 - Buscar dados através da busca binária percorrendo todas cidades.\n");
    printf("0 - Sair\n");
    printf("Opção: ");
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
