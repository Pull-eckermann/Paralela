#include <stdio.h>
#include <omp.h>
int fib(int n)
{
    printf("computing fib(%d)\n",n);
    int i, j;
    if (n<2)
        return n;
    else
    {
        #pragma omp task shared(i) firstprivate(n)
        i=fib(n-1);

        #pragma omp task shared(j) firstprivate(n)
        j=fib(n-2);

        #pragma omp taskwait
        return i+j;
    }
}

int main()
{
    int n = 5;

    omp_set_dynamic(0);
    omp_set_num_threads(4);

    #pragma omp parallel shared(n)
    {
        //Try removing the single directive below
        #pragma omp single
        printf ("fib(%d) = %d\n", n, fib(n));
    }
}
