#pragma once

class Node
{
private:
    int id, peso;
    Node *next;

public:
    Node(int id, int peso);
    ~Node();
    int getId()
    {
        return this->id;
    }
    void setId(int i)
    {
        this->id = i;
    }
    int getPeso()
    {
        return this->peso;
    }
    void setPeso(int p)
    {
        this->peso = p;
    }
    Node *getNext()
    {
        return this->next;
    }
    void setNext(Node *n)
    {
        this->next = n;
    }
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
    int getVertices()
    {
        return this->V;
    }
    int getArestas()
    {
        return this->E;
    }
    Node *getPosicao(int p)
    {
        return this->nodes[p];
    }
};
