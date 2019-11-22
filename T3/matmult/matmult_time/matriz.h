/* Constantes */

#define DBL_FIELD "%12.7lg"
#define SEP_RES "\n\n\n"

#define DEF_SIZE 512
#define DIAG 32


#define ABS(num)  ((num) < 0.0 ? -(num) : (num))

/* Implementações  para matrizes e vetores */

typedef double ** MatPtr;
typedef double * MatRow;
typedef double * MatCol;

typedef double * Vetor;

/* ----------- FUNÇÕES ---------------- */

MatPtr geraMatPtr (int m, int n);
MatRow geraMatRow (int m, int n);
MatCol geraMatCol (int m, int n);
Vetor geraVetor (int n);

void liberaMatPtr (MatPtr mPtr, int n);
void liberaVetor (void *vet);


void multMatPtrVet (MatPtr mat, Vetor v, int m, int n, Vetor res);
void prnMatPtr (MatPtr mat, int m, int n);

void multMatRowVet (MatRow mat, Vetor v, int m, int n, Vetor res);
void prnMatRow (MatRow mat, int m, int n);

void multMatColVet (MatCol mat, Vetor v, int m, int n, Vetor res);
void prnMatCol (MatCol mat, int m, int n);

double prodEscalar (Vetor v1, Vetor v2, int n);
double normaMax (Vetor v1, Vetor v2, int n);
double normaEucl (Vetor vet, int n);
void prnVetor (Vetor vet, int n);

