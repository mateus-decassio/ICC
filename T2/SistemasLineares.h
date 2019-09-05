#ifndef __SISLINEAR_H__
#define __SISLINEAR_H__

// Parâmetros para teste de convergência
#define MAXIT 500
#define EPS 1.0e-4

typedef double real_t;

typedef struct {
  real_t *A; // coeficientes [MATRIZ]
  real_t *b; // termos independentes [VETOR]
  real_t *x; // solucao [VETOR]
  unsigned int n; // tamanho do SL
} SistLinear_t;

// Alocaçao e desalocação de memória
SistLinear_t* alocaSistLinear (unsigned int n);
void liberaSistLinear (SistLinear_t *SL);

// Calcula a normaL2 do resíduo
double normaL2Residuo(SistLinear_t *SL);

// Método da Eliminação de Gauss-Jordan
int eliminacaoGauss (SistLinear_t *SL, double *normaL2);

// Método de Gauss-Jacobi
int gaussJacobi (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter);

// Método de Gauss-Seidel
int gaussSeidel (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter);

#endif // __SISLINEAR_H__

