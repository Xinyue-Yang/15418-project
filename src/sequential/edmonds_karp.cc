#include "sequential/edmonds_karp.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto DUMMY{-2};
    constexpr auto INF{std::numeric_limits<int>::max()};

    void build_layers(
        const Network& network,
        std::vector<int>& edge_in, std::vector<int>& flow_in
    ) {
        const auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        std::fill(std::begin(edge_in), std::end(edge_in), NONE);
        std::queue<int> queue{};

        edge_in[source] = DUMMY;
        flow_in[source] = INF;
        queue.emplace(source);

        while (not std::empty(queue)) {
            const auto u{queue.front()};
            queue.pop();

            for (const auto i: adj[u])
                if (const auto& [from, to, cap, flow]{edges[i]};
                    flow < cap and edge_in[to] == NONE) {
                    edge_in[to] = i;
                    flow_in[to] = std::min(flow_in[u], cap - flow);
                    queue.push(to);
                }
        }
    }

    void push_flow(
        Network& network,
        const std::vector<int>& edge_in, const int flow_pushed
    ) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        for (auto u{sink}; edge_in[u] != DUMMY; u = edges[edge_in[u]].from) {
            edges[edge_in[u]].flow += flow_pushed;
            edges[edge_in[u] ^ 1].flow -= flow_pushed;
        }
    }
}

namespace sequential {
    void run_edmonds_karp(Network& network) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        std::vector<int> edge_in(num_verts);
        std::vector<int> flow_in(num_verts);

        while (true) {
            if (build_layers(network, edge_in, flow_in); edge_in[sink] == NONE)
                return;
            push_flow(network, edge_in, flow_in[sink]);
        }
    }
}
