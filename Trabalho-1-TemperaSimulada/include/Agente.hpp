#pragma once
#include "Grafo.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Agente
{
private:
    Grafo *ambiente;
    int **caminhos;
    int *custoCaminhos;
    int quantVert;

public:
    Agente(Grafo *a);
    ~Agente();
    void imprimeCaminhos();
    int sorteiaNumero(int limite);
    int *calculaCaminhos();
    bool temperaSimulada(int verticeId, int iteracao);
    void resetarCaminho(int caminho);
};