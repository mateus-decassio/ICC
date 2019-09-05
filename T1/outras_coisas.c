//printf("The maximum value of float = %.10e\n", FLT_MAX);
   	//printf("The minimum value of float = %.10e\n", FLT_MIN);

   	//printf("The number of digits in the number = %.10e\n", FLT_MANT_DIG);
	printf("O VALOR DE DBL_MAX É:%.20e\n \n",DBL_MAX );
   	printf("O VALOR DE DBL_MAX DIVIDIVO POR DBL_EPSILON É:%.20e\n\n\n\n", DBL_MAX/DBL_EPSILON);

   	//printf("O VALOR DE DBL_EPSILON É: %.10e\n",DBL_EPSILON);
   	double x = 0.0000000000000000000000000000066723;
   	double y = 0.00000000000000877648;
   	printf("SUM x + y:%.40e\n", x + y);
   	
   	double diff = fabs(x-y);
   	printf("diff = %.20e\n", diff);
   	printf("diff/DBL_EPSILON É: %.20e\n", diff/DBL_EPSILON);


   	double g = 0.5;
   	double h = 0.05;
   	printf("SUM g + h:%.40e\n", g+h);

   	diff = fabs(g-h);
   	printf("diff = %.20e\n", diff);
   	printf("diff/DBL_EPSILON É: %.20e\n", diff/DBL_EPSILON);

   	double somatorio = 0.0;
   	printf("somatorio = %.20e\n", somatorio);




   	diff = fabs (DBL_MAX - DBL_MIN);
   	printf("A DIFERENÇA ENTRE OS EXTREMOS É DE:%.20e\n", diff);


* @title Implementação de Funções de Manipulação de raízes, polinômios e derivadas de Equações






int bisseccao (double (*f)(const double x), double a, double b,
               double eps, int *it, double *raiz)
{
   double xm_new, xm_old, fa, fb, ERr, verificador, fxm;

   fa = f(a);
   fb = f(b);
   ERr = eps;
   *it = 0;
   
   if ((fa*fb) < 0.0){
      xm_new = a + b / 2.0;
      while ((ERr <= eps) && (*it <= MAXIT)){
         xm_old = xm_new;
         fxm = f(xm_old);
         verificador = (fa * fxm);
         if (verificador < 0.0){
            b = xm_old;
         } else if (verificador > 0.0){
            a = xm_old;
            fa = fxm;
         } else{
            *raiz = xm;
            return 0; //achou a raiz
         }
         xm_new = a + b / 2.0;
         ERr = (fabs (xm_new - xm_old) / fabs (xm_new)) * 100;
         (*it)++;
      }
      
      *raiz = xm_new;   
   
   } else {
      fprintf(stderr, "Não foi possível calcular o valor da raiz dssa função nesse intervalo. Escolha um outro intervalo.\n" );
      *raiz = 0.0;
      return -1;
   }

   return 0; //caso obtenha "sucesso" ao encontrar a raiz
};