#include "network.h"

Edge::Edge(int from, int to, int cap, int flow)
: from{from}, to{to}, cap{cap}, flow{flow} {}

std::istream& operator>>(std::istream& sin, Network& network) {
    auto& [num_verts, source, sink, num_edges, edges, adj]{network};

    sin >> num_verts >> source >> sink >> num_edges;

    edges.clear();
    edges.reserve(num_edges * 2);
    adj.clear();
    adj.resize(num_verts);
    for (int i{}; i < num_edges; ++i) {
        int from;
        int to;
        int cap;
        sin >> from >> to >> cap;

        edges.emplace_back(from, to, cap, 0);
        edges.emplace_back(to, from, cap, cap);
        adj[from].emplace_back(2 * i);
        adj[to].emplace_back(2 * i + 1);
    }

    return sin;
}
