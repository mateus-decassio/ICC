/**
  * @file SistemasLineares.c
  * @version 1.0
  * @author Mateus Felipe de Cássio Ferreira (GRR 20176123)
  * @section DESCRIPTION
  * Essa biblioteca tem como objetivo implementar três métodos para a resolução de Sistemas de 
  * equações Lineares, visto que são fundamentais na resolução de problemas científicos. Assim, essa
  * biblioteca implementa a Eliminação de Gauss, o Método de Gauss-Jacobi e o Método de Gauss-Seidel.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <likwid.h>
#include "SistemasLineares.h"

//-----------------------------------NÃO ALTERAR-----------------------------------//

// Alocação de memória
SistLinear_t* alocaSistLinear (unsigned int n)
{
	SistLinear_t *SL = (SistLinear_t *) malloc(sizeof(SistLinear_t));
	if ( SL ) 
	{
		SL->A = (real_t *) malloc(n * n * sizeof(real_t));
		SL->b = (real_t *) malloc(n * sizeof(real_t));
		SL->x = (real_t *) malloc(n * sizeof(real_t));
		if (!(SL->A) || !(SL->b) || !(SL->x))
			liberaSistLinear(SL);
	}
	return (SL);
}


//Liberação de memória
void liberaSistLinear (SistLinear_t *SL)
{
	free(SL->A);
	free(SL->b);
	free(SL->x);
	free(SL);
}


// Inicializa SL
void inicializaSistLinear (SistLinear_t *SL)
{
  static double invRandMax = 1.0 / (double)RAND_MAX;
  // inicializa a matriz A
  for (unsigned int i=0; i<SL->n; ++i)
     for (unsigned int j=0; j<SL->n; ++j)
     {
        SL->A[i*SL->n+j] = ( (i==j) ? (double)(DIAG<<1) : 1.0 )  * (double)rand() * invRandMax;
     }

  // inicializa vetores b e x
  for (unsigned int i=0; i<SL->n; ++i)
  {
     SL->b[i] = (double)rand() * invRandMax;
     SL->x[i] = 0.0;
  }
}


// Retorna o tempo em MILISSEGUNDOS
double timestamp (void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}
//---------------------------------------------------------------------------------//


//--------------------------------------DECLARAÇÕES DE FUNÇÕES AUXILIARES--------------------------------------//

void copiaSistLinear (SistLinear_t *SL, SistLinear_t *SLc, unsigned int n);
void copiaVetor (real_t *origem, real_t *destino, unsigned int n);
unsigned int encontraMax (real_t *A, unsigned int *LT, unsigned int COL, unsigned int n);
real_t normaMAX (real_t *next, real_t *prev, unsigned int n);
void imprime_SL (SistLinear_t *SL, unsigned int n);

//-------------------------------------------------------------------------------------------------------------//
/**
  * @brief Função para o cálculo da Norma L² do resíduo.
  * @param SL Sistema Linear do qual se deseja calcular a norma.
  * @return Valor da norma. 
  */
double normaL2Residuo (SistLinear_t *SL)
{
	real_t *residuo;
	real_t soma = 0.0;
	unsigned int i, k;
	residuo = (real_t *) malloc (SL->n * sizeof (real_t));

	if (!residuo)
	{
		fprintf(stderr, "Erro! Não foi possível alocar o vetor de resíduos usado na função, provavelmente por falta de memória. Desconsidere o valor de retorno.\n");
		return 0.0;
	}

	for (i = 0; i < SL->n; ++i)
	{
		unsigned int j = (i * SL->n);
		k = 0;
		for (unsigned int cont = 0; cont < SL->n; ++cont)
		{
			soma += (SL->A[j] * SL->x[k]);
			++k;
			++j;
		}
		residuo[i] = SL->b[i] - soma;
		soma = 0.0;
	}

	for (i = 0; i < SL->n; ++i)
		soma += residuo[i];

	
	free (residuo);
	return (soma);
}; //TERMINADO



/**
  * @brief Função que implementa a Eliminação de Gauss, que é considerado um Método Direto
  * na resolução de sistemas lineares, visto que, salvo erros introduzidos pela máquina, conduzem
  * à solução exata em um número finito de passos.
  * @param SL Sistema Linear do qual se deseja calcular a solução.
  * @param normaL2 Ponteiro que irá direcionar o valor da norma encontrada antes do método terminar de executar.
  * @return -1 Caso haja algum problema na alocação de um Sistema Linear auxiliar, a fim de evitar alterações no sistema original.
  * @return -2 Caso haja algum problema na alocação de um vetor (Lookup Table) usado nessa função.
  * @return 0 Caso obtenha sucesos em calcular a solução.
  * @see copiaSistLinear()
  * @see encontraMax()
  * @see normaL2Residuo()
  */
int eliminacaoGauss (SistLinear_t *SL, double *normaL2)
{
	unsigned int n = SL->n;
	SistLinear_t *SLc;
	SLc = alocaSistLinear (n);
	
	if (!SLc)
	{
		fprintf (stderr, "Erro! Não foi possível alocar espaço para fazer a cópia da matriz.\n");
		return -1;
	}

	copiaSistLinear (SL, SLc, n);

	//alocagem da LookupTable e preenchimento dos valores da ordem das linhas
	unsigned int *LT, *LTc;
	unsigned int i = 0;
	unsigned int k = 0;
	unsigned int j = 0;
	LT = (unsigned int *) malloc (n * sizeof (unsigned int));
	
	if (!LT)
	{
		fprintf (stderr, "Erro! Não foi possível alocar espaço a LookupTable\n");
		return -2;
	}



	for (i = 0; i < n; ++i)
		LT[i] = i;
	

	for (i = 0; i < (n - 1); ++i)
	{
		unsigned int iPivo = encontraMax (SLc->A, LT, i, n);
		
		//TROCA AS LINHAS DA LookupTable se houver necessidade
		if (LT[i] != iPivo)
		{
			unsigned int aux = LT[i];
			LT[i] = LT[iPivo];
			LT[iPivo] = aux;
		}

		for (k = i+1; k < n; ++k)
		{
			real_t m = SLc->A[(LT[k] * n) + i] / SLc->A[(LT[i] * n) +i];
			SLc->A[(LT[k] * n) + i] = 0.0;

			for (j = i+1; j < n; ++j)
				SLc->A[(LT[k] * n) + j] -= SLc->A[(LT[i] * n) + j] * m;
			SLc->b[LT[k]] -= SLc->b[LT[i]] * m;
		}
	}
	
	//CONVERTE A LookupTable
	LTc = (unsigned int *) malloc (n * sizeof (unsigned int));
	
	if (!LTc)
	{
		fprintf (stderr, "Erro! Não foi possível alocar espaço a LookupTable\n");
		return -2;
	}

	for (i = 0; i < n; ++i)
	{
		j = i;
		LTc[LT[j]] = i;
	}


	//RETROSSUBSTITUIÇÃO
	real_t soma = 0;
	n--; 
	SL->x[n] = (SLc->b[LTc[n]] / SLc->A[(LTc[n] * (n+1)) + n]);

	for (i = n - 1; i > 0; --i)
	{
		soma  = SLc->b[LTc[i]];
		for (j = i + 1; j <= n; ++j)
			soma -= SLc->A[(LTc[i] * (n+1)) + j] * SL->x[j];
		SL->x[i] = (soma / SLc->A[(LTc[i] * (n+1)) + i]);
	}
	//fazer para a última linha
	soma  = SLc->b[LTc[i]];
		for (j = i + 1; j <= n; ++j)
			soma -= SLc->A[(LTc[i] * (n+1)) + j] * SL->x[j];
		SL->x[i] = (soma / SLc->A[(LTc[i] * (n+1)) + i]);
	
	//FIM DA RETROSSUBSTITUIÇÃO
	
	LIKWID_MARKER_START("normaL2Residuo");
  	*normaL2 = normaL2Residuo (SL);
  	LIKWID_MARKER_STOP("normaL2Residuo");
	
	liberaSistLinear (SLc);
	free (LT);
	free (LTc);
	return (0);
}; //TERMINADO



/**
  * @brief Função que implementa o Método de Gauss-Jacobi, que é considerado um Método Iterativo
  * na resolução de sistemas lineares, visto que se baseiam na construção de sequências de aproximações.
  * @param SL Sistema Linear do qual se deseja calcular a solução.
  * @param eps Erro máximo da norma em x.
  * @param normaL2 Ponteiro que irá direcionar o valor da norma encontrada antes do método terminar de executar.
  * @param iter Ponteiro que irá direcionar o valor do número de iterações atingidas antes de abortar a execução do método.
  * @return -1 Caso haja algum problema na alocação de um Sistema Linear auxiliar, a fim de evitar alterações no sistema original.
  * @return -2 Caso atinja o máximo de iterações, ou seja, o método não convergiu.
  * @return 0 Caso obtenha sucesos em calcular a solução.
  * @see copiaSistLinear()
  * @see normaMAX()
  * @see copiaVetor()
  * @see normaL2Residuo()
  */
int  gaussJacobiOLD (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter)
{
	unsigned int n = SL->n;
	SistLinear_t *SLc;
	SLc = alocaSistLinear (n);
	
	if (!SLc)
	{
		fprintf(stderr, "Erro! Não foi possível alocar espaço para fazer a cópia da matriz.\n");
		return -1;
	}
	copiaSistLinear (SL, SLc, n);
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int fator = n + 1;
	unsigned int iFator = 0;
	unsigned int posicao = 0;
	
	
	//aloca o vetor de solucão x (x_next) da próxima iteração
	real_t *x_next;
	x_next = (real_t *) malloc (n * sizeof (real_t));

	
	//percorre o vetor de solução x (inicial) e inicializa com o valor nulo
	for (i = 0; i < n; ++i)
		SLc->x[i] = 0.0;


	//ajusta o vetor de coeficientes, de modo a simular os coeficientes do outro lado na equação
	for (i = 0; i < (n*n); ++i)
	{
		if (i == iFator)
			iFator += fator;
		else
			SLc->A[i] *= (-1);

	}
	

	//INÍCIO DO MÉTODO
	real_t soma, norma;
	*iter = 0;
	while (*iter < MAXIT)
	{
		(*iter)++;
		iFator = 0;
		for (i = 0; i < n; ++i) //trata da "linha"
		{
			soma = 0.0;
			for (j = 0; j < n; ++j) //trata da "coluna"
			{
				posicao = (i * n) + j;
				if (posicao != iFator)
					soma += (SLc->A[posicao] * SLc->x[j]);
			}
			x_next[i] = (SLc->b[i] + soma) / SLc->A[iFator];
			iFator += fator;
		}
		
		//CONDIÇÃO DE PARADA DO MÉTODO
		/*
		norma = normaMAX (x_next, SLc->x, n);
		if (norma < eps)
		{
			copiaVetor (x_next, SL->x, n);
  			*normaL2 = normaL2Residuo (SL);
			return 0;
		}
		*/

		copiaVetor (x_next, SLc->x, n); //origem -> destino
	}

	//*normaL2 = 0.0;
	//fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	//fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");

	liberaSistLinear (SLc);
	free (x_next);
	return 0;
}; //TERMINADO

int  gaussJacobiOPT (SistLinear_t * restrict SL, double eps, double * restrict normaL2, unsigned int * restrict iter)
{
	unsigned int n = SL->n;
	SistLinear_t *SLc;
	SLc = alocaSistLinear (n);
	
	if (!SLc)
	{
		fprintf(stderr, "Erro! Não foi possível alocar espaço para fazer a cópia da matriz.\n");
		return -1;
	}
	copiaSistLinear (SL, SLc, n);
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int fator = n + 1;
	unsigned int iFator = 0;
	unsigned int posicao = 0;
	unsigned int unroll_factor = 8;
	unsigned int idx = 0;
	
	
	//aloca o vetor de solucão x (x_next) da próxima iteração
	real_t *x_next;
	x_next = (real_t *) malloc (n * sizeof (real_t));

	
	//percorre o vetor de solução x (inicial) e inicializa com o valor nulo
	for (i = 0; i < n / unroll_factor; ++i)
	{
		SLc->x[i] = 0.0;
		SLc->x[i+1] = 0.0;
		SLc->x[i+2] = 0.0;
		SLc->x[i+3] = 0.0;
		SLc->x[i+4] = 0.0;
		SLc->x[i+5] = 0.0;
		SLc->x[i+6] = 0.0;
		SLc->x[i+7] = 0.0;
	}
	//remainder
	for (i = n - (n / unroll_factor); i < n; ++i)
	{
		SLc->x[i] = 0.0;
	}
	//-----------------------------------------------------

	//ajusta o vetor de coeficientes, de modo a simular os coeficientes do outro lado na equação
	for (i = 0; i < (n*n); ++i)
	{
		if (i == iFator)
			iFator += fator;
		else
			SLc->A[i] *= (-1);
	}

	//INÍCIO DO MÉTODO
	//real_t norma = eps + 1;
	real_t R0, R1, R2, R3, R4, R5, R6, R7, RPP1, RPP2, RPP3, RPP4, RP1, RP2, soma;
	*iter = 0;
	while (*iter < MAXIT)
	{
		(*iter)++;
		iFator = 0;
		idx = 0;
		for (i = 0; i < n; ++i) //trata da "linha"
		{
			R0 = R1 = R2 = R3 = R4 = R5 = R6 = R7 = soma = 0.0;
			posicao = (i * n);
			for (j = 0; j < n / unroll_factor; ++j) //trata da "coluna"
			{
				R0 += (SLc->A[posicao + j] * SLc->x[j]);
				R1 += (SLc->A[posicao + j+1] * SLc->x[j+1]);
				R2 += (SLc->A[posicao + j+2] * SLc->x[j+2]);
				R3 += (SLc->A[posicao + j+3] * SLc->x[j+3]);
				R4 += (SLc->A[posicao + j+4] * SLc->x[j+4]);
				R5 += (SLc->A[posicao + j+5] * SLc->x[j+5]);
				R6 += (SLc->A[posicao + j+6] * SLc->x[j+6]);
				R7 += (SLc->A[posicao + j+7] * SLc->x[j+7]);
			}
			//remainder
			for (j = n - (n / unroll_factor); j < n; ++j)
			{
				soma += (SLc->A[posicao+j] * SLc->x[j]);
			}

			RPP1 = R0 + R4;
          	RPP2 = R1 + R5;
          	RPP3 = R2 + R6;
          	RPP4 = R3 + R7;

          	RP1 = RPP1 + RPP2;
          	RP2 = RPP3 + RPP4;

          	//remove o elemento A[x][x] que foi multiplicado para remover a condição anterior do laço
          	soma -= (SLc->A[iFator] * SLc->x[idx]);

          	soma += RP1 + RP2;

			x_next[i] = (SLc->b[i] + soma) / SLc->A[iFator];
			iFator += fator;
			idx ++;
		}
		
		//CONDIÇÃO DE PARADA DO MÉTODO
		//norma = normaMAX (x_next, SLc->x, n);

		copiaVetor (x_next, SLc->x, n); //origem -> destino
	}

	//*normaL2 = 0.0;
	//printf("valor de norma = %g\n", norma);
	//fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	//fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");

	liberaSistLinear (SLc);
	free (x_next);
	return 0;
}; //TERMINADO



/**
  * @brief Função que implementa o Método de Gauss-Seidel, que também é um Método Iterativo
  * na resolução de sistemas lineares.
  * @param SL Sistema Linear do qual se deseja calcular a solução.
  * @param eps Erro máximo da norma em x.
  * @param normaL2 Ponteiro que irá direcionar o valor da norma encontrada antes do método terminar de executar.
  * @param iter Ponteiro que irá direcionar o valor do número de iterações atingidas antes de abortar a execução do método.
  * @return -1 Caso haja algum problema na alocação de um Sistema Linear auxiliar, a fim de evitar alterações no sistema original.
  * @return -2 Caso atinja o máximo de iterações, ou seja, o método não convergiu.
  * @return 0 Caso obtenha sucesos em calcular a solução.
  * @see copiaSistLinear()
  * @see normaMAX()
  * @see copiaVetor()
  * @see normaL2Residuo()
  */
int gaussSeidelOLD (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter)
{
	unsigned int n = SL->n;
	SistLinear_t *SLc;
	SLc = alocaSistLinear (n);
	
	if (!SLc)
	{
		fprintf(stderr, "Erro! Não foi possível alocar espaço para fazer a cópia da matriz.\n");
		return -1;
	}
	copiaSistLinear (SL, SLc, n);
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int fator = n + 1;
	unsigned int iFator = 0;
	unsigned int posicao = 0;
	unsigned int marcador;
	unsigned int contador;
	
	
	//aloca o vetor de solucão x (x_next) da próxima iteração
	real_t *x_next;
	x_next = (real_t *) malloc (n * sizeof (real_t));

	
	//percorre o vetor de solução x (inicial) e inicializa com o valor nulo
	for (i = 0; i < n; ++i)
		SLc->x[i] = 0.0;


	//ajusta o vetor de coeficientes, de modo a simular os coeficientes do outro lado na equação
	for (i = 0; i < (n*n); ++i)
	{
		if (i == iFator)
			iFator += fator;
		else
			SLc->A[i] *= (-1);

	}
	

	//INÍCIO DO MÉTODO
	real_t soma, norma;
	*iter = 0;
	while (*iter < MAXIT)
	{
		(*iter)++;
		iFator = 0;
		marcador = 0; //conta quantos disponíveis já existem
		for (i = 0; i < n; ++i) //trata da "linha"
		{
			soma = 0.0;
			contador = 0;
			for (j = 0; j < n; ++j) //trata da "coluna"
			{
				posicao = (i * n) + j;
				if (posicao != iFator)
				{
					if ((marcador > 0) && (contador < i))
					{
						soma += (SLc->A[posicao] * x_next[contador]);
						contador ++;
					}
					else
					{
						soma += (SLc->A[posicao] * SLc->x[j]);
					}
				}
			}
			x_next[i] = (SLc->b[i] + soma) / SLc->A[iFator];
			iFator += fator;
			marcador++;
		}
		
		//CONDIÇÃO DE PARADA DO MÉTODO
		/*
		norma = normaMAX (x_next, SLc->x, n);
		if (norma < eps)
		{
			copiaVetor (x_next, SL->x, n);
  			*normaL2 = normaL2Residuo (SL);
			return 0;
		}
		*/
		copiaVetor (x_next, SLc->x, n); //origem -> destino
	}

	*normaL2 = 0.0;
	//fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	//fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");
	liberaSistLinear (SLc);
	free (x_next);
	return 0;
} //TERMINADO

int gaussSeidelOPT (SistLinear_t * restrict SL, double eps, double * restrict normaL2, unsigned int * restrict iter)
{
	unsigned int n = SL->n;
	SistLinear_t *SLc;
	SLc = alocaSistLinear (n);
	
	if (!SLc)
	{
		fprintf(stderr, "Erro! Não foi possível alocar espaço para fazer a cópia da matriz.\n");
		return -1;
	}
	copiaSistLinear (SL, SLc, n);
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int fator = n + 1;
	unsigned int iFator = 0;
	unsigned int posicao = 0;
	unsigned int unroll_factor = 8;
	
	
	//aloca o vetor de solucão x (x_next) da próxima iteração
	real_t *x_next;
	x_next = (real_t *) malloc (n * sizeof (real_t));

	
	//percorre o vetor de solução x (inicial) e o vetor da próxima iteração, inicializando-os com o valor nulo
	for (i = 0; i < n / unroll_factor; ++i)
	{
		SLc->x[i] = 0.0;
		SLc->x[i+1] = 0.0;
		SLc->x[i+2] = 0.0;
		SLc->x[i+3] = 0.0;
		SLc->x[i+4] = 0.0;
		SLc->x[i+5] = 0.0;
		SLc->x[i+6] = 0.0;
		SLc->x[i+7] = 0.0;

		x_next[i] = 0.0;
		x_next[i+1] = 0.0;
		x_next[i+2] = 0.0;
		x_next[i+3] = 0.0;
		x_next[i+4] = 0.0;
		x_next[i+5] = 0.0;
		x_next[i+6] = 0.0;
		x_next[i+7] = 0.0;
	}
	//remainder
	for (i = n - (n / unroll_factor); i < n; ++i)
	{
		SLc->x[i] = 0.0;
		x_next[i] = 0.0;
	}
	//-----------------------------------------------------


	//ajusta o vetor de coeficientes, de modo a simular os coeficientes do outro lado na equação
	for (i = 0; i < (n*n); ++i)
	{
		if (i == iFator)
			iFator += fator;
		else
			SLc->A[i] *= (-1);

	}
	

	//INÍCIO DO MÉTODO
	//real_t norma = eps + 1;
	real_t R0, R1, R2, R3, R4, R5, R6, R7, RPP1, RPP2, RPP3, RPP4, RP1, RP2, soma;
	*iter = 0;
	while (*iter < MAXIT)
	{
		(*iter)++;

		copiaVetor (x_next, SLc->x, n); //origem -> destino

		for (i = 0; i < n; ++i) //trata da "linha"
		{
			R0 = R1 = R2 = R3 = R4 = R5 = R6 = R7 = soma = 0.0;
			posicao = (i * n);
			for (j = 0; j < n / unroll_factor; ++j) //trata da "coluna" quando há disponível um novo valor de iteração
			{
				R0 += (SLc->A[posicao + j] * x_next[j]);
				R1 += (SLc->A[posicao + j+1] * x_next[j+1]);
				R2 += (SLc->A[posicao + j+2] * x_next[j+2]);
				R3 += (SLc->A[posicao + j+3] * x_next[j+3]);
				R4 += (SLc->A[posicao + j+4] * x_next[j+4]);
				R5 += (SLc->A[posicao + j+5] * x_next[j+5]);
				R6 += (SLc->A[posicao + j+6] * x_next[j+6]);
				R7 += (SLc->A[posicao + j+7] * x_next[j+7]);
			}
			//remainder
			for (j = n - (n / unroll_factor); j < n; ++j)
			{
				soma += (SLc->A[posicao + j] * x_next[j]);
			}

		
			for (j = i + 1; j < n / unroll_factor; ++j) //trata da "coluna" quando não há disponível um novo valor de iteração
			{
				R0 += (SLc->A[posicao + j] * SLc->x[j]);
				R1 += (SLc->A[posicao + j+1] * SLc->x[j+1]);
				R2 += (SLc->A[posicao + j+2] * SLc->x[j+2]);
				R3 += (SLc->A[posicao + j+3] * SLc->x[j+3]);
				R4 += (SLc->A[posicao + j+4] * SLc->x[j+4]);
				R5 += (SLc->A[posicao + j+5] * SLc->x[j+5]);
				R6 += (SLc->A[posicao + j+6] * SLc->x[j+6]);
				R7 += (SLc->A[posicao + j+7] * SLc->x[j+7]);
			}
			//remainder
			for (j = n - (n / unroll_factor); j < n; ++j)
			{
				soma += (SLc->A[posicao + j] * SLc->x[j]);
			}


			RPP1 = R0 + R4;
          	RPP2 = R1 + R5;
          	RPP3 = R2 + R6;
          	RPP4 = R3 + R7;

          	RP1 = RPP1 + RPP2;
          	RP2 = RPP3 + RPP4;

          	soma += RP1 + RP2;
			x_next[i] = (SLc->b[i] + soma) / SLc->A[posicao + i];
		}
		
		//CONDIÇÃO DE PARADA DO MÉTODO
		//norma = normaMAX (x_next, SLc->x, n);
	} 

	*normaL2 = 0.0;
	//fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	//fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");
	liberaSistLinear (SLc);
	free (x_next);
	return 0;
}; //TERMINADO

//---------------------------------------------FUNÇÕES AUXILIARES----------------------------------------------//
/**
  * @brief Função que faz a cópia Do Sistema Linear original, a fim de evitar alterações em sua estrutura.
  * @param SL Sistema Linear do qual se deseja fazer a cópia.
  * @param SLc Sistema Linear do qual será usado como cópia.
  * @param n Tamanho do sistema linear.
  */
void copiaSistLinear (SistLinear_t *SL, SistLinear_t *SLc, unsigned int n)
{
	unsigned int i;
	unsigned int size = n * n;
	unsigned int unroll_factor = 8;
	
	//COPIA SISTEMA LINEAR
	for (i = 0; i < size / unroll_factor; ++i)
	{
		SLc->A[i] = SL->A[i];
		SLc->A[i+1] = SL->A[i+1];
		SLc->A[i+2] = SL->A[i+2];
		SLc->A[i+3] = SL->A[i+3];
		SLc->A[i+4] = SL->A[i+4];
		SLc->A[i+5] = SL->A[i+5];
		SLc->A[i+6] = SL->A[i+6];
		SLc->A[i+7] = SL->A[i+7];

	}
	//remainder
	for (i = size - (size / unroll_factor); i < size; ++i)
	{
		SLc->A[i] = SL->A[i];
	}
	

	//COPIAR VETOR
	for (i = 0; i < n / unroll_factor; ++i)
	{
		SLc->b[i] = SL->b[i];
		SLc->b[i+1] = SL->b[i+1];
		SLc->b[i+2] = SL->b[i+2];
		SLc->b[i+3] = SL->b[i+3];
		SLc->b[i+4] = SL->b[i+4];
		SLc->b[i+5] = SL->b[i+5];
		SLc->b[i+6] = SL->b[i+6];
		SLc->b[i+7] = SL->b[i+7];
	}
	//remainder
	for (i = n - (n / unroll_factor); i < n; ++i)
	{
		SLc->b[i] = SL->b[i];
	}
}; //TERMINADO

/**
  * @brief Função que faz a cópia de um vetor.
  * @param origem Vetor de origem.
  * @param destino Vetor de destino.
  * @param n Tamanho do sistema linear.
  */
void copiaVetor (real_t *origem, real_t *destino, unsigned int n)
{
	unsigned int i;
	unsigned int unroll_factor = 8;
	for (i = 0; i < n / unroll_factor; ++i)
	{
		destino[i] = origem[i];
		destino[i+1] = origem[i+1];
		destino[i+2] = origem[i+2];
		destino[i+3] = origem[i+3];
		destino[i+4] = origem[i+4];
		destino[i+5] = origem[i+5];
		destino[i+6] = origem[i+6];
		destino[i+7] = origem[i+7];
	}

	//remainder
	for (i = n - (n / unroll_factor); i < n; ++i)
	{
		destino[i] = origem[i];
	}
} //TERMINADO

/**
  * @brief Função busca a linha do maior valor em uma determianda coluna (mais conhecido como Pivô).
  * @param A Vetor de coeficientes do qual será buscado.
  * @param LT Lookup Table usado para direcionar a busca.
  * @param COL A coluna que deve ser buscada.
  * @param n Tamanho do sistema linear.
  * @return Retorna a linha que deveria estar o pivo.
  */
unsigned int encontraMax (real_t *A, unsigned int *LT, unsigned int COL, unsigned int n)
{
	unsigned int i = COL;
	unsigned int linha = LT[i];
	real_t a1 = A[(LT[i] * n) + COL];

	for (unsigned int cont = COL; cont < (n - 1); ++cont)
	{
		++i;
		real_t a2 = A[(LT[i] * n) + COL];
		if (fabs(a2) > fabs(a1))
		{
			a1 = a2;
			linha = LT[i];
		}
	}
	return linha;
}; //TERMINADO

/**
  * @brief Função que calcula a norma máxima do erro aproximado em x.
  * @param next Vetor da próxima iteração.
  * @param prev Vetor da iteração anterior.
  * @param n Tamanho do sistema linear.
  * @return O valor encontrado para a norma.
  */
real_t normaMAX (real_t *next, real_t *prev, unsigned int n)
{
	real_t maior, sub;
	maior = fabs (next[0] - prev[0]);
	unsigned int i;
	for (i = 1; i < n; ++i)
	{
		sub = fabs (next[i] - prev[i]);
		if (sub > maior)
			maior = sub;
	}
	return maior;
}; //TERMINADO
//-------------------------------------------------------------------------------------------------------------//