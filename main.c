#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "cidade.h"


/*Sor, nas propriedade do console/terminal, na aba layout tu altera a LARGURA do tamanho do buffer de tela, seta 9999 pra facilitar a visualiza��o, j� que s�o muitos dados printados*/
int main(int argc, char *argv[]) {
				
	setlocale(LC_ALL, "Portuguese");
	CidadeHashTable* hashTable = criaHashTable();

    int count; //N�mero de cidades lidas

    Cidade** cidadeArray = (Cidade**)malloc(TAMANHO_MAX_CIDADES * sizeof(Cidade*));
    if (cidadeArray == NULL) {
        printf("Falha ao alocar mem�ria para o array de cidades.\n");
        //Libera a mem�ria alocada para a tabela hash
        free(hashTable);
        return 1;
    }
	
    lerCidades(hashTable, cidadeArray, &count);
    
    int cod, cod2, opcao;
    do {
        chamaMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\nDigite o codMunic para realizar a busca: ");
                scanf("%d", &cod);
                printf("Digite o codUf para realizar a busca: ");
				scanf("%d", &cod2);
                dadosTabelaDispersa(hashTable, cod, cod2);
                break;
			case 2:
                printf("\nDigite o codMunic para realizar a busca: ");
                scanf("%d", &cod);
                buscaBinaria(cod, cidadeArray, count);
                break;
			case 3:
                buscaBinariaPorTodosDados(cidadeArray, count);
                break;
            case 4:
                todosDadosTabelaDispersa(hashTable);
                break;
			case 0:
                printf("\nPrograma Finalizado.\n");
                break;
			default:
                printf("Op��o inv�lida.\n");
                break;
        }
    }while(opcao != 0);

    //Libera a mem�ria alocada para as cidades e o array de cidades
    int i;
    for (i = 0; i < count; i++) {
        free(cidadeArray[i]);
    }
    free(cidadeArray);

    
    free(hashTable);

    return 0;
}
