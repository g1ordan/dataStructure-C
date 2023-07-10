#include "cidade.h"

#define TAMANHO_TABELA_HASH 10000
#define TAMANHO_MAX_STRING 256


// Fun��o para criar uma nova tabela hash
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

//O uso do tipo "unsigned int" � comum para garantir que o valor retornado seja sempre positivo.
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
        // Atribui a cidade ao novo n�
        novoNo->cidade = cidade;
        novoNo->prox = NULL;

        // Verifica se a posi��o da tabela hash est� vazia
        if (hashTable->tabela[indice] == NULL) {
            // Se estiver vazia, insere o novo n� diretamente na posi��o
            hashTable->tabela[indice] = novoNo;
        } else {
            // Caso contr�rio, ocorreu uma colis�o
            printf("Colis�o encontrada no �ndice %u:\n", indice);

            // Insere o novo n� como pr�ximo do �ltimo n� da lista
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
            printf("\nDados encontrados atrav�s da tabela dispersa:\n");
            printf("Cod. Munic�pio: %d, Munic�pio: %s, Estado: %s\n\n", atual->cidade->codMunic, atual->cidade->nomeMunic, atual->cidade->uf);
            return; // Encontrou o resultado, encerra a fun��o
        }
        atual = atual->prox;
    }

    printf("Falha ao acessar os dados.\n");
}

//Fun��o de compara��o para o qsort, usado para ordenar as cidades
/*Os par�metros const void* a e const void* b s�o ponteiros gen�ricos (void*) para os elementos a serem comparados.
 Eles s�o declarados como "const" para indicar que o conte�do apontado por eles n�o ser� modificado pela fun��o de compara��o*/
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
            printf("Dados encontrados atrav�s da busca bin�ria:\n");
            printf("Cod. Munic�pio: %d, Munic�pio: %s, Estado: %s\n\n", cidadeArray[meio]->codMunic, cidadeArray[meio]->nomeMunic, cidadeArray[meio]->uf);
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
    // Cria um array para armazenar as cidades e seus �ndices
    Cidade** cidadeArray = NULL;
    int count = 0;

    char linha[TAMANHO_MAX_STRING];
    fgets(linha, TAMANHO_MAX_STRING, file); // N�o acessa a primeira linha

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
                // Em caso de falha na realoca��o do array, libera a mem�ria da nova cidade e interrompe o loop
                free(novaCidade);
                break;
            }
        }
    }
    fclose(file);

    printf("\nLeitura e inser��o de cidades conclu�das.\n\n");

    // Ordena o array de cidades usando a fun��o qsort e a fun��o de compara��o
    qsort(cidadeArray, count, sizeof(Cidade*), compare);

    int i;
    printf("\nCidades ordenadas pelo c�digo do Munic�pio:\n\n");
    for (i = 0; i < count; i++) {
        printf("Codigo: %d, Estado: %s, Municipio: %s\n", cidadeArray[i]->codMunic, cidadeArray[i]->uf, cidadeArray[i]->nomeMunic);
    }
    
	// Chamar as fun��es de acesso de dados
   	dadosTabelaDispersa(hashTable, 72202);
    dadosBuscaBinaria(cidadeArray, count, 6873);
    

    // Libera a mem�ria alocada para as cidades e o array de cidades
    for (i = 0; i < count; i++) {
        free(cidadeArray[i]);
    }
    free(cidadeArray);
    // Libera a mem�ria alocada para a tabela hash
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
