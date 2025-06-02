#include "graph.h"

Vertex::Vertex(const std::string& name) : name(name), next(nullptr) {}
Vertex::~Vertex() {
    for (Edge* edge : edges) delete edge;
}

Edge::Edge(Vertex* dest, int weight) : destination(dest), weight(weight) {}

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

void Graph::addEdge(const std::string& source, const std::string& destination, int distance) {
    Vertex* src = findOrCreateVertex(source);
    Vertex* dest = findOrCreateVertex(destination);
    src->edges.push_back(new Edge(dest, distance));
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
