#include "parallel2a/edmonds_karp.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto DUMMY{-2};
    constexpr auto INF{std::numeric_limits<int>::max()};
}

namespace parallel2a {
    void run_edmonds_karp(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<int> edge_in(num_verts);
        std::vector<int> flow_in(num_verts);

        bool empty_frontier{};
        std::vector<bool> frontier(num_verts);
        std::vector<bool> new_frontier(num_verts);

        while (true) {
#pragma omp parallel for default(none) shared(num_verts, edge_in)
            for (int u = 0; u < num_verts; ++u)
                edge_in[u] = NONE;
#pragma omp parallel for default(none) shared(num_verts, frontier)
            for (int u = 0; u < num_verts; ++u)
                frontier[u] = false;
#pragma omp parallel for default(none) shared(num_verts, new_frontier)
            for (int u = 0; u < num_verts; ++u)
                new_frontier[u] = false;

            edge_in[source] = DUMMY;
            flow_in[source] = INF;
            empty_frontier = false;
            frontier[source] = true;

            while (not empty_frontier) {
                empty_frontier = true;
#pragma omp parallel for default(none) shared(num_verts, edges, adj, edge_in, \
flow_in, empty_frontier, frontier, new_frontier)
                for (int v = 0; v < num_verts; ++v)
                    if (new_frontier[v] = false; edge_in[v] == NONE)
                        for (const auto i: adj[v])
                            if (const auto& [from, to, cap, flow]{edges[i ^ 1]};
                                frontier[from] and flow < cap) {
                                edge_in[v] = i ^ 1;
                                flow_in[v] = std::min(flow_in[from], cap - flow);
                                empty_frontier = false;
                                new_frontier[v] = true;
                                break;
                            }

                std::swap(frontier, new_frontier);
            }

            if (edge_in[sink] == NONE)
                return;
            const auto flow_pushed{flow_in[sink]};

            for (int i{edge_in[sink]}; i != DUMMY; i = edge_in[edges[i].from]) {
                edges[i].flow += flow_pushed;
                edges[i ^ 1].flow -= flow_pushed;
            }
        }
    }
}
