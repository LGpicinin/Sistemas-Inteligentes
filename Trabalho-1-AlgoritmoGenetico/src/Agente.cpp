#include "../include/Agente.hpp"

Agente::Agente(Grafo *a)
{
    this->quantPopulacao = 10;
    this->quantVert = a->getVertices();
    this->ambiente = a;
    this->populacao = new int *[this->quantPopulacao];
    this->custosPopulacao = new int[this->quantPopulacao];
    this->probs = new float[this->quantPopulacao];
    this->media = 0;
    this->maior = 0;
    this->menor = 0;
    this->somaCusto = 0;

    int i, j;

    for (i = 0; i < this->quantPopulacao; i++)
    {
        this->custosPopulacao[i] = 0;
        this->populacao[i] = new int[this->quantVert];
        for (j = 0; j < this->quantVert; j++)
        {
            if (i == 0)
            {
                this->visitado.push_back(false);
            }
            this->populacao[i][j] = -1;
        }
    }
}

Agente::~Agente()
{
    int i;

    for (i = 0; i < this->quantVert; i++)
    {
        delete this->populacao[i];
    }

    delete populacao;
    delete ambiente;
    delete custosPopulacao;
}

void Agente::imprimePopulacoes()
{
    int i, j;

    for (i = 0; i < quantPopulacao; i++)
    {
        cout << "\nPopulacao: ";
        for (j = 0; j < this->quantVert; j++)
        {
            std::cout << this->populacao[i][j] << " - ";
        }
        cout << "\nCusto: " << this->custosPopulacao[i];
    }
    cout << "\n";
}

int Agente::sorteiaNumero(int limite)
{

    int sorteio = rand() % limite;

    return sorteio;
}

void Agente::desvisita()
{

    int j;
    for (j = 0; j < quantVert; j++)
    {
        visitado[j] = false;
    }
}

void Agente::calculaProbs()
{
    int i;
    float somaProbs;

    for (i = 0; i < quantPopulacao; i++)
    {
        probs[i] = float(float(somaCusto - custosPopulacao[i]) / float(somaCusto));
    }
    for (i = 0; i < quantPopulacao; i++)
    {
        somaProbs = somaProbs + probs[i];
    }
    for (i = 0; i < quantPopulacao; i++)
    {
        probs[i] = probs[i] / somaProbs;
        // cout << "\nProbabilidade do individuo " << i << " eh " << probs[i];
    }
}

void Agente::calculaCusto(int x)
{
    int i;
    int custo = 0;
    Node *atual;
    Aresta *aresta;
    for (i = 0; i < quantVert - 1; i++)
    {
        atual = ambiente->getPosicao(populacao[x][i]);
        aresta = atual->getInicioArestas();
        while (aresta->getIdDestino() != populacao[x][i + 1])
        {
            aresta = aresta->getNext();
        }
        custo = custo + aresta->getPeso();
    }
    custosPopulacao[x] = custo;
}

void Agente::geraPrimeiraPopulacao(int k)
{
    int i, j;
    int primeiroVertice, vizinhoPosicao;
    int posicao;
    int custo;
    Node *atual;
    Node *vizinho;
    Aresta *aresta;

    for (i = 0; i < k; i++)
    {
        custo = 0;
        primeiroVertice = sorteiaNumero(this->quantVert);
        visitado[primeiroVertice] = true;
        atual = ambiente->getPosicao(primeiroVertice);
        populacao[i][0] = primeiroVertice;

        for (posicao = 1; posicao != this->quantVert; posicao++)
        {
            vizinhoPosicao = sorteiaNumero(this->quantVert);
            while (visitado[vizinhoPosicao] == true)
            {
                vizinhoPosicao++;
                if (vizinhoPosicao == this->quantVert)
                {
                    vizinhoPosicao = 0;
                }
            }
            visitado[vizinhoPosicao] = true;
            aresta = atual->getInicioArestas();
            while (aresta->getIdDestino() != vizinhoPosicao)
            {
                aresta = aresta->getNext();
            }
            custo = custo + aresta->getPeso();
            vizinho = ambiente->getPosicao(aresta->getIdDestino());
            atual = vizinho;
            populacao[i][posicao] = atual->getId();
        }
        custosPopulacao[i] = custo;
        somaCusto = somaCusto + custo;
        desvisita();
    }
    calculaProbs();
}

int *Agente::mutacao(int *individuo)
{
    int aux;
    int x = sorteiaNumero(quantVert);
    int y = sorteiaNumero(quantVert);

    while (x == y)
    {
        y = sorteiaNumero(quantVert);
    }

    aux = individuo[x];
    individuo[x] = individuo[y];
    individuo[y] = aux;

    return individuo;
}

int *Agente::reproduz(int x, int y)
{
    int i, j;
    int *novoIndividuo = new int[quantVert];

    for (i = 0; i < quantVert / 2; i++)
    {
        novoIndividuo[i] = populacao[x][i];
        visitado[novoIndividuo[i]] = true;
    }

    for (j = 0; i < quantVert; j++)
    {
        if (visitado[populacao[y][j]] == false)
        {
            novoIndividuo[i] = populacao[y][j];
            visitado[novoIndividuo[i]] = true;
            i++;
        }
    }
    desvisita();

    return novoIndividuo;
}

int Agente::fitness()
{
    int i = 0;
    float prob = probs[0];
    float sorteio = (float)rand() / RAND_MAX;

    while (sorteio > prob && i < quantPopulacao - 1)
    {
        i++;
        prob = prob + probs[i];
    }

    return i;
}

void Agente::algoritmoGenetico()
{
    int i, j, x, y, sorteio;
    int *individuo;
    int **novaPopulacao = new int *[quantPopulacao];

    for (i = 0; i < 1000; i++)
    {
        for (j = 0; j < quantPopulacao; j++)
        {
            x = fitness();
            y = fitness();
            while (x == y)
            {
                y = fitness();
            }
            individuo = reproduz(x, y);
            sorteio = sorteiaNumero(100);
            if (sorteio <= 10)
            {
                individuo = mutacao(individuo);
            }
            novaPopulacao[j] = individuo;
        }
        populacao = novaPopulacao;
        somaCusto = 0;
        for (j = 0; j < quantPopulacao; j++)
        {
            calculaCusto(j);
            somaCusto = somaCusto + custosPopulacao[j];
        }
        calculaProbs();
    }
}

void Agente::calculaCaminho()
{
    srand(time(NULL));
    geraPrimeiraPopulacao(quantPopulacao);
    imprimePopulacoes();
    algoritmoGenetico();
    imprimePopulacoes();
}