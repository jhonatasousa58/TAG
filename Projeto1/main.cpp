/* 
    Universidade de Brasilia
    Departamento de Ciencia da Computacao
    Projeto 1 - Cliques Maximais utilizando o algoritimo de Bron-Kerbosch e Coeficiente de Aglomeracao
    Teoria e Aplicacao de Grafos, Turma A, 1/2019
    Prof. Díbio

   Autores: Adelson Jhonata Silva de Sousa - 18/0114913
            Heitor de Lima - 16/0123950
*/
#include <bits/stdc++.h>
#include "grafo.h"

using namespace std;

//funcao para criar uma nova lista de adjacencias
Grafo::Grafo(int v) {
        this->V = v;
        adj = new vector<int>[v];
}

//responsavel por adicionar as arestar e ligar os vertices
void Grafo::adcAresta(int v, int w)  {
        adj[v].push_back(w);
        adj[w].push_back(v);
}

//Le o arquivo karate.gml e adiciona as arestas ao respectivos vertices nas listas de adjacencias seguindo o padrao do arquivo gml.
//retorna o grafo montado
Grafo montaGrafo(){
    ifstream arq ("karate.gml");
    Grafo k(34);
    int source, target;
    char* lineCor;
    string line;

    if (arq.is_open()){
        while (! arq.eof() ){
            getline (arq,line);
           
            lineCor = (char*) line.c_str();

        	if (strstr(lineCor, "source")) {
				sscanf(lineCor, " %*s %d", &source);
			}
			else if (strstr(lineCor, "target")) {
				sscanf(lineCor, " %*s %d", &target);
				k.adcAresta(source-1, target-1);
			}
        }
        arq.close();
    }else{
        cout << "Impossivel ler o arquivo!";
    }

	return k;
}

//mostra o grafo na tela de modo a mostrar os vertices e o grau de cada vertice
void printaGrafo(Grafo k){
    int i;
    int g;
    for(i = 0; i < 34; i++){
        g = k.adj[i].size();
        printf("O vertice %d tem grau %d\n", i+1, g);
    }
    
    printf("\n");
}

//funcao de uniao, une dois vetores e ordena, eh usada no algoritimo de Bron-Kerbosch 
vector<int> Uniao(vector<int> v1, vector<int> v2){
       
		int n = v1.size()+v2.size();

		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());

        vector<int> v(n); 
        auto it = set_union(v1.begin(), v1.end(), v2.begin(),  v2.end(), v.begin()); 

        v.resize(it - v.begin());

        return v;
}

//funcao de interseccao de vetores e ordenacao dos vetores, eh usada no algoritimo de Bron-Kerbosch
vector<int> Interseccao(vector<int> v1, vector<int> v2){

	int n = v1.size()+v2.size();

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
    
    vector<int> v(n); 
    auto it = set_intersection(v1.begin(), v1.end(), v2.begin(),  v2.end(), v.begin()); 

    v.resize(it - v.begin());

    return v;

}

//funcao de diferenca entre dois vetores, eh usada no algoritimo de Bron-Kerbosch
vector<int> Diferenca(vector<int> v1, vector<int> v2){

	int n = v1.size()+v2.size();

	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

    vector<int> v(n); 
    auto it = set_difference(v1.begin(), v1.end(), v2.begin(),  v2.end(), v.begin()); 

    v.resize(it - v.begin());

    return v;

}

//retorna os vizinhos do vertice
vector<int> Vizinhos(int v, Grafo k) {
        if(k.adj[v].size() == 0) {
        	return {};
        }
        return k.adj[v];

}

//vetor global para armazenar todos os cliques maximais, o vetor eh global pois é utilizado tambem na funcao de aglomeracao
vector<vector<int>> Cliques;

//Algoritimos de Bron-Kerbosch, responsavel por percorrer todo o grafo e retornar os cliques maximais
//o algoritimos foi desenvolvido a partir do pseudo codigo disponibilizado no moodle e apresentado em sala de aula.
void BronK(vector<int> R, vector<int> P, vector<int> X, Grafo k) {
        vector<int> recebeV;
        vector<int> newR;
        vector<int> newP;
        vector<int> newX;


        if(P.empty() && X.empty()) {
                Cliques.push_back(R);
        }

        vector<int>::iterator v = P.begin();
        while(!P.empty() && v != P.end()) {
        		recebeV = {*v};
        		newR = Uniao(R, recebeV);
        		newP = Interseccao(P, Vizinhos(*v, k));
        		newX = Interseccao(X, Vizinhos(*v, k));

                BronK(newR, newP, newX, k);
                P = Diferenca(P, recebeV);
                X = Uniao(X, recebeV);
                if(!P.empty()) {
                    v = P.begin();
                }
        }
}

//Printa na tela a quantidade de cliques maximais e o vertices dos cliques
void printaMaxl() {
    int i = 0;
        for(vector<int> it : Cliques) {
            cout << "Quantidade de vertices: " << Cliques[i].size();
            cout << ", Clique maximal(vertices): ";
            cout << "( ";
            
            for(int c : it){

                    cout << c + 1 << " ";
                
                }
                cout << ")" << endl;
            i++;
        }
}

//funcao que percorre um vetor e ve se o valor esta dentro do mesmo
bool taDentro(vector<int> l, int v) {
        vector<int>::iterator it;
        for (it = l.begin(); it !=  l.end(); ++it) {
            if(*it == v){
                return true;
            }
        }
        return false;
}

//Funcao que calcula  o coeficiente de aglomeracao de cada vertice e armazena em vetor com o coeficiente de todos os vertices.
//estao todos armazenado no mesmo vetor para facilitar o calculo da media de aglomeracao do grafo todo
vector<float> coeAglo(Grafo k) {
 
        float calculo;
        int tri = 0;
        float coef;


        //procura triangulos no grafo com ajuda do vetor de cliques maximais
        //assim facilitando o calculo do coeficiente de aglomeracao
        vector<vector<int>> triangulos;
        vector<vector<int>>::iterator it = Cliques.begin();

        for(it = Cliques.begin(); it != Cliques.end(); ++it){
                if(it->size() >= 3) {
                    triangulos.push_back(*it);
                }
        }



        vector<float> coeficientes;

        for(int i = 0; i < 34; i++) {
            int te = k.adj[i].size();
            calculo =  te * (te - 1);
            if(calculo < 1){
                coeficientes.push_back(0);
            }else{
                tri = 0;
                for(int j = 0; j < triangulos.size(); j++) {
                    if(taDentro(triangulos[j],i)){
                        if(triangulos[j].size() == 5){
                            tri += 4;
                        }
                        else if(triangulos[j].size() == 4){
                            tri += 3;
                        }
                        else {
                            tri++;
                        }
                    }
                }
                coef = (2*tri);
                coef = coef/calculo;
                coeficientes.push_back(coef);
            }
        }

        return coeficientes;
}

//mostra na tela nos coeficientes de cada vertice e retorna a soma de todos eles para calcular a media do grafo
float printaCoe(vector<float> coeficientes){
        float media = 0.0;
        vector<float>::iterator it;
        int i = 1;
        for(it = coeficientes.begin(); it != coeficientes.end(); ++it){
            cout << "Vertice: " << i << ", Coeficiente : " << *it << " " << endl;
            media += *it;
            i++;
        }
        return media;
}


int main(){

    //variaveis basicas para o funcionamento
	int size = 34;
    Grafo k(size);
    vector<int> X, R, P;
    float media = 0.0;

    //alimenta o vetor P para a funcao de Bron-Kerbosch
    //nao esta direto na funcao pois a funcao é recursiva
    //assim a funcao nao buga
    for(int i = 0; i < size; i++)
        P.push_back(i);
    

    //monta o grafo
    k = montaGrafo();
	
    printf("\nPressione Enter para ver os vertices e seus respectivos graus...\n");
    getchar();

    //mostra os vertices e grau na tela
    printaGrafo(k);

    //chama a funcao de Bron-Kerbosch para calcular os cliques Maximais
    BronK(R, P, X, k);

    printf("Pressione Enter para ver os cliques Maximais e seus vertices...\n");
    getchar();
    
    //mostra na tela os cliques maximais, seus vertices e a quatindade de vertices
    printaMaxl();

    //vetor para receber o calculo de coeficientes de todos os vertices
    vector<float> coeficientes;

    //chama a funcao para calcular os coeficientes
    coeficientes = coeAglo(k);

    printf("\nPressione Enter para ver o coeficiente de aglomeracao...\n");
    getchar();

    //mostra na tela os coeficientes de cada vertice e recebe a soma de todos os coeficientes
    media = printaCoe(coeficientes);

    cout << endl;
    //calculo de coeficiente medio do grafo todo e exibe na tela
    cout << "O Coeficiente medio de Aglomeracao do Grafo e de: " << media/34.00 << endl;

	return 0;
}