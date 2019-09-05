#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
int main (){
	



	double x = 0.0001;
	double y = 0.0;
	double soma = 0.0;
   	double contador = 0.0;


   	double diff = fabs(x-y);
   	printf("diff = %.20e\n", diff);
   	printf("diff/DBL_EPSILON É: %.20e\n", diff/DBL_EPSILON);

   	return 0;
}	
