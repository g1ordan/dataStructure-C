#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "cidade.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	setlocale(LC_ALL, "Portuguese");
	LerCidades();
	
	return 0;
}

