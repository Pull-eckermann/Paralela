import subprocess as subp



#Copile the serial an paralell codes
subp.run(['make'], shell=True)

##Calcula o tempo de execução serial
#time_serial = 0.0
#for i in range(0,20):
#  time = subp.check_output(['./extract_serial entradas/exemplo3.txt 1000000 10000 | grep -i Time'], shell=True, text=True)
#  time = time.split(' ')
#  time_serial += float(time[1])
#time_serial = time_serial / 20.0
#print("Processadores: 1")
#print("Tempo:",time_serial)

#Cria lista de numero de processadores
procs = [4, 8, 16, 32]

#===========================================================================#
#   Teste de escalabilidade forte, O tamanho de entrada corresponde 
# a um em que a execução sequencial é maior que 1- segundos
for p in procs:
  time_t = 0.0
  for i in range(0,20):
    time = subp.check_output(['mpirun --host localhost:32 -np '+str(p)+' ./extract_paralelo entradas/exemplo3.txt 1000000 10000 | grep Time'], shell=True, text=True)
    time = time.split(' ')
    time_t += float(time[1])
  time_t = time_t / 20.0
  print("Processadores:",p)
  print("Tempo:",time_t) 

subp.run(['make clean'], shell=True)