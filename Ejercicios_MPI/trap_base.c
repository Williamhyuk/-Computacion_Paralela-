
#include <stdio.h>
#include <mpi.h>


/* Función que estamos integrando*/
double f(double x) {
   return x*x;
} /* f */


/* Calcular integral local */
double Trap(
      double a  /* in */, //left_endpt
      double b /* in */, //right_endpt
      int    n  /* in */, //trap_count 
      double h    /* in */) //base_len 

{
   double approx, x; 
   int i;

   approx = (f(a) + f(b))/2.0;
   for (i = 1; i <= n-1; i++) {
      x = a + i*h;
      approx += f(x);
   }
   approx= approx*h;

   return approx;
} /*  Trap  */



int main(void) {
   int my_rank, comm_sz, n = 1024, local_n;   
   double a = 0.0, b = 3.0, h, local_a, local_b;
   double local_int, total_int;
   int source; 

   /* se inicia MPI */
   MPI_Init(NULL, NULL);

   /* Obtener mi rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* saber cuántos procesos se están utilizando */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   h = (b-a)/n;          /* h es igual para todos los procesos */
   local_n = n/comm_sz;  /*  es el número de trapezoides  */

   /* Tamaño del intervalo de cada proceso de 
   * integración = local_n 
   * h. Entonces mi intervalo * comienza en: */
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   /* Suma las integrales calculadas por cada proceso */
   if (my_rank != 0) { 
      MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, 
            MPI_COMM_WORLD); 
   } else {/*my_rank==0*/
      total_int = local_int;
      for (source = 1; source < comm_sz; source++) {
         MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         total_int += local_int;
      }
   } 

   if (my_rank == 0) {
      printf("En n = %d trapezoides, el aproximado\n", n);
      printf("de la integral de %f para %f = %.15e\n",
          a, b, total_int);
   }

   MPI_Finalize();

   return 0;
} /*  main  */


