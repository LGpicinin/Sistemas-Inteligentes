#pragma once
#include <iostream>

class Aresta
{
private:
    int idDestino;
    int peso;
    Aresta *next;

public:
    Aresta(int dest, int peso);
    Aresta();

    int getPeso() { return this->peso; }

    int getIdDestino() { return this->idDestino; }

    Aresta *getNext() { return this->next; }
    void setNext(Aresta *aresta) { this->next = aresta; }
};

class Node
{
private:
    int id;
    bool visitado;
    Aresta *inicioArestas;

public:
    Node(int id);
    ~Node();

    int getId() { return this->id; }
    void setId(int i) { this->id = i; }

    Aresta *getInicioArestas() { return this->inicioArestas; }
    void setInicioArestas(Aresta *a) { this->inicioArestas = a; }

    bool getVisitado() { return this->visitado; }
    void setVisitado(bool b) { this->visitado = b; }
};

class Grafo
{
private:
    Node **nodes;
    int V, E;

public:
    Grafo(int v);
    ~Grafo();

    Grafo *insereAresta(int v1, int v2, int peso);
    void insereArestaDirecionada(int origem, int destino, int peso);
    void imprimeGrafo();
    void desvisitaGrafo();

    int getVertices() { return this->V; }
    int getArestas() { return this->E; }

    Node *getPosicao(int p) { return this->nodes[p]; }

    bool getVisitado(int p) { return this->nodes[p]->getVisitado(); }
    void setVisitado(int p, bool b) { this->nodes[p]->setVisitado(b); }
};
