#include "parallel1/edmonds_karp.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto DUMMY{-2};
    constexpr auto INF{std::numeric_limits<int>::max()};

    void build_layers(
        const Network& network,
        std::vector<std::atomic<int>>& edge_in, std::vector<int>& flow_in
    ) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto adj{network.adj};
        const auto edges{network.edges};

        std::fill(std::begin(edge_in), std::end(edge_in), NONE);

        int frontier_size{};
        std::vector<int> frontier(num_verts);
        std::atomic<int> new_frontier_size{};
        std::vector<int> new_frontier(num_verts);

        edge_in[source] = DUMMY;
        flow_in[source] = INF;
        frontier[frontier_size++] = source;

        while (frontier_size > 0) {
#pragma omp parallel for default(none) shared(adj, edges, edge_in, flow_in, \
frontier_size, frontier, new_frontier_size, new_frontier)
            for (int i = 0; i < frontier_size; ++i)
                for (const auto j: adj[frontier[i]])
                    if (const auto& [from, to, cap, flow]{edges[j]}; flow < cap)
                        if (auto expected{NONE};
                            edge_in[to].compare_exchange_strong(expected, j)) {
                            flow_in[to] = std::min(
                                flow_in[frontier[i]],
                                cap - flow
                            );
                            new_frontier[new_frontier_size++] = to;
                        }
            frontier_size = new_frontier_size;
            new_frontier_size = 0;
            std::swap(frontier, new_frontier);
        }
    }

    void push_flow(
        Network& network,
        const std::vector<std::atomic<int>>& edge_in, const int flow_pushed
    ) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        for (auto u{sink}; edge_in[u] != DUMMY; u = edges[edge_in[u]].from) {
            edges[edge_in[u]].flow += flow_pushed;
            edges[edge_in[u] ^ 1].flow -= flow_pushed;
        }
    }
};

namespace parallel1 {
    void run_edmonds_karp(Network& network) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        std::vector<std::atomic<int>> edge_in(num_verts);
        std::vector<int> flow_in(num_verts);

        while (true) {
            if (build_layers(network, edge_in, flow_in); edge_in[sink] == NONE)
                return;
            push_flow(network, edge_in, flow_in[sink]);
        }
    }
};
