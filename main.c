#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "cidade.h"

int main(int argc, char *argv[]) {
	
	setlocale(LC_ALL, "Portuguese");
	lerCidades();
	
	return 0;
}

