all: paralelo serial

paralelo: passeio_cavalo_paralelo.c
	gcc -fopenmp -O3 -o cavalo_paralelo passeio_cavalo_paralelo.c

serial: passeio_cavalo.c
	gcc -O3 -o cavalo_serial passeio_cavalo.c

clean: 
	rm -rf cavalo_paralelo cavalo_serial