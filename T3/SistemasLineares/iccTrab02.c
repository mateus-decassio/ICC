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
	double normaL2;
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

	LIKWID_MARKER_INIT;

  	LIKWID_MARKER_START ("eliminacaoGauss");
  	verificador = eliminacaoGauss (SL, &normaL2);
  	LIKWID_MARKER_STOP ("eliminacaoGauss");
	
	
	LIKWID_MARKER_START("gaussJacobi");
  	verificador = gaussJacobi (SL, EPS, &normaL2, &iter);
  	LIKWID_MARKER_STOP ("gaussJacobi");


  	LIKWID_MARKER_START("gaussSeidel");
  	verificador = gaussSeidel (SL, EPS, &normaL2, &iter);
  	LIKWID_MARKER_STOP ("gaussSeidel");
	
	LIKWID_MARKER_CLOSE;

} //END FINAL