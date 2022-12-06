#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 1

static long num_steps = 100000;
double step;

int main () {
  int i; 
	double pi = 0.0;
  step = 1.0/(double) num_steps;

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int i, id, nthrds;
		double x, sum = 0.0;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
  	for (i=id, sum = 0.0; i < num_steps; i = i + nthrds){
  	  x = (i + 0.5) * step; // Largura do retângulo
  	  sum = sum + 4.0 / (1.0 + x*x); // Sum += Área do retângulo
  	}
		#pragma omp critical
  		pi = step * sum;
	}

  printf("PI: %f\n", pi*4);
} 

