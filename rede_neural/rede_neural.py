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


# Convert string column to integer
def coluna_para_int(dataset, column):
    class_values = [row[column] for row in dataset]
    unique = set(class_values)
    lookup = {}
    for i, value in enumerate(unique):
        lookup[value] = i
    for row in dataset:
        row[column] = lookup[row[column]]
    return lookup


def inicializar_rede_neural(num_entradas, num_saidas):
    rede_neural = []
    camada_oculta = [
        {'weights':[
            rnd.random() 
            for i in range(num_entradas + 1)]} 
        for i in range(NUM_CAMADA_OCULTA)
        ]

    rede_neural.append(camada_oculta)
    camada_saida = [
        {'weights':[rnd.random() for i in range(NUM_CAMADA_OCULTA + 1)]} 
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
def feedForward(rede_neural, linha):
    lista_entradas = linha
    for camada in rede_neural:
        novas_entradas = []
        for neuronio in camada:
            ativacao = funcao_ativacao(neuronio['weights'], lista_entradas)
            neuronio['output'] = transferencia_sigmoide(ativacao)
            novas_entradas.append(neuronio['output'])
        lista_entradas = novas_entradas
    return lista_entradas


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
                    error += (neuronio_oculto['weights'][j] * neuronio_oculto['delta'])
                lista_erros.append(error)
        else: #Camada de Saída
            for j in range(len(camada)):
                neuronio_saida = camada[j]
                lista_erros.append(lista_valor_esperado[j] - neuronio_saida['output'])

        #Calculo de erro de cada Neuronio
        for j in range(len(camada)):
            neuronio = camada[j]
            neuronio['delta'] = lista_erros[j] * derivada_sigmoide(neuronio['output'])


# Update network weights with error
def atualizar_pesos(rede_neural, row, taxa_aprendizado):
    for i in range(len(rede_neural)):
        lista_entradas = row[:-1]
        if i != 0:
            lista_entradas = [neuron['output'] for neuron in rede_neural[i - 1]]

        for neuronio in rede_neural[i]:
            for j in range(len(lista_entradas)):
                neuronio['weights'][j] += taxa_aprendizado * neuronio['delta'] * lista_entradas[j]
            neuronio['weights'][-1] += taxa_aprendizado * neuronio['delta']


# Train a network for a fixed number of epochs
def treinamento_rede_neural(rede_neural, dados_treino, num_saidas):
    for epoca in range(NUM_EPOCAS):
        soma_erro = 0
        for linha in dados_treino:
            lista_saidas = feedForward(rede_neural, linha)
            lista_esperado = [0 for i in range(num_saidas)]
            lista_esperado[linha[-1]] = 1
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
    num_saidas = len({linha[-1] for linha in dados_treino})
    rede_neural = inicializar_rede_neural(num_entradas, num_saidas)
    treinamento_rede_neural(rede_neural, dados_treino, num_saidas)

    previsoes = []
    for linha in dados_teste:
        previsao = previsao_dados(rede_neural, linha)
        previsoes.append(previsao)

    return previsoes


# Find the min and max values for each column
def dados_min_max(dados):
    stats = [[min(coluna), max(coluna)] for coluna in zip(*dados)]
    return stats


# Rescale dataset columns to the range 0-1
def normalizar_dados(dataset, minmax):
    for row in dataset:
        for i in range(len(row)-1):
            row[i] = (row[i] - minmax[i][0]) / (minmax[i][1] - minmax[i][0])


# Split a dataset into k folds
def cross_validation_split(dataset):
    dataset_split = []
    dataset_copy = list(dataset)
    fold_size = int(len(dataset) / NUM_DIVISOES)
    for i in range(NUM_DIVISOES):
        fold = []
        while len(fold) < fold_size:
            index = rnd.randrange(len(dataset_copy))
            fold.append(dataset_copy.pop(index))
        dataset_split.append(fold)
    return dataset_split


# Calculate accuracy percentage
def medir_acuracia(lista_resultados, lista_previstos):
    dado_correto = 0
    for i in range(len(lista_resultados)):
        if lista_resultados[i] == lista_previstos[i]:
            dado_correto += 1
    return dado_correto / float(len(lista_resultados)) * 100.0


def main() -> None:
    rnd.seed(1)

    # load and prepare data
    dados = pd.read_csv('./rede_neural/treino_sinais_vitais_com_label.txt', encoding='utf-8')
    colunas = ['id','pSist','pDiast','qPA','bpM','respiração','gravidade','classe']
    dados.columns = colunas
    dados = dados.drop("id", axis=1)
    dados = dados.drop("pSist", axis=1)
    dados = dados.drop("pDiast", axis=1)
    dados = dados.drop("gravidade", axis=1)
    #dados = dados.values.tolist()




    dados_teste = dados.sample(frac=(1/3))
    dados_treino = dados.sample(frac=(2/3))
    dados = dados.values.tolist()

    # normalize input variables
    minmax = dados_min_max(dados)
    normalizar_dados(dados, minmax)
    coluna_para_int(dados, len(dados[0])-1)

    #Separação dos dados de teste e de treinamento
    lista_folds = cross_validation_split(dados)
    acertos = []
    for fold in lista_folds:
        dados_treino = list(lista_folds)
        dados_treino.remove(fold)
        dados_treino = sum(dados_treino, [])
        dados_teste = []
        for row in fold:
            row_copy = list(row)
            dados_teste.append(row_copy)
            row_copy[-1] = None

        #Inicio da rede neural
        dados_previstos = backPropagation(dados_treino, dados_teste)
        lista_resultados = [row[-1] for row in fold]
        accuracy = medir_acuracia(lista_resultados, dados_previstos)
        acertos.append(accuracy)

    print('Acertos: ', acertos)
    print('Acurácia: %.3f' %(sum(acertos)/float(len(acertos))))

if __name__ == "__main__":
    main()