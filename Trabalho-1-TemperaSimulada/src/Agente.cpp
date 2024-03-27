#include "../include/Agente.hpp"

Agente::Agente(Grafo *a)
{
    this->quantVert = a->getVertices();
    int i, j;

    this->ambiente = a;
    this->caminhos = new int[this->quantVert];
    this->custoCaminho = 0;

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

void Agente::resetarCaminho(int caminho)
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

    printf("\n %d", sorteio);

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

    int lim2, sorteado;
    int i = 0;
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
        while (vizinho->getVisitado() == true && aresta != nullptr)
        {
            aresta = aresta->getNext();
            vizinho = ambiente->getPosicao(aresta->getIdDestino());
        }
        if (aresta == nullptr)
        {
            // std::cout << "Aqui?\n";
            return false;
        }

        if (custo == 0)
        {
            custo = custo + aresta->getPeso();
            posicao++;
            nodeAtual = vizinho;
            aresta = nodeAtual->getInicioArestas();
            ambiente->setVisitado(nodeAtual->getId(), true);
            caminhos[posicao] = nodeAtual->getId();

            vizinho = ambiente->getPosicao(aresta->getIdDestino());
            // atualizaCaminho(aresta, nodeAtual, vizinho, posicao);
            //  std::cout << "Entrei\n";
        }
        else if (aresta->getPeso() <= (int)custo / posicao)
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
            std::cout << "\nDelta: " << delta;
            std::cout << "\ni: " << i;
            prob = std::exp((float)delta / i);
            std::cout << "\nProbabilidade: " << prob;
            lim = prob * 1000000;
            lim2 = lim;
            sorteado = sorteiaNumero(1000000);
            if (sorteado <= lim2)
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

    int sorteado = 0;
    int verticeId = sorteiaNumero(this->quantVert);

    temperaSimulada(verticeId);

    /*while (temperaSimulada(verticeId) == false)
    {

        sorteado = sorteiaNumero(this->quantVert);
        while (sorteado == verticeId)
        {
            sorteado = sorteiaNumero(this->quantVert);
        }
        verticeId = sorteado;
    }*/

    imprimeCaminho();

    return caminhos;
}