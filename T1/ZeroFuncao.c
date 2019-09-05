/**
  * @file ZeroFuncao.c
  * @version 1.0
  * @author Mateus Felipe de Cássio Ferreira (GRR 20176123)
  * @section DESCRIPTION
  * Essa biblioteca tem como objetivo implementar as rotinas mais comuns no cálculo de raízes de funções f(x)
  * definidas e contínuas em um intervalo I. Ainda, são implementadas rotinas para calcular o valor de um 
  * polinômio em um ponto, bem como a sua derivada; e um método de cálculo da média de n valores em ponto flutuante.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "utils.h"
#include "ZeroFuncao.h"
//-----------------------------------------ESCOPO DE FUNÇÕES AUXILIARES-----------------------------------------//

int cmpfunc (const void *a, const void *b);

//-------------------------------------IMPLEMENTAÇÃO DE FUNÇÕES AUXILIARES-------------------------------------//
/**
  * @brief Função de comparação de dois números que é utilizada pelo qsort na função do cálculo da média.
  * @param a Primeiro elemento a ser comparado;
  * @param b Segundo elemento a ser comparado.
  * @return -1 Se a < b;
  * @return 0 Se a == b;
  * @return 1 Se a > b.
  * @see media()
  */
int cmpfunc (const void *a, const void *b) {
   return (*(double*)a - *(double*)b); 
} 

//---------------------------------------------------------------------------------------------------------------//
/**
  * @brief Função responsável por implementar o Método da Bissecção para encontrar a raiz de uma
  * função f(x) contínua e definida no intervalo [a,b].
  * @param *f Ponteiro para a função da qual se deseja calcular a raiz;
  * @param a Limite inferior do intervalo em que, provavelmente, a raiz se encontra;
  * @param b Limite superior do intervalo em que, provavelmente, a raiz se encontra;
  * @param eps Erro relativo máximo em x;
  * @param *it Ponteiro que irá direcionar o número de iterações feitas quando o método terminar de executar;
  * @param *raiz Ponteiro que irá direcionar o valor da raiz encontrada quando o método terminar de executar.
  * @return -1 Caso o número de iterações do método atinja um limite definido em MAXIT (500). Nesse caso, desconsidera-se o valor de retorno da raiz;
  * @return 0 Caso o método consiga encontrar uma raiz que esteja dentro do limite tolerável de erro definido, pelo usuário, em eps.
  */
int bisseccao (double (*f)(const double x), double a, double b,
               double eps, int *it, double *raiz)
{
	double xm_new, xm_old, fa, fb, fxm, ERr, verificador;

	fa = f(a);
	fb = f(b);
	*it = 0;
	
	if ((fa*fb) < 0.0){ //condição fundamental para entrar no método da Bissecção
		xm_new = (a + b) / 2.0;
		while (*it < MAXIT){
			xm_old = xm_new;
			fxm = f(xm_old);
			verificador = (fa * fxm);
			if (verificador < 0.0){
				b = xm_old;
			} else if (verificador > 0.0){
				a = xm_old;
				fa = fxm;
			} else{
				*raiz = xm_old;
				return 0; 
			}
			xm_new = (a + b) / 2.0;
			

			//CONDIÇÃO DE PARADA DO MÉTODO
			ERr = (fabs (xm_new - xm_old) / fabs (xm_new)); //determina o erro relativo real
			if (ERr < eps){
				*raiz = xm_new;
				return 0;
			}

			(*it)++;
		}
	} else if (f(0.0) == 0){ //testar se 0.0 é raiz da equação (um dos casos do material)
		*raiz = 0.0;
		fprintf(stderr, "Apesar de não satisfazer a conição fundamental do método, foi possível calcular o valor da raiz pois atendia a uma das situações previstas.\n");
		return 0; 
	} else { // NÃO ACHOU A RAIZ
		fprintf(stderr, "Não foi possível calcular o valor da raiz dessa função nesse intervalo pelo Método da Bissecção.\n");
		fprintf(stderr, "Por favor, escolha um outro intervalo e desconsidere o valor de retorno da raiz.\n");
		*raiz = 0.0;
		return -1;
	}
}; 

/**
  * @brief Função responsável por implementar o Método de Newton para encontrar a raiz de uma
  * função f(x) também contínua e definida. 
  * @remarks Esse método está dentro do grupo dos chamados Métodos de Ponto Fixo,
  * que utilizam uma função de Iteração para convergir para a raiz. Nesse caso, utiliza-se, como função
  * de iteração, a derivada primeira da função em um ponto (x), obtendo a reta tangente ao ponto para convergir para a raiz.
  * Caso a derivada primeira de um ponto durante o método seja zero, é implementado uma tentativa de deslocamento de xi para continuar a 
  * convergência para a raiz.
  * @param *f Ponteiro para a função da qual se deseja calcular a raiz;
  * @param *df Ponteiro para a função do cálculo da derivada primeira da função passada anteriormente;
  * @param x0 Ponto inicial de procura pela raiz da equação;
  * @param eps Erro relativo máximo em x;
  * @param *it Ponteiro que irá direcionar o número de iterações feitas quando o método terminar de executar;
  * @param *raiz Ponteiro que irá direcionar o valor da raiz encontrada quando o método terminar de executar.
  * @return -4 Caso o número de iterações do método atinja um limite definido em MAXIT (500).
  * Provavelmente, nesse caso, o método ficou preso em um ponto de máximo / mínimo da função ou ficou alternando a busca das raízes. Nesse caso, desconsidera-se o valor de retorno da raiz;
  * @return -3 Caso não seja possível, pela implementação, de deslocar o valor de xi quando atinge-se um ponto de inflexão. Nesse caso, desconsidera-se o valor de retorno da raiz;
  * @return -2 Caso a derivada primeira da função dada, no valor inical x0, seja zero. Nesse caso, para evitar que ocorra divisão por zero, solicita-se ao usuário para digitar um novo valor de x0
  * e desconsidera-se o valor de retorno da raiz;
  * @return 0 Caso o método consiga encontrar uma raiz que esteja dentro do limite tolerável de erro definido, pelo usuário, em eps.
  */
int newton (double (*f)(const double x), double (*df)(const double x), double x0, 
            double eps, int *it, double *raiz)
{
	double dfx, ERa, fx, x1;
	short sinal, contador;
	
	*it = 0;
	dfx = df(x0);
	if (dfx == 0){
		fprintf (stderr, "Não foi possível calcular a raiz dessa função pelo Método de Newton pois a derivada nesse ponto é zero.\n");
		fprintf (stderr, "Por favor, entre com um valor diferente de x0e desconsidere o valor de retorno da raiz.\n");
		*raiz = 0.0;
		return -2;
	} else {
		while (*it < MAXIT){
			fx = f(x0);
			x1 = x0 - (fx/dfx);

			
			//CONDIÇÃO DE PARADA DO MÉTODO
			ERa = (fabs (x1 - x0) / fabs (x1)); //determina o erro relativo aproximado
			if (ERa < eps){
				*raiz = x1;
				return 0;
			}
			
			//VERIFICAÇÃO DO SENTIDO DA CONVERGÊNCIA
			if (x0 > x1)
				sinal = -1; //convergindo para a esquerda
			else
				sinal = 1; //convergindo para a direita

			
			x0 = x1;
			(*it)++;
			dfx = df(x0);
			
			//ROTINA PARA TRATAR O CASO DE INFLEXÃO
			if (dfx == 0){
				contador = 0;
				fprintf (stderr, "Atenção! O valor de xi chegou a um ponto de inflexão.\n");
				fprintf (stderr, "Um novo xi será levemente deslocado para a esquerda ou para a direita.\n");
				while ((dfx == 0) && contador < 100){
					x0 = x0 + (sinal * 10 * DBL_EPSILON);
					dfx = df(x0);
					contador ++;
				}
				if (contador == 100){
					fprintf (stderr, "Não foi possível calcular a raiz da equação utilizando esse método pois xi entrou em uma região de inflexão e não conseguiu sair.\n");
					fprintf (stderr, "Por favor, escolha outro valor de entrada x0 e desconsidere o valor de retorno da raiz\n");
					*raiz = 0.0;
					return -3;				
				}
			} //end if (dfx == 0) 
		}//end while
		
		//ROTINA PARA TRATAR O CASO DOS MÁXIMOS E MÍNIMOS E/OU ALTERNANDO RAÍZES
		if (*it == MAXIT){ //atingiu o máximo de iterações possíveis
			fx = f(x0);
			if (fx > 0.2 || fx < -0.2){ //arbitrariamente escolhido
				fprintf (stderr, "Erro! Não foi possível calcular a raiz dessa função pelo Método de Newton.\n");
				fprintf (stderr, "Provavelmente, em função do x0 escolhido, a resolução com esse método ficou preso em um ponto de máximo / mínimo da função ou alternando as raízes.\n");
				fprintf (stderr, "Por favor, entre com um novo valor de x0 e desconsidere o valor de retorno da raiz.\n");
				*raiz = 0.0;
				return -4;
			}
		}
	}//end else
}; 

/**
  * @brief Função responsável por implementar o Método das Secantes para encontrar a raiz de uma
  * função f(x) também contínua e definida. 
  * @remarks Esse método também está dentro do grupo dos chamados Métodos de Ponto Fixo,
  * que utilizam uma função de Iteração para convergir para a raiz. Nesse caso, diferentemente do Método de Newton,
  * a função de iteração não necessita do cálculo de uma derivada. Utiliza-se uma reta secante aos pontos para convergir para a raiz.
  * @param *f Ponteiro para a função da qual se deseja calcular a raiz;
  * @param x0 Primeiro ponto inicial de procura pela raiz da equação;
  * @param x1 Segundo ponto inicial de procura pela raiz da equação;
  * @param eps Erro relativo máximo em x;
  * @param *it Ponteiro que irá direcionar o número de iterações feitas quando o método terminar de executar;
  * @param *raiz Ponteiro que irá direcionar o valor da raiz encontrada quando o método terminar de executar.
  * @return -5 Caso o número de iterações do método atinja um limite definido em MAXIT (500). Nesse caso, desconsidera-se o valor de retorno da raiz;
  * @return 0 Caso o método consiga encontrar uma raiz que esteja dentro do limite tolerável de erro definido, pelo usuário, em eps.
  */
int secante (double (*f)(const double x), double x0, double x1, 
             double eps, int *it, double *raiz)
{
	double ERa, fx, x2, numerador, denominador;
	
	
	*it = 0;
	while (*it < MAXIT){
		fx = f(x1);
		numerador = fx * (x1 - x0);
		denominador = (fx - f(x0));
		x2 = x1 - (numerador/denominador);

		//CONDIÇÃO DE PARADA DO MÉTODO
		ERa = (fabs(x2 - x1)/ fabs(x2));
		if (ERa < eps){
			*raiz = x2;
			return 0; 
		}
		x0 = x1;
		x1 = x2;
		(*it)++;
	}
	fprintf(stderr, "Erro! O método da Secante atingiu o limite de iterações e não pôde devolver um valor da raiz dessa equação.\n");
	fprintf(stderr, "Por favor, escolha novos valores para x0 e x1 e desconsidere o valor de retorno da raiz.\n");
	*raiz = 0.0;
	return -5;
}; 

/**
  * @brief Função responsável por calcular o valor de um polinômio em um ponto x e sua derivada primeira f'(x).
  * @remarks Nessa função, percorre-se apenas um vez no vetor de coeficientes, fazendo, simultaneamente, o cálculo do polinômio
  * e da derivada naquele ponto.
  * @param p Estrutura de dados contendo o valor do grau e o vetor de coeficientes do Polinômio;
  * @param x Valor do qual se deseja saber o resultado do polinômio e a sua derivada;
  * @param *px Ponteiro que irá direcionar o valor obtido do cálculo do polinômio no ponto x;
  * @param *dpx Ponteiro que irá direcionar o valor obtido do cálculo da derivada no ponto x.
  * @return -7 Caso o tamanho desse polinômio não seja válido para o cálculo;
  * @return -6 Caso haja um problema na alocação da variável que irá receber o valor de px ou dpx;
  * @return 0 Caso tenha sido possível calcular o valor do polinômio no ponto x e de sua derivada primeira.
  */
int calcPolinomioEDerivada (Polinomio p, double x, double *px, double *dpx)
{
	//if (*px != 0 || *dpx != 0){
		//fprintf(stderr, "Não foi possível realizar o cálculo do Polinômio e da Derivada pois *px ou *dpx não estão alocados para receber os valores de saída da função.\n");
		//return -6;
	//}
	
	if (p.grau == 0){
		*px = p.p[0];
		return 0;
	} else if (p.grau < 0){
		fprintf (stderr, "Não foi possível realizar o cálculo do Polinômio e da Derivada pois esse tamanho de polinômio é incompatível.\n");
		return -7;
	}

	unsigned int indice_anterior = p.grau - 1;
	unsigned int contador = 1;
	double soma = p.p[p.grau];
	double soma_derivada = p.p[p.grau] * p.grau;
	
	while (contador <= p.grau){
		//CÁLCULO DO POLINÔMIO EM UM PONTO
		soma = (soma * x) + p.p[indice_anterior];
	
		//CÁLCULO DA DERIVADA PRIMEIRA NESSE PONTO
		if (contador < p.grau){
			soma_derivada = (soma_derivada * x) + (p.p[indice_anterior] * indice_anterior);
		}
		
		indice_anterior --;
		contador ++;
	}
	*px = soma;
	*dpx = soma_derivada;
	return 0; 
}; 

/**
  * @brief Função responsável por calcular o valor da média de n valores do tipo double. 
  * @remarks Nessa implementação, utiliza-se
  * um tamanho máximo em que poderá ser feita uma ordenação do vetor para minimizar os erros numéricos que podem acontecer
  * utilizando-se o método usual. Essa escolha foi em função do pior caso de ordenação do QuickSort ser da ordem O(n^2).
  * @param *valores Vetor de valores do tipo double;
  * @param n Tamanho do vetor.
  * @return Valor da média obtida.
  * @see cmpfunc ()
  */
double media (double *valores, unsigned long n)
{
	if (n == 0){
		fprintf(stderr, "Não foi possível efetuar o cálculo da média pois o tamanho (n) do vetor é zero.\n" );
		return -8;
	}

	if (n <= 294967295){ //limite para ordenação do vetor (mais do que isso custa muito caro e demora muito tempo)
		qsort (valores, n, sizeof(double), cmpfunc);
	}

	unsigned long contador = 0;
	double somatorio = 0.0;
	for (contador = 0; contador < n; ++ contador)
		somatorio = somatorio + valores[contador];
	
	return (somatorio / n);
}; 