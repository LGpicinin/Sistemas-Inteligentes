#pragma once
#include "Grafo.hpp"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>

class Agente
{
private:
    Grafo *ambiente;
    int *caminhos;
    int custoCaminho;
    int quantVert;
    float media;
    int menor;
    int maior;

public:
    Agente(Grafo *a);
    ~Agente();
    void imprimeCaminho();
    int sorteiaNumero(int limite);
    int *calculaCaminho();
    bool temperaSimulada(int verticeId);
    void resetarCaminho();
    void atualizaCaminho(Aresta *a, Node *atual, Node *vizinho, int posicao);
};