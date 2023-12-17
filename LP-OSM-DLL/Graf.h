#pragma once
#include "Util.h"
#include "Common.h"
#include "CamiSolució.h"
#include <queue>
#include <stack>
#include <list>
#include <limits>

class GrafSolucio
{
public:
	GrafSolucio() : m_numNodes(0) { DISTMAX = std::numeric_limits<double>::max(); }

	void clear();

	void afegirNode(const Coordinate& coord1);
	void afegirAresta(const Coordinate& coord1, const Coordinate& coord2);

	size_t trobarPosicio(const Coordinate& coord);

	size_t minDistance(const std::vector<double>& dist, const std::vector<bool>& visited) const;
	void dijkstra(size_t node1, size_t node2, std::vector<double>& dist, std::vector<size_t>& anterior);

	void camiMesCurt(const Coordinate& desde, const Coordinate& a, std::stack<Coordinate>& cami);

private:
	std::vector<Coordinate> m_nodes;
	std::vector<std::vector<double>> m_matriuAdj;
	size_t m_numNodes;
	double DISTMAX;
};



