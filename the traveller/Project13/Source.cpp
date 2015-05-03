#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <thread>
#include <ctime>
#include <vector>
#include <map>
#include <utility>

using namespace std;

//--------------------------------------------------------
//			codigo para uma funcao devolver 2 valores
//--------------------------------------------------------
//template functor
template <typename T1, typename T2>
struct t_unpair
{
	T1& a1;
	T2& a2;
	explicit t_unpair(T1& a1, T2& a2): a1(a1), a2(a2)	{}
	t_unpair<T1, T2>& operator = (const pair<T1, T2>& p)
	{
		a1 = p.first;
		a2 = p.second;
		return *this;
	}
};

//functor helper
template < typename T1, typename T2 >
t_unpair<T1, T2> unpair(T1& a1, T2& a2)
{
	return t_unpair<T1, T2>(a1, a2);
}

//-----------------------------------------------------------------
//		Agora ja tenho template para uma funcao devolver 2 valores
//-----------------------------------------------------------------

//general variables
float distancia[15][15];
float tempo[15][15];
float price[15][15];
int origem_index;
int destino_index;


int temp;
float soma_final = 100000000;
int rota_final[15];

//general methods para definir as matrizes de dados
template <size_t size_x, size_t size_y>
void get_matrix(float (&matriz_nula)[size_x][size_y], string name_matrix);

//general methods do algoritmo
pair<int, float> procura_min(vector<int> rotas, int indice, float matriz[15][15]);
void procura_next3min(vector<int> rotas, vector<int> temp_rota, float soma, int indice, float matriz[15][15]);


int main()
{
	//lista dos paises
	std::string countries[15] = { "Amsterdam", "Athens", "Berlin",	"Bern",	"Brussels",	"Copenhagen",	"Edinburgh",	"Lisbon",	"London",	"Luxembourg",	"Madrid",	"Paris",	"Prague",	"Rome",	"Vienna" };

	//adquirir as matrizes de dados
	get_matrix(distancia, "distance.txt");
	get_matrix(tempo, "time.txt");
	get_matrix(price, "cost.txt");

	//parametros de entrada
	std::string origem;
	std::string destino;
	int caso0 = 10;
	int caso1 = 10;
	std::string variavel[2];


	while (caso0 != 0)
	{

		//Iniciacao com interaccao:
		cout << "O programa de seleccao de melhores rotas vai entao ser iniciado." << endl;
		cout << "O utilizador pretende seleccionar uma rota ou sair do programa?" << endl;
		cout << " 0 - exit" << endl;
		cout << " 1 - seleccionar rota " << endl;
		cin >> caso0;
		cout << endl;
		cout << endl;

		if (caso0 == 0)
		{
			cout << "Obrigado por utilizar o programa." << endl;
			break;
		}
			
		
		//definir a entrada pelo utilizador
		cout << "Introduza a cidade de origem com primeira letra maiuscula: " << endl;
		cin >> origem;
		cout << endl;

		cout << "Introduza a cidade de destino com primeira letra maiuscula: " << endl;
		cin >> destino;
		cout << endl;
		cout << endl;

		cout << "A preparar os dados..." << endl;

		//determinacao dos indices de entrada e saida

		for (int index = 0; index < 15; index++)
		{
			if (countries[index].compare(origem) == 0)
				origem_index = index;
			if (countries[index].compare(destino) == 0)
				destino_index = index;
		}

		int trajectos[15];
		for (int t = 0; t < 15; t++)
		{
			trajectos[t] = t;
		}

		//ordenar segundo a escolha do utilizador
		trajectos[destino_index] = trajectos[14];
		trajectos[14] = destino_index;
		trajectos[origem_index] = trajectos[0];
		trajectos[0] = origem_index;

		//preparar os vectorers e os dados de entrada para iniciar o algoritmo
		int cidade = origem_index;
		vector<int> rotas;
		vector<int> temp_rotas;
		for (int k = 1; k < 14; k++)
			rotas.push_back(trajectos[k]);


		cout << "Qual a caracteristica que vai definir a sua rota? (seleccione a opcao que pretende: " << endl;
		cout << "1 - Distancia mais curta" << endl;
		cout << "2 - Tempo mais curto" << endl;
		cout << "3 - Mais barato" << endl;
		cin >> caso1;
		cout << endl;
		cout << endl;
		cout << "A determinar a melhor rota..." << endl;

		switch (caso1)
		{
		case 1: 
			variavel[0] = "distancia";
			variavel[1] = "km";
			//determinar melhor trajecto
			temp_rotas.push_back(trajectos[0]);
			procura_next3min(rotas, temp_rotas, 0, trajectos[0], distancia);
			//adicionar o ultimo percurso ate ao destino
			rota_final[14] = trajectos[14];
			//soma_final += distancia[rota_final[13]][rota_final[14]];
			break;
		case 2:
			variavel[0] = "tempo";
			variavel[1] = "horas";
			temp_rotas.push_back(trajectos[0]);
			procura_next3min(rotas, temp_rotas, 0, trajectos[0], tempo);
			//adicionar o ultimo percurso ate ao destino
			rota_final[14] = trajectos[14];
			//soma_final += tempo[rota_final[13]][rota_final[14]];
			break;
		case 3:
			variavel[0] = "preco";
			variavel[1] = "euros";
			temp_rotas.push_back(trajectos[0]);
			procura_next3min(rotas, temp_rotas, 0, trajectos[0], price);
			//adicionar o ultimo percurso ate ao destino
			rota_final[14] = trajectos[14];
			//soma_final += price[rota_final[13]][rota_final[14]];
			break;
		}

		cout << "Melhor rota determinada!" << endl;
		cout << "A rota que percorre as cidade em menor " << variavel[0] << " começando em " << countries[origem_index]
			<< " e terminando em " << countries[destino_index] << " e a seguinte: " << endl;

		for (int i = 0; i < 14; i++)
		{
			cout << countries[rota_final[i]] << " -> ";
		}
		cout << countries[rota_final[14]] << endl;
		cout << " num total de: " << soma_final << " " << variavel[1];
		cout << endl;
		cout << endl;
	}
	

	//--------------------------------------------------
	//		Algoritmo para deteccao da melhor rota	
	//--------------------------------------------------


	return 0;

}

template <size_t size_x, size_t size_y>
void get_matrix(float (&matriz_nula)[size_x][size_y], string name_matrix)
{
	//abrir ficheiro
	std::ifstream matriz(name_matrix);

	std::string content;
	//tirar a primeira linha
	getline(matriz, content);
	
	int line_index = 0;	//indice da linha lida no ciclo
	int size = 15; //tamanho inicial do preenchimento de cada linha
	while (getline(matriz, content))
	{
		//remover primeira palavra de cada linha
		istringstream alpha{ content }; 
		string first;
		alpha >> first;

		//converter cada palavra a inteiro e introduzir na matriz
		
		for (int i = 0; i < size; i++)
		{
			float s;
			alpha >> s;
			matriz_nula[line_index][(i+line_index)] = s;
			matriz_nula[(i + line_index)][line_index] = s;
		}

		size--;
		line_index++;
	}

	//fechar ficheiro
	matriz.close();

}

//next neighborhood algorithm
pair<int, float> procura_min(vector<int> rotas, int indice, float matriz[15][15])
{
	float s;

		float x = 100000;
		for (const auto &i : rotas)
		{
			if (x > matriz[indice][i])
			{
				x = matriz[indice][i];
				temp = i;
			}
		}
		//calcular a distancia final
		s = x;
		//anular posicao anterior

	
		return make_pair(temp, s);
}

void procura_next3min(vector<int> rotas, vector<int> temp_rota, float soma, int indice, float matriz[15][15])
{

	//vector cidade visitada
	vector<int>::iterator cidade_visitada, temp_cidade_visitada;

	
	//caso final, em que a escolha se prende com os 3 ultimos destinos possiveis
	if (rotas.size() == 1)
	{
		
		if (soma_final > (soma + matriz[indice][rotas[0]] + matriz[rotas[0]][destino_index]))
		{
			soma_final = (soma + matriz[indice][rotas[0]] + matriz[rotas[0]][destino_index]);
			for (int i = 0; i < temp_rota.size(); i++)
					rota_final[i] = (temp_rota[i]);
			/*{
				cout << temp_rota[i] << "->";
			}
			cout << rotas[0] << endl;
			cout << soma_final;
*/
			rota_final[13] = rotas[0];
		}
		
	}
	else if (rotas.size() == 2)
	{
		int cidade[2];
		float dist[2];
		
		//procura o minimo entre os dois pontos restantes
		unpair(cidade[0], dist[0]) = procura_min(rotas, indice, matriz);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[0]);
		rotas.erase(cidade_visitada);
		temp_rota.push_back(cidade[0]);

		//procura rota 1
		procura_next3min(rotas, temp_rota, (dist[0] + soma), cidade[0], matriz);

		cidade[1] = rotas[0];
		dist[1] = matriz[indice][rotas[0]];

		//procura rota 2
		rotas.push_back(cidade[0]);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[0]);
		temp_rota.erase(temp_cidade_visitada);

		temp_rota.push_back(cidade[1]);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[1]);
		rotas.erase(cidade_visitada);
		procura_next3min(rotas, temp_rota, (dist[1] + soma), cidade[1], matriz);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[1]);
		temp_rota.erase(temp_cidade_visitada);
	}
	else if (rotas.size() == 3)
	{
		int cidade[3];
		float dist[3];

		//procura 1º minimo
		unpair(cidade[0], dist[0]) = procura_min(rotas, indice, matriz);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[0]);
		rotas.erase(cidade_visitada);
		temp_rota.push_back(cidade[0]);
		

		procura_next3min(rotas, temp_rota, (dist[0] + soma), cidade[0], matriz);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[0]);
		temp_rota.erase(temp_cidade_visitada); 


		//procuro 2º minimo
		unpair(cidade[1], dist[1]) = procura_min(rotas, indice, matriz);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[1]);
		rotas.erase(cidade_visitada);
		cidade[2] = rotas[0];
		dist[2] = matriz[indice][cidade[2]];

		//procuro proximos 2 minimos para rota 2
		rotas.push_back(cidade[0]);
		temp_rota.push_back(cidade[1]);
		procura_next3min(rotas, temp_rota, (dist[1] + soma), cidade[1], matriz);
	
		//procuro proximos 2 minimos para rota 3
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[1]);
		temp_rota.erase(temp_cidade_visitada);
		rotas.push_back(cidade[1]);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[2]);
		rotas.erase(cidade_visitada);
		temp_rota.push_back(cidade[2]);
		procura_next3min(rotas, temp_rota, (dist[2] + soma), cidade[2], matriz);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[2]);
		temp_rota.erase(temp_cidade_visitada);


	}
	else
	{
		//array cidades
		int cidade[3];
		//array distancias, tempo, preco 
		float dist[3];


		//procura 1o minimo
		unpair(cidade[0], dist[0]) = procura_min(rotas, indice, matriz);
		//retiro cidade visitada
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[0]);
		rotas.erase(cidade_visitada);
		

		//procura proximos 3 minimos para rota 1
		//coloco cidade visitada na rota percorrida
		temp_rota.push_back(cidade[0]);
		procura_next3min(rotas, temp_rota, (dist[0] + soma), cidade[0], matriz);
		

		//procura 2o minimo
		unpair(cidade[1], dist[1]) = procura_min(rotas, indice, matriz);
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[1]);
		rotas.erase(cidade_visitada);
		
		//preparar rotas temporarias e rota percorrida para rota 2
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[0]);
		temp_rota.erase(temp_cidade_visitada);
		rotas.push_back(cidade[0]);
		temp_rota.push_back(cidade[1]);

		//procura 3o minimo
		unpair(cidade[2], dist[2]) = procura_min(rotas, indice, matriz);

		//procura proximos 3 minimos para rota 2
		procura_next3min(rotas, temp_rota, (dist[1] + soma), cidade[1], matriz);
		
		//retira 3o minimo
		cidade_visitada = find(rotas.begin(), rotas.end(), cidade[2]);
		rotas.erase(cidade_visitada);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[1]);
		temp_rota.erase(temp_cidade_visitada);


		//procura proximos 3 minimos para rota 3
		rotas.push_back(cidade[1]);
		temp_rota.push_back(cidade[2]);
		procura_next3min(rotas, temp_rota, (dist[2] + soma), cidade[2], matriz);
		temp_cidade_visitada = find(temp_rota.begin(), temp_rota.end(), cidade[2]);
		temp_rota.erase(temp_cidade_visitada);
	}

}

