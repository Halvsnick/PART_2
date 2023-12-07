#pragma once
#include <iostream>
#include <vector>
#include "MapaSolucio.h"

using namespace std;

// Definición de un grafo utilizando una lista de adyacencia
class Graph : public MapaSolucio
{
private:
    int V; // Número de nodos en el grafo
    vector<vector<int>> adjacencyList; // Lista de adyacencia

    vector<PuntDeInteresBase*> m_WoPI;
    vector<CamiBase*> m_Ways;
    vector<string> m_Punts_id;

public:
    // Constructor que inicializa el grafo con V nodos
    Graph(int vertices) : V(vertices) {
        adjacencyList.resize(V); // Inicializa la lista de adyacencia con V elementos
    }

    // Función para agregar una arista entre dos nodos
    void addEdge(int u, int v) {
        adjacencyList[u].push_back(v); // Agrega v a la lista de adyacencia de u
        adjacencyList[v].push_back(u); // Para un grafo no dirigido, agregar u a la lista de v
    }

    // Función para imprimir el grafo
    void printGraph() {
        for (int i = 0; i < V; ++i) {
            cout << "Nodo " << i << " conectado con: ";
            for (int j = 0; j < adjacencyList[i].size(); ++j) {
                cout << adjacencyList[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    // Crear un grafo utilizando vectores
    int numNodes = 5; // Por ejemplo, un grafo con 5 nodos

    Graph graph(numNodes);

    // Agregar aristas al grafo
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);

    // Imprimir el grafo
    cout << "Representación del grafo:" << endl;
    graph.printGraph();

    return 0;
}

