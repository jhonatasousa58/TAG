#ifndef GRAFO_H
#define GRAFO_H
#include <bits/stdc++.h>
using namespace std;


//Classes do grafo e de disciplinas e lista de adjacencias
class Disciplinas{

public:
	
	vector<int> adj;
	int codigo;
	int peso;
	int grau;

};

class Grafo{

    int materia;

public:

    Grafo(int V);
	vector<Disciplinas> curso;
	void montaGrafo();
	void printGrafo(Grafo *grafo);
	void grauV();
    vector<vector<int>> cpm(Grafo *grafo);
};

//Classe que cria e armazena uma ordenacao topologica do grafo das disciplinas
class OrdenacaoTopologica{

	vector<Disciplinas> grafoOrdenado;

public:

	void sortTopological(Grafo *grafo);

};

//Assinatura das funcoes utilizadas neste projeto
void printV(vector<int> v);

#endif