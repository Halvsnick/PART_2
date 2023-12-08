#pragma once
#include <iostream>
#include <vector>
#include "MapaSolucio.h"
#include "PuntDeInteresBase.h"

using namespace std;

class Graph : public MapaSolucio {
private:

    int VoW; // Número de vértices o ways
    vector<vector<Coordinate>> W_graf; // Grafo con ways

    vector<PuntDeInteresBase*> m_WoPI;
    vector<CamiBase*> m_Ways;

    int findNodeIndex(const Coordinate& coord) {
        for (int i = 0; i < W_graf.size(); ++i) {
            if (W_graf[i][0].lat == coord.lat && W_graf[i][0].lon == coord.lon) {
                return i; // Retorna el índice del nodo si se encuentra la coordenada
            }
        }
        return -1; // Retorna -1 si no se encuentra la coordenada
    }

public:
    Graph() : VoW(0) {}

    void addNode(Coordinate node) 
    {
        if (find(W_graf.begin(), W_graf.end(), node) == W_graf.end()) // El nodo no está presente, así que lo añadimos al grafo
        {
            VoW++; // Incrementa el número de nodos en el grafo
            W_graf.resize(VoW); // Añade un nuevo vector vacío para el nuevo nodo
            W_graf.push_back({ node }); // Agrega la coordenada como un nuevo vector en el grafo
        }
       
    }

    void afegirAresta(const Coordinate& aresta)
    {
        // Obtiene los nodos asociados a la arista y añade conexiones al grafo
        int index = findNodeIndex(aresta);

        if (index != -1)
        {
            // Añade una conexión desde la nueva arista a los nodos existentes en el grafo
            for (int i = 0; i < W_graf.size(); ++i) {
                if (i != index) {
                    W_graf[i].push_back(aresta); // Conecta la arista con el nodo existente
                    W_graf[index].push_back(W_graf[i][0]); // Conecta el nodo existente con la arista
                }
            }
        }
    }
      
    void CrearGraf(vector<CamiBase*>& m_Ways)
    {
       
        for (CamiBase* way : m_Ways) {
            vector<Coordinate> coords = way->getCamiCoords();

            // Agregar los nodos al grafo
            for (const auto& coord : coords) {
                addNode(coord); // Agregar el nodo al grafo
            }

            // Conectar los nodos como aristas
            for (size_t i = 0; i < coords.size() - 1; ++i) {
                afegirAresta(coords[i + 1]); // Conectar el nodo actual con el siguiente
            }
        }

    }

};




