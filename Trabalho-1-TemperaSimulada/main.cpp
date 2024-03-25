#include "./include/Grafo.hpp"

int main()
{
    Grafo *ambiente = new Grafo(4);
    ambiente->insereAresta(0, 1, 23)->insereAresta(0, 2, 34)->insereAresta(0, 3, 18);
    ambiente->insereAresta(1, 2, 56)->insereAresta(1, 3, 28);
    ambiente->insereAresta(2, 3, 49);

    ambiente->imprimeGrafo();
}