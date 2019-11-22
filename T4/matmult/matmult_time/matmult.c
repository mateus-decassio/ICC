#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */

#include <likwid.h>

#include "matriz.h"

#include <sys/time.h>
/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ -n <ordem> ] \n", progname);
  exit(1);
}

//----------------------------FUNÇÃO PARA MEDIÇÃO DO TEMPO----------------------------//
// Medição de tempo
double timestamp (void);

// Retorna o tempo em MILISSEGUNDOS
double timestamp (void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

//------------------------------------------------------------------------------------//
/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main (int argc, char *argv[]) 
{
  int c, n=DEF_SIZE;
  double norma, tempo;
  
  MatPtr mPtr;
  MatRow mRow;
  MatCol mCol;
  //Vetor vet, resPtr, resRow, resCol;
  Vetor vet, resRow, resCol;
  
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
 
  //resPtr = (double *) malloc (n * sizeof(double));
  resRow = (double *) malloc (n * sizeof(double));
  resCol = (double *) malloc (n * sizeof(double)); 
  
  srand(20191);
      
  //mPtr = geraMatPtr (n, n);
  mRow = geraMatRow (n, n);
  mCol = geraMatCol (n, n);
  vet = geraVetor (n);
    
#ifdef DEBUG
    //prnMatPtr (mPtr, n, n);
    prnMatRow (mRow, n, n);
    prnMatCol (mCol, n, n);
    prnVetor (vet, n);
    printf ("=================================\n\n");
#endif /* DEBUG */

  
  printf("%d  ", n);
  
  tempo = timestamp();
  multMatRowVetOPT (mRow, vet, n, n, resRow);
  tempo  = timestamp() - tempo;
  printf("%.8g  ",tempo);
  

  tempo = timestamp();
  multMatColVetOPT (mCol, vet, n, n, resCol);
  tempo  = timestamp() - tempo;
  printf("%.8g  \n",tempo);
  

  //norma = normaMax(resRow, resPtr, n);

  //norma = normaEucl(resCol, n);

#ifdef DEBUG
    //prnVetor (resPtr, n);
    prnVetor (resRow, n);
    prnVetor (resCol, n);
#endif /* DEBUG */

  //liberaMatPtr (mPtr, n);
  liberaVetor ((void*)mRow);
  liberaVetor ((void*)mCol);
  liberaVetor ((void*)vet);
    
  free(resCol);
  free(resRow);
  //free(resPtr);

  return 0;
}

