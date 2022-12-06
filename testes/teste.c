#include <stdio.h>
#include <omp.h>

int main(){
	int ID = 0;

	#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		printf("Hello(%d)", ID);
		printf("World(%d)\n", ID);
	}
}

