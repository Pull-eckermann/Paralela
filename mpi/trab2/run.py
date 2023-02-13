import subprocess as subp
import numpy as np

#Copile the serial an paralell codes
subp.run(['make'], shell=True)

#===========================================================================#
#Calcula o tempo de execução serial
time_serial = list()
for i in range(0,20):
  time = subp.check_output(['./extract_serial entradas/exemplo1.txt 1000000 10000 | grep -i time'], shell=True, text=True)
  time = time.split('#')
  time_serial.append(float(time[1]))

media_serial = np.mean(time_serial)
desvioPadrao_serial = np.std(time_serial)
print("Processadores: 1")
print("Tempo:",media_serial)
print("Desvio Padrão:",desvioPadrao_serial)
print("")

#Cria lista de numero de processadores
procs = [2, 4, 8, 16]

#===========================================================================#
#   Teste de escalabilidade forte, O tamanho de entrada corresponde 
# a um em que a execução sequencial é maior que 10 segundos
time_t = list()
for p in procs:
  time_t.clear()
  for i in range(0,20):
    time = subp.check_output(['mpirun --host localhost:16 -np '+str(p)+' ./extract_paralelo entradas/exemplo1.txt 1000000 10000 | grep Time'], shell=True, text=True)
    time = time.split('#')
    time_t.append(float(time[1]))
  media = np.mean(time_t)
  desvioPadrao = np.std(time_t)
  print("Processadores:", p)
  print("Tempo:",media)
  print("Desvio Padrão:",desvioPadrao)
  print("")
#===========================================================================#
##Teste de escalabilidade fraca, variando a entrada com o tamanho da janela
#janela = [100, 1000, 10000, 100000]
#vetor = [10000, 100000, 1000000]



subp.run(['make clean'], shell=True)