import subprocess as subp
import numpy as np

#Copile the serial an paralell codes
subp.run(['make'], shell=True)

#===========================================================================#
#Calcula o tempo de execução serial
print("==========Tempo de execução sequencial==========")
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
print("==========Teste de escalabilidade forte==========")
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
print("==========Teste de escalabilidade fraca==========")
janela = [100, 1000, 10000, 100000] #Lista para os testes que contem tamanhos de janelas
vetor = [1000000] #Lista para os testes que contem tamanhos de vetor
time_t = list()
procs = [2, 4, 8, 16]

for v in vetor:
  for j in janela:
    print('### VETOR de',v,'posiçoes e JANELA de tamanho', j)
    time_t.clear()
    for i in range(0,2):
      time = subp.check_output(['./extract_serial entradas/exemplo1.txt '+str(v)+' '+str(j)+' | grep -i time'], shell=True, text=True)
      time = time.split('#')
      time_t.append(float(time[1]))
    media_serial = np.mean(time_t)
    desvioPadrao_serial = np.std(time_t)
    print("Processadores: 1")
    print(f'Tempo: {media_serial:,.4f}')
    print(f'Desvio Padrão: {desvioPadrao_serial:,.4f}')
    print("")

    for p in procs:
      time_t.clear()
      for i in range(0,2):
        time = subp.check_output(['mpirun --host localhost:16 -np '+str(p)+' ./extract_paralelo entradas/exemplo1.txt '+str(v)+' '+str(j)+' | grep -i time'], shell=True, text=True)
        time = time.split('#')
        time_t.append(float(time[1]))
      media = np.mean(time_t)
      desvioPadrao = np.std(time_t)
      print("Processadores:", p)
      speed = media_serial / media
      print(f'Speedup: {speed :,.4f}')
      print(f'Desvio Padrão: {desvioPadrao:,.4f}')
      print("")



subp.run(['make clean'], shell=True)