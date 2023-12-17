#include "pch.h"
#include "Graf.h"

// Esborra tot el contingut del graf
void GrafSolucio::clear()
{
    // Esborra tots els nodes del vector de nodes
    m_nodes.clear();

    // Esborra tot el contingut de la matriu d'adjac�ncia
    m_matriuAdj.clear();

    // Restableix el nombre total de nodes a zero
    m_numNodes = 0;

    // Restableix el valor de DISTMAX com a m�xim valor possible de tipus double
    DISTMAX = std::numeric_limits<double>::max();
}

// Troba la posici� on es troba el node dins del vector m_nodes
size_t GrafSolucio::trobarPosicio(const Coordinate& coord)
{
    // Inicialitza la variable que indica si s'ha trobat el node i l'�ndex del vector
    bool trobat = false;
    size_t i = 0;

    // Cerca la posici� del node amb les coordenades donades
    while (i < m_numNodes && !trobat)
    {
        // Compara les coordenades del node actual amb les coordenades donades
        if (m_nodes[i].lat == coord.lat && m_nodes[i].lon == coord.lon)
        { // Si les coordenades coincideixen, indica que s'ha trobat i retorna l'�ndex
            trobat = true;
            return i;
        }
        else // Si no coincideixen, incrementa l'�ndex per continuar la cerca
            i++;
    }

    // Si no es troba cap coincid�ncia, retorna -1
    return -1;
}

// Afegeix un node al graf amb les coordenades proporcionades
void GrafSolucio::afegirNode(const Coordinate& coord)
{
    // Afegir les coordenades del node al vector de nodes
    m_nodes.push_back(coord);

    // Afegir una fila buida a la matriu d'adjac�ncia per al nou node
    m_matriuAdj.push_back(std::vector<double>(m_numNodes));

    // Incrementar el nombre total de nodes al graf
    m_numNodes++;

    // Inicialitzar les entrades de la nova fila a zero a la matriu d'adjac�ncia
    for (size_t i = 0; i < m_numNodes; i++)
        m_matriuAdj[i].push_back(0);
}

// Afegeix una aresta entre dos nodes del graf amb les coordenades proporcionades
void GrafSolucio::afegirAresta(const Coordinate& coord1, const Coordinate& coord2)
{
    // Troba les posicions dels nodes a la matriu d'adjac�ncia
    size_t pos1 = trobarPosicio(coord1);
    size_t pos2 = trobarPosicio(coord2);

    // Si es troben ambdues posicions, calcula la dist�ncia i afegeix l'aresta
    if (pos1 != -1 && pos2 != -1)
    {
        // Calcula la dist�ncia entre els nodes utilitzant la f�rmula Haversine
        double dist = Util::DistanciaHaversine(coord1, coord2);

        // Afegeix la dist�ncia com a valor d'aresta entre els dos nodes a la matriu
        m_matriuAdj[pos1][pos2] = dist;
        m_matriuAdj[pos2][pos1] = dist;
    }
}

// Troba el node no visitat amb la dist�ncia m�nima
size_t GrafSolucio::minDistance(const std::vector<double>& dist, const std::vector<bool>& visited) const
{
    // Inicialitza el valor m�nim com a infinit
    double min = DISTMAX;

    // Inicialitza l'�ndex corresponent al node amb dist�ncia m�nima com a -1
    size_t minIndex = -1;

    // Recorre tots els nodes
    for (size_t i = 0; i < m_numNodes; i++)
    {
        // Comprova si el node no ha estat visitat i si la seva dist�ncia �s menor o igual al m�nim actual
        if (!visited[i] && dist[i] <= min)
        {
            // Actualitza el m�nim i l'�ndex corresponent
            min = dist[i];
            minIndex = i;
        }
    }

    // Retorna l'�ndex del node amb la dist�ncia m�nima
    return minIndex;
}

// Implementaci� de l'algorisme de Dijkstra per trobar el cam� m�s curt
void GrafSolucio::dijkstra(size_t node1, size_t node2, std::vector<double>& dist, std::vector<size_t>& anterior)
{
    // Inicialitza vectors
    std::vector<bool> visited(m_numNodes, false);
    dist.resize(m_numNodes, DISTMAX); // Inicialitza les dist�ncies com a infinit
    anterior.resize(m_numNodes, -1);

    // Inicialitza la dist�ncia al node origen com a zero
    dist[node1] = 0;
    anterior[node1] = node1;

    // Itera sobre tots els nodes
    for (size_t i = 0; i < m_numNodes - 1; i++)
    {
        // Troba el node amb la dist�ncia m�nima no visitat
        size_t index = minDistance(dist, visited);
        visited[index] = true;

        // Si es troba el node dest�, surt de la funci�
        if (index == node2)
            return;

        // Itera sobre tots els nodes per actualitzar les dist�ncies
        for (size_t j = 0; j < m_numNodes; j++)
        {
            // Actualitza la dist�ncia si es troba un cam� m�s curt
            if (m_matriuAdj[index][j] != 0 && !visited[j] && dist[index] + m_matriuAdj[index][j] < dist[j])
            {
                dist[j] = dist[index] + m_matriuAdj[index][j];
                anterior[j] = index;
            }
        }
    }
}

// Troba el cam� m�s curt entre dos coordenades utilitzant l'algorisme de Dijkstra
void GrafSolucio::camiMesCurt(const Coordinate& desde, const Coordinate& a, std::stack<Coordinate>& cami)
{
    // Troba les posicions dels nodes corresponents a les coordenades donades
    size_t pos1 = trobarPosicio(desde);
    size_t pos2 = trobarPosicio(a);

    // Si es troben ambdues posicions, calcula el cam� m�s curt
    if (pos1 != -1 && pos2 != -1)
    {
        // Inicialitza vectors per a l'algorisme de Dijkstra
        std::vector<size_t> anterior;
        std::vector<double> dist;

        // Aplica l'algorisme de Dijkstra per trobar les dist�ncies m�s curtes
        dijkstra(pos1, pos2, dist, anterior);

        // Inicialitza l'�ndex per reconstruir el cam� des de 'a' fins a 'desde'
        size_t it = pos2;

        // Afegir el node final del cam� a la pila
        cami.push(m_nodes[pos2]);

        // Reconstrueix el cam� des de 'a' fins a 'desde'
        while (it != pos1)
        {
            cami.push(m_nodes[anterior[it]]);
            it = anterior[it];
        }
    }
}