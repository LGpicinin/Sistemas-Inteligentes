import pandas as pd
import numpy as np
import random as rnd
import math as mt
import csv as cs
from pandas.core.frame import DataFrame

NUM_DIVISOES = 3
TAXA_APRENDIZADO = 0.3
NUM_EPOCAS = 500
NUM_CAMADA_OCULTA = 5

class Neuronio:
     def __init__(self, funcao_ativacao, saida) -> None:
          self.peso = []
          self.entrada = []
          self.funcao_ativacao = funcao_ativacao
          self.saida = saida   


def inicializar_rede_neural(num_entradas, num_saidas):
     rede_neural = []
     camada_oculta = [
          {'pesos':[
                rnd.random() 
                for i in range(num_entradas + 1)]} 
          for i in range(NUM_CAMADA_OCULTA)
          ]

     rede_neural.append(camada_oculta)
     camada_saida = [
          {'pesos':[rnd.random() for i in range(NUM_CAMADA_OCULTA + 1)]} 
          for i in range(num_saidas)
          ]
     rede_neural.append(camada_saida)
     return rede_neural


# Calculate neuron activation for an input
# activation = sum(weight_i * input_i) + bias
def funcao_ativacao(lista_pesos, lista_entradas):
     ativacao = 0
     vies_neuronio = lista_pesos[-1]
     for i in range(len(lista_pesos)-1):
          ativacao += lista_pesos[i] * lista_entradas[i]
     ativacao += vies_neuronio
     return ativacao


# Transfer neuron activation
# output = 1 / (1 + e^(-activation))
def transferencia_sigmoide(ativacao):
     return 1.0 / (1.0 + mt.exp(-ativacao))


# Other transfer neuron activation
# output = se >= 0 retorna 1, senão 0
def transferencia_linear(ativacao):
     return int(ativacao>=0)


# Forward propagate input to a network output
def feedForward(rede_neural, row):
     inputs = row
     for camada in rede_neural:
          new_inputs = []
          for neuron in camada:
               activation = funcao_ativacao(neuron['pesos'], inputs)
               neuron['saida'] = transferencia_sigmoide(activation)
               new_inputs.append(neuron['saida'])
          inputs = new_inputs
     return inputs


# Calculate the derivative of an neuron output
# derivative = output * (1.0 - output)
def derivada_sigmoide(saida):
     return saida * (1.0 - saida)

# Calculate the derivative of an neuron output
# derivative = output * (1.0 - output)
def derivada_linear(saida):
     return 1


# Backpropagate error and store in neurons
# Saída: error = (expected - output) * transfer_derivative(output)
# Oculta: error = (weight_k * error_j) * transfer_derivative(output)
def backPropagation_Erro(rede_neural, lista_valor_esperado):
     lista_invertida_rede = reversed(range(len(rede_neural)))

     for i in lista_invertida_rede:
          lista_erros = []
          camada = rede_neural[i]
          if i != len(rede_neural)-1: #Camada Oculta
                for j in range(len(camada)):
                     error = 0.0
                     for neuronio_oculto in rede_neural[i + 1]:
                          error += (neuronio_oculto['pesos'][j] * neuronio_oculto['delta'])
                     lista_erros.append(error)
          else: #Camada de Saída
                for j in range(len(camada)):
                     neuronio_saida = camada[j]
                     lista_erros.append(lista_valor_esperado[j] - neuronio_saida['saida'])

          #Calculo de erro de cada Neuronio
          for j in range(len(camada)):
                neuronio = camada[j]
                neuronio['delta'] = lista_erros[j] * derivada_sigmoide(neuronio['saida'])


# Update network weights with error
def atualizar_pesos(rede_neural, row, taxa_aprendizado):
     for i in range(len(rede_neural)):
          lista_entradas = row[:-1]
          if i != 0:
                lista_entradas = [neuronio['saida'] for neuronio in rede_neural[i - 1]]

          for neuronio in rede_neural[i]:
                for j in range(len(lista_entradas)):
                     neuronio['pesos'][j] += taxa_aprendizado * neuronio['delta'] * lista_entradas[j]
                neuronio['pesos'][-1] += taxa_aprendizado * neuronio['delta']


# Train a network for a fixed number of epochs
def treinamento_rede_neural(rede_neural, dados_treino, num_saidas):
     for epoca in range(NUM_EPOCAS):
          soma_erro = 0
          for linha in dados_treino:
               lista_saidas = feedForward(rede_neural, linha)
               lista_esperado = [0 for i in range(num_saidas)]
               print(int(linha[-1]))
               print(lista_esperado)
               lista_esperado[int(linha[-1])] = 1
               soma_erro += sum([(lista_esperado[i] - lista_saidas[i])**2 for i in range(len(lista_esperado))])
               backPropagation_Erro(rede_neural, lista_esperado)
               atualizar_pesos(rede_neural, linha, TAXA_APRENDIZADO)
          print('>Epoca=%d, Taxa de Aprendizado=%.3f, Erro=%.3f' % (epoca, TAXA_APRENDIZADO, soma_erro))


# Make a prediction with a network
def previsao_dados(rede_neural, linha):
     saidas = feedForward(rede_neural, linha)
     return saidas.index(max(saidas))


# Backpropagation Algorithm With Stochastic Gradient Descent
def backPropagation(dados_treino, dados_teste):
     num_entradas = len(dados_treino[0]) - 1
     print(num_entradas)
     #num_saidas = len(set([linha[-1] for linha in dados_treino]))
     num_saidas = len({linha[-1] for linha in dados_treino})
     print(num_saidas)
     rede_neural = inicializar_rede_neural(num_entradas, num_saidas)
     treinamento_rede_neural(rede_neural, dados_treino, num_saidas)

     previsoes = []
     for linha in dados_teste:
          previsao = previsao_dados(rede_neural, linha)
          previsoes.append(previsao)

     return previsoes


# Calculate accuracy percentage
def medir_acuracia(lista_resultados, lista_previstos):
     dado_correto = 0
     for i in range(len(lista_resultados)):
          if lista_resultados[i] == lista_previstos[i]:
                dado_correto += 1
     return dado_correto / float(len(lista_resultados)) * 100.0


# Find the min and max values for each column
def dados_extremos(dados):
     minmax = [[min(column), max(column)] for column in zip(*dados)]
     return minmax


# Rescale dataset columns to the range 0-1
def normalizar_dados(dados):
     lista_min_max = dados_extremos(dados)
     for linha in dados:
          for i in range(len(linha)-1):
                linha[i] = (linha[i] - lista_min_max[i][0]) / (lista_min_max[i][1] - lista_min_max[i][0])
     return dados
           
# Split a dataset into k folds
def cross_validation_split(dataset, n_folds):
   dataset_split = []
   dataset_copy = list(dataset)
   fold_size = int(len(dataset) / n_folds)
   for i in range(n_folds):
      fold = []
      while len(fold) < fold_size:
         index = rnd.randrange(len(dataset_copy))
         fold.append(dataset_copy.pop(index))
      dataset_split.append(fold)
   return dataset_split

     
# Evaluate an algorithm using a cross validation split
def evaluate_algorithm(dataset, n_folds):
   folds = cross_validation_split(dataset, n_folds)
   scores = []
   for fold in folds:
      train_set = list(folds)
      train_set.remove(fold)
      train_set = sum(train_set, [])
      test_set = []
      for row in fold:
         row_copy = list(row)
         test_set.append(row_copy)
         row_copy[-1] = None
      predicted = backPropagation(train_set, test_set)
      actual = [row[-1] for row in fold]
      accuracy  = medir_acuracia(actual, predicted)
      scores.append(accuracy)
   return scores

def main() -> None:
     # load and prepare data
     rnd.seed(1)
     dados = pd.read_csv('./rede_neural/treino_sinais_vitais_com_label.txt', encoding='utf-8')

     colunas = ['id','pSist','pDiast','qPA','bpM','respiração','gravidade','classe']
     dados.columns = colunas
     dados = dados.drop("id", axis=1)
     dados = dados.drop("pSist", axis=1)
     dados = dados.drop("pDiast", axis=1)
     dados = dados.drop("gravidade", axis=1)

     dados_teste = dados.sample(frac=(1/3))
     dados_treino = dados.sample(frac=(2/3))
     dados = dados.values.tolist()
     #dados_treino = normalizar_dados(dados_treino.values.tolist())
     #dados_teste = normalizar_dados(dados_teste.values.tolist())

     normalizar_dados(dados)
     # evaluate algorithm
     n_folds = 3
     l_rate = 0.3
     n_epoch = 500
     n_hidden = 5
     scores = evaluate_algorithm(dados, n_folds)
     print('Scores: %s' % scores)
     print('Mean Accuracy: %.3f%%' % (sum(scores)/float(len(scores))))

if __name__ == "__main__":
     main()