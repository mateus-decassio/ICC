#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matriz.h"

/**
 * Função que gera os coeficientes de um sistema linear k-diagonal
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
 * @param k numero de diagonais da matriz A
 */
static inline double generateRandomA( unsigned int i, unsigned int j, unsigned int k )
{
  static double invRandMax = 1.0 / (double)RAND_MAX;
  return ( (i==j) ? (double)(k<<1) : 1.0 )  * (double)rand() * invRandMax;
}

/**
 * Função que gera os termos independentes de um sistema linear k-diagonal
 * @param k numero de diagonais da matriz A
 *
 */
static inline double generateRandomB( unsigned int k )
{
  static double invRandMax = 1.0 / (double)RAND_MAX;
  return (double)(k<<2) * (double)rand() * invRandMax;
}



/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatPtr: gera matriz como vetor de ponteiros para as suas linhas
 * 
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *  @return  ponteiro para a matriz gerada
 *
 */

MatPtr geraMatPtr (int m, int n)
{
  MatPtr matriz = (double **) malloc(m*sizeof(double));

  if (matriz) {
    for (int i=0; i < m; ++i) {
      if (matriz[i] = (double *) malloc(n*sizeof(double)))
        for (int j=0; matriz[i] && j < n; ++j)
          matriz[i][j] = generateRandomA(i, j, DIAG);
      else
          return NULL;
    }
  }
  
  return (matriz);
}

/**
 *  \brief: libera matriz alocada como vetor de ponteiros para as suas linhas
 * 
 *  @param  ponteiro para matriz
 *
 */
void liberaMatPtr (MatPtr matriz, int m)
{
  if (matriz) {
    for (int i=0; i < m; ++i) {
      free (matriz[i]);
    }
    free (matriz);
  }
}



/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow (int m, int n)
{
  MatRow matriz = (double *) malloc(m*n*sizeof(double));

  if (matriz) {
    for (int i=0; i < m; ++i) {
      for (int j=0; j < n; ++j)
        matriz[i*m + j] = generateRandomA(i, j, DIAG);
    }
  }
  
  return (matriz);
}


/**
 *  Funcao geraMatCol: gera matriz como vetor único, 'column-oriented'
 * 
 *  @param m  número de linhas da matriz
 *  @param n  número de colunas da matriz
 *  @return  ponteiro para a matriz gerada
 *
 */

MatCol geraMatCol (int m, int n)
{
  MatCol matriz = (double *) malloc(m*n*sizeof(double));

  if (matriz) {
    for (int i= 0; i < m; ++i) {
      for (int j=0; j < n; ++j)
        matriz[j*n + i] = generateRandomA(i, j, DIAG);
    }
  }
  
  return (matriz);
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor (int n)
{
  Vetor vetor = (double *) malloc(n*sizeof(double));

  if (vetor)
    for (int i=0; i < n; ++i)
      vetor[i] = generateRandomB(n);
  
  return (vetor);
}

/**
 *  \brief: libera vetor
 * 
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor (void *vet)
{
	free(vet);
}


/**
 *  Funcao multMatPtrVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatPtrVet (MatPtr mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
        res[i] += mat[i][j] * v[j];
  }
}

/**
 *  Funcao prnMatPtr:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMatPtr (MatPtr mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[i][j]);
    printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao multMatRowVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */
void multMatRowVetOLD (MatRow mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
        res[i] += mat[m*i + j] * v[j];
  }
}


void multMatRowVetOPT (MatRow restrict mat, Vetor restrict v, int m, int n, Vetor restrict res)
{ 
  /* Efetua a multiplicação */
  if (res) 
  {
    int b = 8; //tamanho do bloco (8x8) - FIXO
    int ii, istart, iend, jj, jstart, jend, j, ind, i, tamanhoC, tamanhoL;
    double R, R0, R1, R2, R3, R4, R5, R6, R7, RPP1, RPP2, RPP3, RPP4, RP1, RP2; 

    for (ii = 0; ii < n / b; ++ii)
    {
      istart = ii * b;
      iend = istart + b;

      for (jj = 0; jj < n / b; ++jj)
      {
        jstart = jj * b;
        jend = jstart + b;
        j = jstart;
        ind = 0;

        for (i = istart; i < iend; ++i)
        {
          R0 = mat[(m * i) + j] * v [j];
          R1 = mat[(m * i) + j+1] * v [j+1];
          R2 = mat[(m * i) + j+2] * v [j+2];
          R3 = mat[(m * i) + j+3] * v [j+3];
          R4 = mat[(m * i) + j+4] * v [j+4];
          R5 = mat[(m * i) + j+5] * v [j+5];
          R6 = mat[(m * i) + j+6] * v [j+6];
          R7 = mat[(m * i) + j+7] * v [j+7];

          RPP1 = R0 + R4;
          RPP2 = R1 + R5;
          RPP3 = R2 + R6;
          RPP4 = R3 + R7;

          RP1 = RPP1 + RPP2;
          RP2 = RPP3 + RPP4;

          res [j + ind] = RP1 + RP2;
          ind++;
        }
      }
    }

    //REMAINDER
    tamanhoC = (n / b) * b;
    tamanhoL = n - tamanhoC;

    //remainder que trata as colunas restantes
    for (i = 0; i < tamanhoC; ++i)
    {
      for (j = 0; j < tamanhoL; ++j)
      {
        R += mat[(m * i) + (tamanhoC + j)] * v[tamanhoC + j];
      }
      res[i] += R;
    }

    R = 0.0;

    //remainder que trata as linhas restantes
    for (i = tamanhoC; i < n; ++i)
    {
      for (j = 0; j < n; ++j)
      {
        R += mat[(m * i) + j] * v[j];
      }
      res[i] += R;
    }
  }
}; //TERMINADO



/**
 *  Funcao prnMatRow:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMatRow (MatRow mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[m*i + j]);
    printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao multMatColVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */
void multMatColVetOLD (MatCol mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
        res[i] += mat[n*j + i] * v[j];
  }
}

void multMatColVetOPT (MatCol restrict mat, Vetor restrict v, int m, int n, Vetor restrict res)
{
    
  /* Efetua a multiplicação */
  if (res)
  {
    int b = 8; //tamanho do bloco (8x8) - FIXO
    int jj, jstart, jend, ii, istart, iend, i, j, tamanhoC, tamanhoL;

    for (jj = 0; jj < n / b; ++jj)
    {
      jstart = jj * b;
      jend = jstart + b;

      for (ii = 0; ii < n / b; ++ii)
      {
        istart = ii * b;
        iend = istart + b;
        i = istart;

        for (j = jstart; j < jend; ++j)
        {
          res[i] = mat[(m * (i)) + j] * v[j];
          res[i+1] = mat[(m * (i+1)) + j] * v[j];
          res[i+2] = mat[(m * (i+2)) + j] * v[j];
          res[i+3] = mat[(m * (i+3)) + j] * v[j];
          res[i+4] = mat[(m * (i+4)) + j] * v[j];
          res[i+5] = mat[(m * (i+5)) + j] * v[j];
          res[i+6] = mat[(m * (i+6)) + j] * v[j];
          res[i+7] = mat[(m * (i+7)) + j] * v[j];
        }
      }
    }

    //REMAINDER
    tamanhoC = (n / b) * b;
    tamanhoL = n - tamanhoC;

    //remainder que trata as colunas restantes
    for (i = 0; i < tamanhoL; ++i)
    {
      for (j = 0; j < tamanhoC; ++j)
      {
        res[j] += mat[(m * j) + (tamanhoC + i)] * v[tamanhoC + i];
      }
    }

    //remainder que trata as linhas restantes
    for (j = 0; j < n; ++j)
    {
      for (i = tamanhoC; i < n; ++i)
      {
        res[i] += mat[(m * i) + j] * v[j];
      }
    }
  }
}; //TERMINADO



/**
 *  Funcao prnMatCol:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 *
 */

void prnMatCol (MatCol mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[n*j + i]);
    printf("\n");
  }

  printf(SEP_RES);

}

/**
 *  Funcao prodEscalar:  Calcula o produto escalar entre 2 vetores
 *  @param v1 vetor com 'n' elementos
 *  @param v2 vetor com 'n' elementos
 *  @param n número de elementos dos vetores
 *  @return Valor do produto escalar
 */

double prodEscalar (Vetor v1, Vetor v2, int n)
{
  double prod = 0.0;

  for (int i=0; i < n; ++i)
    prod += v1[i]*v2[i];

  return prod;
}


/**
 *  Funcao normaMax:  Calcula a norma máxima da diferença de 2 vetores
 *  @param v1 vetor com 'n' elementos
 *  @param v2 vetor com 'n' elementos
 *  @param n número de elementos dos vetores
 *  @return Valor da norma máxima
 */

double normaMax (Vetor v1, Vetor v2, int n)
{
  double max = ABS(v1[0] - v2[0]);

  for (int i=0; i < n; ++i)
    if (ABS(v1[i]-v2[i]) > max)
      max = ABS(v1[i]-v2[i]);

  return max;
}


/**
 *  Funcao normaEucl:  Calcula a norma euclidiana de um vetor
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos dos vetores
 *  @return Valor da norma euclidiana
 */

double normaEucl (Vetor vet, int n)
{
  double prod = 0.0;

  for (int i=0; i < n; ++i)
    prod += vet[i]*vet[i];

  return sqrt(prod);
}


/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor (Vetor vet, int n)
{
  for (int i=0; i < n; ++i)
    printf(DBL_FIELD, vet[i]);
  printf(SEP_RES);
}

