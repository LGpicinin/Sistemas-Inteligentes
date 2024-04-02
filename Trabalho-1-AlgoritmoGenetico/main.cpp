#include "./include/Grafo.hpp"
#include "./include/Agente.hpp"
#include <string.h>
#include <stdio.h>

int main()
{

    Grafo *ambiente = new Grafo(29);
    double tempo = 0.0;

    FILE *arq;
    char linha[120];
    char *peso;
    int i = 0;
    int j = 0;
    arq = fopen("bays29.txt", "r");
    if (arq == NULL)
    {
        printf("\nErro na leitura");
        return 0;
    }
    while (fgets(linha, 120, arq))
    {
        peso = strtok(linha, ",");
        while (peso)
        {
            if (atoi(peso) == 0)
            {
                j++;
                peso = strtok(NULL, ",");
                continue;
            }
            ambiente->insereArestaDirecionada(i, j, atoi(peso));
            j++;
            peso = strtok(NULL, ",");
        }
        j = 0;
        i++;
    }

    /*ambiente->insereAresta(0, 1, 23)->insereAresta(0, 2, 34)->insereAresta(0, 3, 18);
    ambiente->insereAresta(1, 2, 56)->insereAresta(1, 3, 28);
    ambiente->insereAresta(2, 3, 49);*/
    // ambiente->imprimeGrafo();

    Agente *agente = new Agente(ambiente);
    clock_t begin = clock();
    agente->calculaCaminho();
    clock_t end = clock();
    tempo = tempo + (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTempo execucao: %f segundos\n", tempo);
}