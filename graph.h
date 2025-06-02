#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <limits>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>

struct Edge;

struct Vertex {
    std::string name;
    std::vector<Edge*> edges;
    Vertex* next;

    Vertex(const std::string& name);
    ~Vertex();
};

struct Edge {
    Vertex* destination;
    int weight;

    Edge(Vertex* dest, int weight);
};

class Graph {
public:
    Graph();
    ~Graph();

    void addEdge(const std::string& source, const std::string& destination, int distance);
    void removeEdge(const std::string& source, const std::string& destination);
    Vertex* findVertex(const std::string& name);

    bool dijkstra(const std::string& start, const std::string& end);

    int getDistance() const;
    double getCost() const;
    double getTime() const;
    std::vector<std::string> getPath() const;

private:
    Vertex* vertices;

    int distanceResult;
    double costResult;
    double timeResult;
    std::vector<std::string> pathResult;

    Vertex* findOrCreateVertex(const std::string& name);
    void clearGraph();
};

#endif // GRAPH_H
