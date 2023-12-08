#include"pch.h"
#include "BallTree.h"

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades)
{
    // TODO: Utilitza aquest metode per construir el teu BallTree
    // TODO: amb les coordenades que se us passen per parametre
    // Calucla el punt central dels nodes (punt C)
    // Si el vector coordenades es buit retornem
    if (coordenades.empty())
        return;

    // Assignem el vector coordenades al vector de la classe BallTree m_coordenades
    m_coordenades = coordenades;
    // Calucla el punt central dels nodes (punt C) i assignem C com a pivot
    Coordinate C = Util::calcularPuntCentral(m_coordenades);
    m_pivot = C;

    // Calculem el radi
    for (size_t i = 0; i < m_coordenades.size(); i++)
    {
        double dist = Util::DistanciaHaversine(m_coordenades[i], m_pivot);
        if (dist > m_radi)
            m_radi = dist;
    }

    // Calcula totes les distàncies dels nodes respecte del punt C
    std::vector<double> distC;
    for (size_t i = 0; i < m_coordenades.size(); i++)
        distC.push_back(Util::DistanciaHaversine(C, m_coordenades[i]));

    // Agafem el punt més llunyà (punt A)
    size_t indexA = std::distance(distC.begin(), std::max_element(distC.begin(), distC.end()));
    Coordinate A = m_coordenades[indexA];

    // Calcula totes les distàncies dels nodes respecte el punt A
    std::vector<double> distA;
    for (size_t i = 0; i < m_coordenades.size(); i++)
        distA.push_back(Util::DistanciaHaversine(A, m_coordenades[i]));

    // Agafem el punt més llunyà (punt B)
    size_t indexB = std::distance(distA.begin(), std::max_element(distA.begin(), distA.end()));
    Coordinate B = m_coordenades[indexB];

    // Creem les llistes de nodes per a cada bola
    std::vector<Coordinate> bolaEsq, bolaDreta;

    // Per a cada node
    for (size_t i = 0; i < m_coordenades.size(); i++) {
        // Calcula la distància node al punt A (D1)
        double D1 = Util::DistanciaHaversine(A, m_coordenades[i]);

        // Calcula la distància node al punt B (D2)
        double D2 = Util::DistanciaHaversine(B, m_coordenades[i]);

        //Si D1 < D2, assignem el node a la bola esquerra
        //Si D1 >= D2, assignem el node a la bola dreta
        if (D1 < D2)
            bolaEsq.push_back(m_coordenades[i]);
        else
            bolaDreta.push_back(m_coordenades[i]);
    }

    // Creem les dues boles
    BallTree* esquerra = new BallTree();
    esquerra->setPivot(A);
    esquerra->setCoordenades(bolaEsq);
    esquerra->setRadius(*std::max_element(distA.begin(), distA.end()));

    BallTree* dreta = new BallTree();
    dreta->setPivot(B);
    dreta->setCoordenades(bolaDreta);

    // Calcula totes les distàncies dels nodes respecte el punt B
    std::vector<double> distB;
    for (size_t i = 0; i < bolaDreta.size(); i++)
        distB.push_back(Util::DistanciaHaversine(B, bolaDreta[i]));

    dreta->setRadius(*std::max_element(distB.begin(), distB.end()));

    // Assignem les boles a l'arbre actual i l'arrel
    this->setEsquerre(esquerra);
    this->setDreta(dreta);

    m_root = this;

    // Si les boles tenen més d'un node, cridem recursivament a construirArbre
    if (bolaEsq.size() > 1) {
        m_left = esquerra;
        m_left->construirArbre(bolaEsq);
    }
    if (bolaDreta.size() > 1) {
        m_right = dreta;
        m_right->construirArbre(bolaDreta);
    }
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out)
{
    // TODO: TASCA 2
    if (this != nullptr)
    {
        // Recorrem el subarbre esquerra
        if (m_left != nullptr)
            m_left->inOrdre(out);

        // Creem una llista per a les coordenades actuals
        std::list<Coordinate> currentCoordinates;

        // Afegix totes les coordenades del node actual a la llista
        for (const auto& coordinate : this->getCoordenades())
            currentCoordinates.push_back(coordinate);

        // Afegim la llista de coordenades al vector de sortida
        out.push_back(currentCoordinates);

        // Recorreme el subarbre dret
        if (m_right != nullptr)
            m_right->inOrdre(out);
    }
}

void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out)
{
    // TODO: TASCA 2
    // Comprovem que el node actual no es nul
    if (this != nullptr)
    {
        // Creem una llista per a les coordenades actuals
        std::list<Coordinate> currentCoordinates;

        // Afegix totes les coordenades del node actual a la llista
        for (const auto& coordinate : this->getCoordenades())
            currentCoordinates.push_back(coordinate);

        // Afegim la llista de coordenades al vector de sortida
        out.push_back(currentCoordinates);

        // Recorrem el subarbre esquerra
        if (m_left != nullptr)
            m_left->preOrdre(out);

        // Recorreme el subarbre dret
        if (m_right != nullptr)
            m_right->preOrdre(out);
    }
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out)
{
    // TODO: TASCA 2
    if (this != nullptr)
    {
        // Recorrem el subarbre esquerra
        if (m_left != nullptr)
            m_left->postOrdre(out);

        // Recorrem el subarbre dret
        if (m_right != nullptr)
            m_right->postOrdre(out);

        // Creem una llista per a les coordenades actuals
        std::list<Coordinate> currentCoordinates;

        // Afegix totes les coordenades del node actual a la llista
        for (const auto& coordinate : this->getCoordenades())
            currentCoordinates.push_back(coordinate);

        // Afegim la llista de coordenades al vector de sortida
        out.push_back(currentCoordinates);
    }
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball)
{
    if (ball == nullptr)
        return Q;

    // Calcula la distancia del punt central de la bola respecte al targetQuery (D1)
    double D1 = Util::DistanciaHaversine(ball->getPivot(), targetQuery);

    // Calcula la distancia del punt central de la bola respecte al Q (D2)
    double D2 = Util::DistanciaHaversine(ball->getPivot(), Q);

    // Si D1 - ball.radi >= D2 retorna Q
    if (D1 - ball->getRadi() >= D2)
        return Q;

    // Si la bola es una hoja del árbol, actualiza Q si es el nodo camino más cercano al punto de interés, de los puntos que forman la bola
    if (ball->getEsquerre() == nullptr && ball->getDreta() == nullptr)
    {
        for (const Coordinate& coord : ball->getCoordenades())
        {
            if (Util::DistanciaHaversine(coord, targetQuery) < Util::DistanciaHaversine(Q, targetQuery))
                Q = coord;
        }

        return Q;
    }

    // Calcula la distancia del targetQuery respecto al punt central de la bola izquierda (Da)
    double Da = Util::DistanciaHaversine(targetQuery, ball->getEsquerre()->getPivot());

    // Calcula la distancia del targetQuery respecto al punt central de la bola derecha (Db)
    double Db = Util::DistanciaHaversine(targetQuery, ball->getDreta()->getPivot());

    // Si Da < Db, comienza la búsqueda por la bola izquierda y después por la derecha
    // Si Da >= Db, comienza la búsqueda por la bola derecha y después por la izquierda
    if (Da < Db)
    {
        Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
        Q = nodeMesProper(targetQuery, Q, ball->getDreta());
    }
    else
    {
        Q = nodeMesProper(targetQuery, Q, ball->getDreta());
        Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
    }

    return Q;
}





