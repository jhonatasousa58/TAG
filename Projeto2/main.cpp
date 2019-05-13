/* 
    Universidade de Brasilia
    Departamento de Ciencia da Computacao
    Projeto 2 - Ordenacao topoliga e Caminhos Criticos no grafo de disciplinas do curso de
    Bacharelado em Ciencia da Computaçao da UnB
    Teoria e Aplicacao de Grafos, Turma A, 1/2019
    Prof. Díbio

   Autores: Adelson Jhonata Silva de Sousa - 18/0114913
            Heitor de Lima - 16/0123950
*/

#include <bits/stdc++.h>
#include "grafo.h"

using namespace std;

// Cria e aloca a memoria do grafo.
Grafo::Grafo(int qtdDis){
	this->materia = qtdDis;
}

//Le o arquivo txt e pegando dados linha por linha e monta o grafo das disciplinas
void Grafo::montaGrafo(){
	ifstream file ("disciplinas.txt");
	int i, p;
	Disciplinas mat;
	string aux, num;

	if (file.is_open()){
			string line;
		while (! file.eof() ){
				getline (file,line);
				i=0;
				if((line[0]!='\0')){
					while(line[i+1]!='|'){
						aux+=line[i];
						i++;
					}
					mat.codigo = stoi(aux);
					aux="";
					i=i+4;
					while(line[i]!=')'){
						if(line[i]==','){
							mat.adj.push_back(stoi(aux));
							aux="";
							i=i+2;
						}
						aux+=line[i];
						i++;
					}
					if(aux.size()>0)
						mat.adj.push_back(stoi(aux));
					while(line[i]!='\0'){
						if(line[i]=='|'){
							num=line[i+2];
							p = stoi(num);
						}
						i++;
					}
					mat.peso = p;
					mat.grau=0;
					curso.push_back(mat);
					mat.adj.clear();
					aux="";
				}
		}
		file.close();
	}

	else{
		cout << "impossivel ler o arquivo.";
	}
}

//mostra o grafo no terminal
void Grafo::printGrafo(Grafo *grafo){
	for (Disciplinas mt : grafo->curso) {
        cout << "Codigo: " << mt.codigo;
        cout << " | Peso: " << mt.peso << endl;
        cout << "Eh pre-requisito para: ";
        if(mt.adj.size() > 0){
	        for(int aux : mt.adj){
	            cout << aux;
	            if(aux != mt.adj.back())
	             cout << ", ";
	        }
        }else{
        	cout << "Nenhuma materia!";
        }
        cout << endl;
        cout << endl;
    }
}

//funcao que ordena topologicamente o grafo de discpilinas e mostra no terminal a ordenacao
void OrdenacaoTopologica::sortTopological(Grafo *grafo){

	int n=34, i = 0;

	//pega o grau de cada vertice
	for(Disciplinas mt : grafo->curso){
		for(Disciplinas mt1 : grafo->curso){
			for(int adja : mt1.adj){
				if(mt.codigo == adja){
					grafo->curso[i].grau++;
				}
			}
		}
		i++;
	}

	//alimenta o vetor para ordencao de acordo com grau
    for(i = 0; i < n; i++){
    	if(grafo->curso[i].grau == 0){
			grafoOrdenado.push_back(grafo->curso[i]);
		}
	}


	//ordena de acordo com o grau do vertice
    int j = 0;
    while(j < (int)grafoOrdenado.size()){
        Disciplinas mtatual = grafoOrdenado[j];
        j++;

		for(int mt : mtatual.adj){
			i=0;
			while(i<n){
				if(mt == grafo->curso[i].codigo){
					grafo->curso[i].grau--;
					if(grafo->curso[i].grau == 0)
						grafoOrdenado.push_back(grafo->curso[i]);
					break;
				}
				i++;
			}
		}
    }

    //mostra no terminal a ordenacao
	cout << "Ordenacao Topologica do grafo: " << endl;
    for(int i = 0;i < (int)grafoOrdenado.size();i++){
		cout << grafoOrdenado[i].codigo << endl;
	}

}

//variaveis globais para o funcionamento das funcoes de busca de caminhos criticos
int tmp = 0;
vector<vector<int>> paths;
vector<int> visits;
vector<int> pathat;


//verifica se o vertice ja foi visitado
bool visit(vector<int> visits, int codigomt) {
	vector<int>::iterator it;
	for(it = visits.begin(); it  != visits.end(); ++it) {
		if(codigomt == *it) {
			return true;
		}
	}
	return false;
}

//procura e marca os vertices adjacentes
vector<int> Adjacentes(int codigomt, Grafo *grafo){
	vector<Disciplinas>::iterator it;
	for(it = grafo->curso.begin(); it != grafo->curso.end(); ++it) {
		if (it->codigo == codigomt) {
			return it->adj;
		}
	}
	return {};
}

//encontra os caminhos no grafo
void PossiveisCaminhos(int mtb, Grafo *grafo) {
	vector<int> adjacentes = Adjacentes(mtb, grafo);	// pega as materias adjacentes dessa materia
	vector<int>::iterator it;

	pathat.push_back(mtb);

	if(adjacentes.size() == 0) {
		paths.push_back(pathat);
		return;
	}
	for(it = adjacentes.begin(); it != adjacentes.end(); ++it) {
		if(!visit(visits, *it)) {
			PossiveisCaminhos(*it, grafo);
		}
		pathat.pop_back();
	}
}

//calcula o valor total dos caminhos do grafo e auxilia na funcao de caminho critico
int tamanhoCaminho(vector<int> v, Grafo *grafo) {
	vector<Disciplinas>::iterator it;
	int pesoe = 0;
	for(it = grafo->curso.begin(); it != grafo->curso.end(); ++it) {
		if(visit(v, it->codigo)) {
			pesoe += it->peso;
		}
	}
	return pesoe;
}

//funcao que procura o caminho mais longo no grafo(caminho critico) de acordo com o peso total dos possiveis caminhos
vector<vector<int>> Grafo::cpm(Grafo *grafo) {

	vector<int> mtSPre;
	vector<Disciplinas>::iterator itsp;
	for(itsp = grafo->curso.begin(); itsp != grafo->curso.end(); ++itsp) {
		if(itsp->grau == 0) {
			mtSPre.push_back(itsp->codigo);
		}
	}
	
	vector<int> mtSPreVis;
	vector<int>::iterator it;
	vector<vector<int>> caminhoCritico;
	vector<vector<int>>::iterator cp;
	int maiorC = 0;
	int maiorP = 0;
	int pesoA = 0;

	// adiciona todos os caminhos a 'caminhos'
	for (it = mtSPre.begin(); it != mtSPre.end(); ++it) {
		if(!visit(mtSPreVis, *it)){
			PossiveisCaminhos(*it, grafo);
		}
		mtSPreVis.push_back(*it);
		pathat.erase(pathat.begin());
	}

	for(cp = paths.begin(); cp != paths.end();  ++cp) {
		pesoA = tamanhoCaminho(*cp, grafo);
		if (pesoA > maiorP) {
			maiorP = pesoA;
			tmp = pesoA;
			caminhoCritico.insert(caminhoCritico.begin(), *cp);
		}
	}

	return caminhoCritico;
}

//mostra no terminal o caminho mais longo do grafo
void printV(vector<int> v) {
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		cout << *it;
		if(it == v.end()-1){
			cout << endl;
		}else{
			cout << " -> ";
		}
	}
}


int main(){

    Grafo *grafo = new Grafo(34);
    grafo->montaGrafo();

    cout << "Bacharelado em Ciencia da Computacao" << endl;
    cout << "Disciplinas do curso:" << endl << endl;
    grafo->printGrafo(grafo);
	
	OrdenacaoTopologica top;
    cout << endl;
    top.sortTopological(grafo);

    cout << endl;
	vector<vector<int>> caminhoCritico = grafo->cpm(grafo);
    cout << "Caminho Critico do grafo: " << endl;
    printV(caminhoCritico[0]);
    cout << "Peso Total: " << tmp << endl;
	return 0;
}