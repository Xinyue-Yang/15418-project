#pragma once

#include <istream>
#include <vector>

class Edge {
public:
    int from;
    int to;
    int cap;
    int flow;

    Edge(int from, int to, int cap, int flow);
};

class Network {
public:
    int num_verts;
    int source;
    int sink;
    int num_edges;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;

    friend std::istream& operator>>(std::istream& sin, Network& network);
    friend std::ostream& operator<<(std::ostream& sout, const Network& network);

    void reset_flow();

    long long evaluate_flow() const;
};
