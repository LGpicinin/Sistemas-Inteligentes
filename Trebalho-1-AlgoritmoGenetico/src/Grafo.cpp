#include "../include/Grafo.hpp"

// Aresta

Aresta::Aresta(int dest, int peso)
{
    this->idDestino = dest;
    this->peso = peso;
    this->next = nullptr;
}
Aresta::Aresta()
{
    delete this->next;
}

// Node

Node::Node(int id)
{
    this->setId(id);
    this->visitado = false;
    this->inicioArestas = nullptr;
}

Node::~Node()
{
    Aresta *arestaAtual = this->inicioArestas;
    Aresta *aux;

    while (arestaAtual != nullptr)
    {
        aux = arestaAtual->getNext();
        delete arestaAtual;
        arestaAtual = aux;
    }
}

// Grafo

Grafo::Grafo(int v)
{
    this->V = v;
    this->E = 0;

    int i;

    this->nodes = new Node *[v];
    for (i = 0; i < v; i++)
    {
        this->nodes[i] = new Node(i);
    }
}

Grafo::~Grafo()
{
    int i;
    Node *atual;
    Node *aux;
    for (i = 0; i < this->V; i++)
    {
        delete this->nodes[i];
    }
    delete this->nodes;
}

Grafo *Grafo::insereAresta(int v1, int v2, int peso)
{
    this->insereArestaDirecionada(v1, v2, peso);
    this->insereArestaDirecionada(v2, v1, peso);

    return this;
}

void Grafo::insereArestaDirecionada(int origem, int destino, int peso)
{
    if (origem > this->V)
    {
        return;
    }

    this->E++;
    Aresta *arestaAtual;

    if (!this->nodes[origem]->getInicioArestas())
    {
        arestaAtual = new Aresta(destino, peso);
        this->nodes[origem]->setInicioArestas(arestaAtual);
    }
    else
    {
        Aresta *aux = this->nodes[origem]->getInicioArestas();

        while (aux->getNext() != nullptr)
        {
            aux = aux->getNext();
        }

        aux->setNext(new Aresta(destino, peso));
    }
}

void Grafo::imprimeGrafo()
{
    int i;
    Aresta *aux;
    for (i = 0; i < this->V; i++)
    {
        std::cout << "\n"
                  << i << " --> ";

        if (this->nodes[i])
        {
            aux = this->nodes[i]->getInicioArestas();
            while (aux != nullptr)
            {
                std::cout << aux->getIdDestino() << "(peso: " << aux->getPeso() << "), ";
                aux = aux->getNext();
            }
        }
    }
}

void Grafo::desvisitaGrafo()
{
    int i;

    for (i = 0; i < this->V; i++)
    {
        this->nodes[i]->setVisitado(false);
    }
}