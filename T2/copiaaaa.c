#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "SistemasLineares.h"

int main ()
{
	SistLinear_t *SL = alocaSistLinear (3);

	if (!SL)
	{
		printf("DEU ERRO AQUI CARA!\n");
		return 0;
	}

	SL->A[0] = 1;
	SL->A[1] = 0;
	SL->A[2] = 1;
	SL->A[3] = 3;
	SL->A[4] = 3;
	SL->A[5] = 4;
	SL->A[6] = 2;
	SL->A[7] = 2;
	SL->A[8] = 3;


	SL->b[0] = 1;
	SL->b[1] = 2;
	SL->b[2] = -2;

	SL->n = 3;

	//imprime_SL (SL, 3);

	double normaL2;
	int verifica = eliminacaoGauss (SL, &normaL2);



}

