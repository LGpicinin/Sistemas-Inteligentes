#include "../include/Agente.hpp"

#define NUM_ITERACOES 320

Agente::Agente(Grafo *a)
{
    this->quantVert = a->getVertices();
    int i, j;

    this->ambiente = a;
    this->caminhos = new int[this->quantVert];
    this->custoCaminho = 0;
    this->media = 0;
    this->maior = 0;
    this->menor = 0;

    for (i = 0; i < this->quantVert; i++)
    {
        this->caminhos[i] = -1;
    }
}

Agente::~Agente()
{
    delete caminhos;
    delete ambiente;
}

void Agente::resetarCaminho()
{
    int i;

    for (i = 0; i < this->quantVert; i++)
    {
        if (this->caminhos[i] == -1)
        {
            break;
        }
        this->caminhos[i] = -1;
    }
}

void Agente::imprimeCaminho()
{
    int i;

    std::cout << "\nCaminho: ";
    for (i = 0; i < this->quantVert; i++)
    {
        std::cout << this->caminhos[i] << " - ";
    }
    std::cout << "\nCusto: " << this->custoCaminho;
}

int Agente::sorteiaNumero(int limite)
{

    int sorteio = rand() % limite;

    return sorteio;
}

void Agente::atualizaCaminho(Aresta *a, Node *atual, Node *vizinho, int posicao)
{
    /*atual = vizinho;
    a = atual->getInicioArestas();
    ambiente->setVisitado(atual->getId(), true);
    caminhos[posicao] = atual->getId();

    vizinho = ambiente->getPosicao(a->getIdDestino());*/
}

bool Agente::temperaSimulada(int verticeId)
{

    int lim2, sorteiaProb, sorteiaVizinho;
    int i = 0;
    int j;
    int posicao = 0;
    float prob;
    float custo = 0;
    float media, delta, lim;

    ambiente->setVisitado(verticeId, true);
    caminhos[posicao] = verticeId;

    Node *nodeAtual = ambiente->getPosicao(verticeId);
    Aresta *aresta = nodeAtual->getInicioArestas();

    Node *vizinho = ambiente->getPosicao(aresta->getIdDestino());

    while (ambiente->getVertices() - 1 != posicao)
    {
        sorteiaVizinho = sorteiaNumero(quantVert - 1);
        for (j = 0; j < sorteiaVizinho && aresta->getNext() != nullptr; j++)
        {
            aresta = aresta->getNext();
        }
        vizinho = ambiente->getPosicao(aresta->getIdDestino());
        while (vizinho->getVisitado() == true)
        {
            if (aresta->getNext() == nullptr)
            {
                aresta = nodeAtual->getInicioArestas();
                vizinho = ambiente->getPosicao(aresta->getIdDestino());
                continue;
            }
            aresta = aresta->getNext();
            vizinho = ambiente->getPosicao(aresta->getIdDestino());
        }
        if (aresta == nullptr)
        {
            return false;
        }

        if (custo == 0 || aresta->getPeso() <= (int)custo / posicao)
        {
            custo = custo + aresta->getPeso();
            posicao++;
            nodeAtual = vizinho;
            aresta = nodeAtual->getInicioArestas();
            ambiente->setVisitado(nodeAtual->getId(), true);
            caminhos[posicao] = nodeAtual->getId();

            vizinho = ambiente->getPosicao(aresta->getIdDestino());
        }

        else
        {
            media = custo / posicao;
            delta = media - custo;
            prob = std::exp((float)delta / i);
            lim = prob * 1000000;
            lim2 = lim;
            sorteiaProb = sorteiaNumero(1000000);
            if (sorteiaProb <= lim2)
            {
                posicao++;
                custo = custo + aresta->getPeso();
                nodeAtual = vizinho;
                aresta = nodeAtual->getInicioArestas();
                ambiente->setVisitado(nodeAtual->getId(), true);
                caminhos[posicao] = nodeAtual->getId();

                vizinho = ambiente->getPosicao(aresta->getIdDestino());
            }
        }
        i++;
    }
    this->custoCaminho = custo;
    return true;
}

int *Agente::calculaCaminho()
{

    srand(time(NULL));

    for (int i = 0; i < NUM_ITERACOES; i++)
    {
        int sorteado = 0;
        int verticeId = sorteiaNumero(this->quantVert);
        temperaSimulada(verticeId);

        if (custoCaminho > maior || i == 0)
        {
            maior = custoCaminho;
        }
        if (custoCaminho < menor || i == 0)
        {
            menor = custoCaminho;
        }
        media = media + custoCaminho;

        imprimeCaminho();
        ambiente->desvisitaGrafo();
    }
    media = float(media / (NUM_ITERACOES));
    std::cout << "\nMaior valor: " << maior << "\nMenor valor: " << menor << "\nMedia: " << media;

    return caminhos;
}