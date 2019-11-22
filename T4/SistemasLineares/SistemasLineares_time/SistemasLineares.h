#ifndef __SISLINEAR_H__
#define __SISLINEAR_H__

// Parâmetros para teste de convergência
#define MAXIT 5
#define EPS 1.0e-4
#define DIAG 32
#define DEF_SIZE 512 //incluído


typedef double real_t;

typedef struct {
  real_t *A; // coeficientes
  real_t *b; // termos independentes
  real_t *x; // solucao
  unsigned int n; // tamanho do SL
} SistLinear_t;

// Alocação e desalocação de memória
SistLinear_t* alocaSistLinear (unsigned int n);
void liberaSistLinear (SistLinear_t *SL);

//Inicialização do Sistema Linear
void inicializaSistLinear (SistLinear_t *SL);

// Medição de tempo
double timestamp (void);

// Calcula a normaL2 do resíduo
double normaL2Residuo(SistLinear_t *SL);

// Método da Eliminação de Gauss-Jordan
int eliminacaoGauss (SistLinear_t *SL, double *normaL2);

// Método de Gauss-Jacobi
int gaussJacobiOLD (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter);
int gaussJacobiOPT (SistLinear_t * restrict SL, double eps, double * restrict normaL2, unsigned int * restrict iter);

// Método de Gauss-Seidel
int gaussSeidelOLD (SistLinear_t *SL, double eps, double *normaL2, unsigned int *iter);
int gaussSeidelOPT (SistLinear_t * restrict SL, double eps, double * restrict normaL2, unsigned int * restrict iter);

#endif // __SISLINEAR_H__

