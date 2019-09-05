#ifndef __ICCTRAB01_H__
#define __ICCTRAB01_H__


// Parâmetros para teste de convergência
#define MAXIT 500

typedef struct {
  double *p;
  unsigned int grau;
} Polinomio;

// Métodos de zero de Função

int bisseccao (double (*f)(const double x), double a, double b,
               double eps, int *it, double *raiz);

int newton (double (*f)(const double x), double (*df)(const double x), double x0, 
            double eps, int *it, double *raiz);

int secante (double (*f)(const double x), double x0, double x1, 
             double eps, int *it, double *raiz);

// Cálculo de Polinômios
int calcPolinomioEDerivada(Polinomio p, double x, double *px, double *dpx);

// Cálculo de Média
double media(double *valores, unsigned long n);

#endif // __ICCTRAB01_H__

