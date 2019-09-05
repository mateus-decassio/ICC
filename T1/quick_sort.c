#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int cmpfunc (const void * a, const void * b) {
   //double i = (*(double*)a - *(double*)b);
   //return fabs(i);
   return (*(double*)a - *(double*)b);
}

int main () {
   unsigned int tamanho = 100;
   unsigned int cont = 0;
   double *values = malloc (tamanho * sizeof(double));
      
   double i = 0.9;
   while (cont < tamanho){
      values[cont] = i + 0.65;
      i = i + 0.89;
      cont ++;
   }

   //printf("Before sorting the list is: \n");
   //for( cont = 0 ; cont < tamanho; cont++ ) {
      //printf("%g ", values[cont]);
   //}

   qsort(values, tamanho, sizeof(double), cmpfunc);

   //printf("\nAfter sorting the list is: \n");
   for( cont = 0 ; cont < tamanho; cont++ ) {   
      printf("%.10e ", values[cont]);
   }
   printf("VETOR ORDENADO.\n");
  
   return(0);
}