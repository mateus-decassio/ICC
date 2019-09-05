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
#include "utils.h"
#include "SistemasLineares.h"

//-----------------------------------NÃO ALTERAR-----------------------------------//
//Alocação de memória
SistLinear_t* alocaSistLinear (unsigned int n)
{
	SistLinear_t *SL = (SistLinear_t *) malloc(sizeof(SistLinear_t));
	if ( SL ) 
	{
		SL->A = (real_t *) malloc(n * n * sizeof(real_t)); //VETOR DE COEFICIENTES
		SL->b = (real_t *) malloc(n * sizeof(real_t)); 	   //VETOR DE TERMOS INDEPENDENTES
		SL->x = (real_t *) malloc(n * sizeof(real_t));	   //VETOR DE INCÓGNITAS
		if (!(SL->A) || !(SL->b) || !(SL->x))
			liberaSistLinear (SL);
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
		soma += (residuo[i] * residuo[i]);

	
	free (residuo);
	return sqrt (soma);
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
	
	*normaL2 = normaL2Residuo (SL);
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
int  gaussJacobi (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter)
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
	while (*iter < 500)
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
		norma = normaMAX (x_next, SLc->x, n);
		if (norma < eps)
		{
			copiaVetor (x_next, SL->x, n);
			*normaL2 = normaL2Residuo (SL);
			return 0;
		}

		copiaVetor (x_next, SLc->x, n);
	}

	*normaL2 = 0.0;
	fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");

	liberaSistLinear (SLc);
	free (x_next);
	return -2;
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
int gaussSeidel (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter)
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
	while (*iter < 500)
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
		norma = normaMAX (x_next, SLc->x, n);
		if (norma < eps)
		{
			copiaVetor (x_next, SL->x, n);
			*normaL2 = normaL2Residuo (SL);
			return 0;
		}

		copiaVetor (x_next, SLc->x, n);
	}

	*normaL2 = 0.0;
	fprintf(stderr, "ATENÇÃO! O método atingiu o número máximo de iterações definido em MAXIT.\n");
	fprintf(stderr, "Por favor, desconsidere o valor de retorno da normaL2 visto que o método não convergiu.\n");
	return -2;
} //TERMINADO

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
	for (i = 0; i < (n * n); ++i)
		SLc->A[i] = SL->A[i];

	for (i = 0; i < n; ++i)
		SLc->b[i] = SL->b[i];
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
	for (i = 0; i < n; ++i)
		destino[i] = origem[i];
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