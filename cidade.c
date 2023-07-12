#include "cidade.h"


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

int calculaHash(int codMunic) {
    int numPrimo = 5573;
    return codMunic % numPrimo;
}

void insereCidade(CidadeHashTable* hashTable, Cidade* cidade) {
    int indice = calculaHash(cidade->codMunic);

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
            printf("Colis�o encontrada no �ndice %d:\n", indice);
            
            // Insere o novo n� como pr�ximo do �ltimo n� da lista
            CidadeNo* ultimo = hashTable->tabela[indice];
            while (ultimo->prox != NULL) {
                ultimo = ultimo->prox;
            }
            ultimo->prox = novoNo;
        }   
    }  
}

Cidade* criaCidade(const char* uf, int codUf, int codMunic, const char* nomeMunic, const char* populacao) {
    
    Cidade* novaCidade = (Cidade*)malloc(sizeof(Cidade));
    if (novaCidade != NULL) {
        strcpy(novaCidade->uf, uf);
        novaCidade->codUf = codUf;
        novaCidade->codMunic = codMunic;
        strcpy(novaCidade->nomeMunic, nomeMunic);
        strcpy(novaCidade->populacao, populacao);
    }
    return novaCidade;
}

void dadosTabelaDispersa(CidadeHashTable* hashTable, int codMunic, int codUf) {
    int indice = calculaHash(codMunic);

    CidadeNo* atual = hashTable->tabela[indice];
    int encontrados = 0;
    while (atual != NULL) {
        if (atual->cidade->codMunic == codMunic && atual->cidade->codUf == codUf) {
            printf("\nCod. Munic�pio: %d, Munic�pio: %s, Estado: %s\n\n", atual->cidade->codMunic, atual->cidade->nomeMunic, atual->cidade->uf);
            encontrados++;
            return; //Se encontrar o resultado a fun��o j� encerra
        }
        atual = atual->prox;
    }

    printf("\nFalha ao acessar os dados.\n");
}

void todosDadosTabelaDispersa(CidadeHashTable* hashTable) {
    int totalAcessos = 0;
	int i;
    for (i = 0; i < TAMANHO_TABELA_HASH; i++){
        CidadeNo* atual = hashTable->tabela[i];
        while(atual != NULL){
            printf("C�digo: %d, Munic�pio: %s\n", atual->cidade->codMunic, atual->cidade->nomeMunic);
            totalAcessos++;
            atual = atual->prox;
        }
    }
	printf("\nTotal de acessos: %d\n\n", totalAcessos);
}

/*Fun��o de compara��o para o qsort e bsearch.
Os par�metros const void* a e const void* b s�o ponteiros gen�ricos (void*) para os elementos a serem comparados.
Eles s�o declarados como "const" para indicar que o conte�do apontado por eles n�o ser� modificado pela fun��o de compara��o*/
int compare(const void* a, const void* b) {
    const Cidade* cidadeA = *(const Cidade**)a;
    const Cidade* cidadeB = *(const Cidade**)b;
    return cidadeA->codMunic - cidadeB->codMunic;
}

void buscaBinaria(int buscaCodigo, Cidade** cidadeArray, int count) {
    Cidade* cidadeProcurada = malloc(sizeof(Cidade));
    if (cidadeProcurada == NULL) {
        printf("Erro na aloca��o de mem�ria.\n");
        return;
    }
    cidadeProcurada->codMunic = buscaCodigo;

    Cidade** resultado = bsearch(&cidadeProcurada, cidadeArray, count, sizeof(Cidade*), compare);

    if (resultado != NULL) {
        printf("\nCod. Munic�pio: %d, Munic�pio: %s, Estado: %s\n\n", (*resultado)->codMunic, (*resultado)->nomeMunic, (*resultado)->uf);
    } else {
        printf("\nDados n�o encontrados.\n");
    }

    free(cidadeProcurada);
}

void buscaBinariaPorTodosDados(Cidade** cidadeArray, int count) {
    int i;
    for (i = 0; i < count; i++) {
        int numAcessos = 0;

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
                esquerda = meio + 1; // Atualiza a posi��o da esquerda
            } else {
                direita = meio - 1; // Atualiza a posi��o da direita
            }
        }

        if (resultado != NULL) {
            printf("Cod. do Munic�pio: %d - ", codMunicBusca);
            printf("Nome do Munic�pio: %s ----> ", resultado->nomeMunic);
            printf("N�mero de acessos necess�rios: %d\n", numAcessos);
        } else {
            printf("N�o foram encontrados dados para o codMunic %d.\n\n", codMunicBusca);
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
    fgets(linha, TAMANHO_MAX_STRING, file); // N�o acessa a primeira linha

    while (fgets(linha, TAMANHO_MAX_STRING, file) != NULL) {
        char uf[3], nomeMunic[33], populacao[9];
        int codMunic, codUf;
        sscanf(linha, "%2s,%d,%d,%32[^,],%8s", uf, &codUf, &codMunic, nomeMunic, populacao); 

        Cidade* novaCidade = criaCidade(uf, codUf, codMunic, nomeMunic, populacao);
        if (novaCidade != NULL) {
            insereCidade(hashTable, novaCidade);
            cidadeArray[count] = novaCidade;
            count++;
        }
    }
    fclose(file);
    
    printf("\nLeitura e inser��o de cidades conclu�das.\n\n");
    printf("Inser��o dos novos registros:\n\n");
	novosRegistros(hashTable, cidadeArray, &count);
    qsort(cidadeArray, count, sizeof(Cidade*), compare);

    int i;
    printf("\nCidades ordenadas pelo c�digo do Munic�pio:\n\n");
    for (i = 0; i < count; i++) {
        printf("Codigo: %d, codUf: %d, Municipio: %s, Estado: %s\n", cidadeArray[i]->codMunic, cidadeArray[i]->codUf, cidadeArray[i]->nomeMunic, cidadeArray[i]->uf);
    }

    *countPtr = count;
}

void novosRegistros(CidadeHashTable* hashTable, Cidade** cidadeArray, int* countPtr) {
  
    Cidade* novaCidade = criaCidade("RS", 01, 6001, "Albatroz", "5.000");
    insereCidade(hashTable, novaCidade);
    cidadeArray[*countPtr] = novaCidade;
    (*countPtr)++;//incrementa +1 registrto no cidadeArray

    novaCidade = criaCidade("SC", 02, 6001, "Borussia", "4.000");
    insereCidade(hashTable, novaCidade);
    cidadeArray[*countPtr] = novaCidade;
    (*countPtr)++;

    novaCidade = criaCidade("PR", 03, 6001, "Palmital", "3.000");
    insereCidade(hashTable, novaCidade);
    cidadeArray[*countPtr] = novaCidade;
    (*countPtr)++;

    novaCidade = criaCidade("MT", 04, 6001, "Laranjeiras", "2.000");
    insereCidade(hashTable, novaCidade);
    cidadeArray[*countPtr] = novaCidade;
    (*countPtr)++;
}

void chamaMenu() {
    printf("\nSelecione uma op��o:\n\n");
    printf("1 - Buscar dados na tabela dispersa.\n");
    printf("2 - Buscar dados atrav�s da busca bin�ria.\n");
    printf("3 - Buscar dados atrav�s da busca bin�ria percorrendo todas cidades.\n");
    printf("4 - Buscar todos os dados na tabela dispersa.\n");
    printf("0 - Sair\n\n");
    printf("Op��o: ");
}
