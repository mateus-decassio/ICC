#include <stdio.h>
#include <stdlib.h>    
#include <string.h>
#include <getopt.h>

#include <likwid.h>

#include "SistemasLineares.h"

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ -n <ordem> ] \n", progname);
  exit(1);
}


int main (int argc, char *argv[])
{
	int c, verificador;
	double normaL2, tempoL;
	unsigned int n=DEF_SIZE, iter;

/* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  char *opts = "n:";
  c = getopt (argc, argv, opts);
  
  while ( c != -1 ) {
    switch (c) {
    case 'n':  n = atoi(optarg);              break;
    default:   usage(argv[0]);
    }
      
    c = getopt (argc, argv, opts);
  }
  
/* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */
	
	SistLinear_t *SL = alocaSistLinear (n);
	
	if (!SL)
	{
		fprintf (stderr, "ERRO AO ALOCAR O SISTEMA LINEAR!\n");
		return -1;
	}

	SL->n = n; //tamanho do sistema linear

	inicializaSistLinear (SL);


  printf("%d  ", n);
	
	tempoL = timestamp();
  verificador = gaussJacobiOPT (SL, EPS, &normaL2, &iter);
  tempoL  = timestamp() - tempoL;
  printf("%.8g  ",tempoL);

  tempoL = timestamp();
  verificador = gaussSeidelOPT (SL, EPS, &normaL2, &iter);
  tempoL  = timestamp() - tempoL;
  printf("%.8g  \n",tempoL);

  //medição da média do tempo total da normaL2Residuo
  //printf("%.8g  \n",tempoNormaL2/conta_entrada);

} //END FINAL