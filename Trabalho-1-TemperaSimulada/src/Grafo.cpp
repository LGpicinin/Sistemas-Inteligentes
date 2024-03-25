#include "../include/Grafo.hpp"
#include <iostream>

Node::Node(int id, int peso)
{
    this->setId(id);
    this->setPeso(peso);
    this->next = nullptr;
}

Node::~Node()
{
    if (this->next)
    {
        delete this->next;
    }
}

Grafo::Grafo(int v)
{
    this->V = v;
    this->E = 0;

    int i;

    this->nodes = new Node *[v];
    for (i = 0; i < v; i++)
    {
        this->nodes[i] = nullptr;
    }
}

Grafo::~Grafo()
{
    int i;
    Node *atual;
    Node *aux;
    for (i = 0; i < this->V; i++)
    {
        atual = this->nodes[i];
        while (atual)
        {
            aux = atual->getNext();
            delete atual;
            atual = aux;
        }
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
    if (!this->nodes[origem])
    {
        this->nodes[origem] = new Node(destino, peso);
    }
    else
    {
        Node *aux = this->nodes[origem];

        while (aux->getNext() != nullptr)
        {
            aux = aux->getNext();
        }

        aux->setNext(new Node(destino, peso));
    }
}

void Grafo::imprimeGrafo()
{
    int i;
    Node *aux;
    for (i = 0; i < this->V; i++)
    {
        std::cout << "\n"
                  << i << " --> ";

        if (this->nodes[i])
        {
            aux = this->nodes[i];
            while (aux != nullptr)
            {
                std::cout << aux->getId() << "(peso: " << aux->getPeso() << "), ";
                aux = aux->getNext();
            }
        }
    }
}