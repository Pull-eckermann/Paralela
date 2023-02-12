all: paralelo serial

paralelo: extract_paralelo.c
	mpicc extract_paralelo.c -O3 -o extract_paralelo

serial: extract_serial.c
	gcc extract_serial.c -O3 -o extract_serial

clean: 
	rm -rf extract_paralelo	 extract_serial