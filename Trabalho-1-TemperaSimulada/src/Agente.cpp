#include "../include/Agente.hpp"

Agente::Agente(Grafo *a)
{
    this->quantVert = a->getVertices();
    int i, j;

    this->ambiente = a;
    this->caminhos = new int *[this->quantVert];
    this->custoCaminhos = new int[this->quantVert];

    for (i = 0; i < this->quantVert; i++)
    {
        this->custoCaminhos[i] = 0;
        this->caminhos[i] = new int[this->quantVert];
        for (j = 0; j < this->quantVert; j++)
        {
            this->caminhos[i][j] = -1;
        }
    }
}

Agente::~Agente()
{
    int i;

    for (i = 0; i < this->quantVert; i++)
    {
        delete this->caminhos[i];
    }

    delete caminhos;
    delete ambiente;
    delete custoCaminhos;
}

void Agente::resetarCaminho(int caminho)
{
    int i;

    for (i = 0; i < this->quantVert; i++)
    {
        if (this->caminhos[caminho][i] == -1)
        {
            break;
        }
        this->caminhos[caminho][i] = -1;
    }
}

void Agente::imprimeCaminhos()
{
    int i, j;

    for (i = 0; i < this->quantVert; i++)
    {
        std::cout << "\nCaminho " << i << ": ";
        for (j = 0; j < this->quantVert - 1; j++)
        {
            std::cout << this->caminhos[i][j] << " - ";
        }
        std::cout << "\nCusto: " << this->custoCaminhos[i];
    }
}

int Agente::sorteiaNumero(int limite)
{
    srand(time(NULL));

    return rand() % limite;
}

bool Agente::temperaSimulada(int verticeId, int iteracao)
{
    this->ambiente->setVisitado(verticeId, true);
    this->caminhos[iteracao][0] = verticeId;

    int i;

    Node *vizinho = this->ambiente->getPosicao(verticeId);
    int custo = vizinho->getPeso();

    for (i = 0; i < this->ambiente->getVertices(); i++)
    {
        while (vizinho->getVisitado() == true && vizinho != nullptr)
        {
            vizinho = vizinho->getNext();
        }
        if (vizinho == nullptr)
        {
            return false;
        }

        if (vizinho->getPeso() <= custo / (i + 1))
        {
        }
    }
}

int *Agente::calculaCaminhos()
{
    int i;
    int verticeId = 0;
    bool completouCaminho;
    int melhorCaminho = 0;

    for (i = 0; i < this->quantVert; i++)
    {
        verticeId = sorteiaNumero(this->quantVert);
        completouCaminho = temperaSimulada(verticeId, i);
        if (completouCaminho == false)
        {
            i--;
        }
        else
        {
            if (this->custoCaminhos[melhorCaminho] >= this->custoCaminhos[i])
            {
                melhorCaminho = i;
            }
        }
    }

    imprimeCaminhos();
}