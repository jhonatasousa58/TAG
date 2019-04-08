#ifndef GRAFO_H
#define GRAFO_H
#include <bits/stdc++.h>
using namespace std;

//Classe do grafo e lista de adjacencias
class Grafo {
	int V;

public:
	
    vector<int> *adj;
	Grafo(int);
    void adcAresta(int, int);    
};

//Assinatura das funcoes utilizadas neste projeto
Grafo montaGrafo();
void printaGrafo(Grafo k);
vector<int> Uniao(vector<int> v1, vector<int> v2);
vector<int> Interseccao(vector<int> v1, vector<int> v2);
vector<int> Diferenca(vector<int> v1, vector<int> v2);
vector<int> Vizinhos(int v, Grafo k);
void BronK(vector<int> R, vector<int> P, vector<int> X, Grafo k);
void printaMaxl();
bool taDentro(vector<int> l, int v);
vector<float> coeAglo(Grafo k);


#endif
