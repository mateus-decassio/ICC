#include <stdio.h>
#include <math.h>
#include <float.h>
#include "utils.h"
#include "ZeroFuncao.h"
#include <limits.h>

double f1 (const double x){
	double soma;
	soma = x*x -9*x -4;
	return soma/6;
}

double f2 (const double x){
	return sin(x*x*x -x +1) - 0.5;
}

double f3 (const double x){
	return (4*x*x*x*x*x - 2*x*x*x*x - 3*x*x*x -5*x*x -3*x + 2);
}

double dxf3 (const double x){
	return (20*x*x*x*x - 8*x*x*x - 6*x*x -10*x -3);
}

double f4 (const double x){
	return sin(x*x*x);
}

double f5 (const double x){
	return (x*x - 5*x + 7);
}

double dxf5 (const double x){
	return (2*x - 5);
}

int main (){
//------------------------------------TESTE DO CÁLCULO DAS RAÍZES------------------------------------//
	/*
	double raiz, eps = 0.0001;
	int error, iter;
	printf("EXECUTANTO O TESTE DE CÁLCULO DAS RAÍZES DAS FUNÇÕES...\n\n");


	error = bisseccao (f1, 9, 10, eps, &iter, &raiz);
	printf("(f1): PELO MÉTODO DA BISSECÇÃO, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = bisseccao (f2, 1.2, 1.6, eps, &iter, &raiz);
	printf("(f2): PELO MÉTODO DA BISSECÇÃO, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = bisseccao (f3, 0.0, 0.5, eps, &iter, &raiz);
	printf("(f3): PELO MÉTODO DA BISSECÇÃO, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = bisseccao (f4, -0.5, 0.5, eps, &iter, &raiz);
	printf("(f4): PELO MÉTODO DA BISSECÇÃO, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = bisseccao (f5, 2, 3, eps, &iter, &raiz);
	printf("(f5): PELO MÉTODO DA BISSECÇÃO, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	printf("----------------------------------------------------------------------------------------\n");
	
	
	
	error = newton (f3, dxf3, 0.0, eps, &iter, &raiz);
	printf("(f3): PELO MÉTODO DE NEWTON, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = newton (f5, dxf5, 2.0, eps, &iter, &raiz);
	printf("(f5): PELO MÉTODO DE NEWTON, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	

	printf("----------------------------------------------------------------------------------------\n");


	error = secante (f3, 0.0, 0.5, eps, &iter, &raiz);
	printf("(f3): PELO MÉTODO DA SECANTE, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);

	error = secante (f5, 2.0, 3.0, eps, &iter, &raiz);
	printf("(f3): PELO MÉTODO DA SECANTE, O VALOR OBTIDO DA RAIZ FOI DE %g EM %d ITERAÇÕES\n",raiz, iter);
	*/
//------------------------------------TESTE DO CÁLCULO E DA DERIVADA------------------------------------//
	
	Polinomio equacao, teste;
	unsigned int contador = 0;
	double i = 0.97923;
	int verificador;
	
	printf("EXECUTANTO O TESTE DE CÁLCULO DO POLINÔMIO E SUA DERIVADA PRIMEIRA...\n\n");
	
	equacao. grau = 10;
	unsigned int tamanho = equacao.grau + 1;
	equacao.p = malloc (tamanho * sizeof(double));
	
	printf("PREENCHENDO O VETOR...\n");
	for (contador = 0; contador < tamanho; contador ++){
		equacao.p[contador] = i;
		i = i + 4.56412;
	}
	
	double x = 3.872;
	double *px;
	double *dpx;
	verificador = calcPolinomioEDerivada (equacao, x, px, dpx);
	if (verificador == 0){
		printf("O valor do polinômio no ponto %.5e = %.10e\n",x, *px);
		printf("O valor da derivada do polinômio no ponto %.5e = %.10e\n",x, *dpx);
	}
	
//---------------------------------------------------------------------------------------//	
//------------------------------------TESTE DA MÉDIA------------------------------------//
	/*
	printf("EXECUTANTO O TESTE DA MÉDIA...\n\n");
	unsigned int tamanho = 294967295;
	
	unsigned int contador = 0;
	unsigned int tras = 0;
	double media_valor;
	double i = 0.00000000098498920549;
	double *valores;

	//99400000
	//4294967295
	valores = malloc (tamanho  * sizeof (double));
	if (valores == NULL){
		printf("NÃO CONSEGUIU ALOCAR ESPAÇO PARA ESSE VETOR!\n");
		return 0;
	}
	
	
	tras = tamanho-1;
	printf("PREENCHENDO O VETOR...\n");
	for (contador = 0; contador < tamanho; contador ++){
		valores[tras] = i + 0.003521;
		i = i + 0.00000788768;
		tras --;
	}

	printf("CALCULANDO O VALOR DA MÉDIA...\n");
	media_valor = media (valores, tamanho);
	printf("O VALOR DA MÉDIA OBTIDO FOI DE: %g\n",media_valor);
	*/
//---------------------------------------------------------------------------------------//	
	printf("TODOS OS TESTES CONCLUÍDOS!\n");
	printf("end:main\n");	
  	return 0;
}

