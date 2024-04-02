#pragma once
#include "Grafo.hpp"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

class Agente
{
private:
    Grafo *ambiente;
    int **populacao;
    int *custosPopulacao;
    float *probs;
    float *intervaloProbs;
    int somaCusto;
    int quantVert;
    float media;
    int menorCusto;
    int maiorCusto;
    int quantPopulacao;
    vector<bool> visitado;

public:
    Agente(Grafo *a);
    ~Agente();
    void imprimePopulacoes();
    int sorteiaNumero(int limite);
    void calculaCaminho();
    void algoritmoGenetico();
    void resetarCaminho();
    void geraPrimeiraPopulacao(int k);
    int *reproduz(int x, int y);
    int *mutacao(int *inividuo);
    int fitness();
    void calculaCusto(int x);
    void calculaProbs();
    void desvisita();
};