#include "graph.h"

Vertex::Vertex(const std::string& name) : name(name), next(nullptr) {}
Vertex::~Vertex() {
    for (Edge* edge : edges) delete edge;
}

Edge::Edge(Vertex* dest, int weight, bool isDirtRoad)
    : destination(dest), weight(weight), isDirtRoad(isDirtRoad) {}

Graph::Graph() : vertices(nullptr), distanceResult(0), costResult(0.0), timeResult(0.0) {}
Graph::~Graph() {
    clearGraph();
}

void Graph::clearGraph() {
    while (vertices) {
        Vertex* tmp = vertices;
        vertices = vertices->next;
        delete tmp;
    }
}

Vertex* Graph::findVertex(const std::string& name) {
    Vertex* current = vertices;
    while (current) {
        if (current->name == name) return current;
        current = current->next;
    }
    return nullptr;
}

Vertex* Graph::findOrCreateVertex(const std::string& name) {
    Vertex* found = findVertex(name);
    if (found) return found;

    Vertex* newVertex = new Vertex(name);
    newVertex->next = vertices;
    vertices = newVertex;
    return newVertex;
}

void Graph::addEdge(const std::string& source, const std::string& destination, int distance, bool isDirtRoad) {
    Vertex* src = findOrCreateVertex(source);
    Vertex* dest = findOrCreateVertex(destination);
    src->edges.push_back(new Edge(dest, distance, isDirtRoad));
}

void Graph::removeEdge(const std::string& source, const std::string& destination) {
    Vertex* src = findVertex(source);
    if (!src) return;

    src->edges.erase(std::remove_if(src->edges.begin(), src->edges.end(),
                                    [&destination](Edge* edge) {
                                        if (edge->destination->name == destination) {
                                            delete edge;
                                            return true;
                                        }
                                        return false;
                                    }), src->edges.end());
}

bool Graph::dijkstra(const std::string& start, const std::string& end) {
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> previous;
    std::set<std::string> visited;

    auto compare = [&](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
        return a.first > b.first;
    };
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, decltype(compare)> queue(compare);

    // Inicialização
    for (Vertex* v = vertices; v != nullptr; v = v->next) {
        distances[v->name] = std::numeric_limits<int>::max();
    }

    distances[start] = 0;
    queue.push({0, start});

    while (!queue.empty()) {
        auto [dist, currentName] = queue.top();
        queue.pop();

        if (visited.count(currentName)) continue;
        visited.insert(currentName);

        Vertex* current = findVertex(currentName);
        if (!current) continue;

        for (Edge* edge : current->edges) {
            std::string neighborName = edge->destination->name;
            int newDist = dist + edge->weight;

            if (newDist < distances[neighborName]) {
                distances[neighborName] = newDist;
                previous[neighborName] = currentName;
                queue.push({newDist, neighborName});
            }
        }
    }

    // Se destino não encontrado
    if (distances[end] == std::numeric_limits<int>::max()) return false;

    // Reconstroi o caminho
    pathResult.clear();
    std::string current = end;
    while (current != start) {
        pathResult.insert(pathResult.begin(), current);
        current = previous[current];
    }
    pathResult.insert(pathResult.begin(), start);

    // Define resultados
    distanceResult = distances[end];
    costResult = distanceResult * 0.69;
    timeResult = static_cast<double>(distanceResult) / 60.0;

    return true;
}

bool Graph::decisionTree(const std::string& start, const std::string& end) {
    Vertex* startVertex = findVertex(start);
    Vertex* endVertex = findVertex(end);

    if (!startVertex || !endVertex) return false;

    // Map para distâncias mínimas e predecessores
    std::unordered_map<Vertex*, int> distances;
    std::unordered_map<Vertex*, Vertex*> previous;

    // Inicializa distâncias com infinito
    for (Vertex* v = vertices; v != nullptr; v = v->next) {
        distances[v] = std::numeric_limits<int>::max();
        previous[v] = nullptr;
    }

    distances[startVertex] = 0;

    // Min-heap por distância (pair: distância, vértice)
    auto cmp = [](const std::pair<int, Vertex*>& left, const std::pair<int, Vertex*>& right) {
        return left.first > right.first;
    };
    std::priority_queue<std::pair<int, Vertex*>, std::vector<std::pair<int, Vertex*>>, decltype(cmp)> queue(cmp);

    queue.push({0, startVertex});

    while (!queue.empty()) {
        int dist = queue.top().first;
        Vertex* current = queue.top().second;
        queue.pop();

        if (dist > distances[current]) continue;

        if (current == endVertex) break;

        for (Edge* edge : current->edges) {
            // Ignora arestas de terra
            if (edge->isDirtRoad) continue;

            int alt = distances[current] + edge->weight;
            if (alt < distances[edge->destination]) {
                distances[edge->destination] = alt;
                previous[edge->destination] = current;
                queue.push({alt, edge->destination});
            }
        }
    }

    if (distances[endVertex] == std::numeric_limits<int>::max()) {
        // Caminho não encontrado
        return false;
    }

    // Salva resultados
    distanceResult = distances[endVertex];
    pathResult.clear();

    // Reconstrução do caminho
    for (Vertex* v = endVertex; v != nullptr; v = previous[v]) {
        pathResult.push_back(v->name);
    }
    std::reverse(pathResult.begin(), pathResult.end());

    // Atualiza costResult e timeResult se quiser, ou deixar para depois
    costResult = distanceResult * 1.0; // Exemplo
    timeResult = distanceResult / 40.0; // Exemplo: velocidade média 40 km/h

    return true;
}

int Graph::getDistance() const {
    return distanceResult;
}

double Graph::getCost() const {
    return costResult;
}

double Graph::getTime() const {
    return timeResult;
}

std::vector<std::string> Graph::getPath() const {
    return pathResult;
}
