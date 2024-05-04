#include "parallel2/edmonds_karp.h"

namespace {
    constexpr auto RELAXED{std::memory_order_relaxed};
    constexpr auto NONE{-1};
    constexpr auto DUMMY{-2};
    constexpr auto INF{std::numeric_limits<int>::max()};
};

namespace parallel2 {
    void run_edmonds_karp(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<std::atomic<int>> edge_in(num_verts);
        std::vector<int> flow_in(num_verts);

        int frontier_size{};
        std::vector<int> frontier(num_verts);
        std::atomic<int> new_frontier_size{};
        std::vector<int> new_frontier(num_verts);

        while (true) {
#pragma omp parallel for default(none) shared(num_verts, edge_in)
            for (int u = 0; u < num_verts; ++u)
                edge_in[u].store(NONE, RELAXED);

            edge_in[source].store(DUMMY, RELAXED);
            flow_in[source] = INF;
            frontier[frontier_size++] = source;

            while (frontier_size > 0) {
#pragma omp parallel for default(none) shared(adj, edges, edge_in, flow_in, \
frontier_size, frontier, new_frontier_size, new_frontier)
                for (int i = 0; i < frontier_size; ++i) {
                    const auto u{frontier[i]};
                    const auto flow_in_u{flow_in[u]};
                    for (const auto j: adj[u])
                        if (const auto& [from, to, cap, flow]{edges[j]};
                            flow < cap and edge_in[to].load(RELAXED) == NONE)
                            if (auto none{NONE};
                                edge_in[to].compare_exchange_strong(
                                    none, j, RELAXED, RELAXED
                                )) {
                                flow_in[to] = std::min(flow_in_u, cap - flow);
                                new_frontier[
                                    new_frontier_size.fetch_add(1, RELAXED)
                                ] = to;
                            }
                }

                frontier_size = new_frontier_size.load(RELAXED);
                new_frontier_size.store(0, RELAXED);
                std::swap(frontier, new_frontier);
            }

            if (edge_in[sink].load(RELAXED) == NONE)
                return;
            const auto flow_pushed{flow_in[sink]};

            for (
                int i{edge_in[sink].load(RELAXED)};
                i != DUMMY;
                i = edge_in[edges[i].from].load(RELAXED)
            ) {
                edges[i].flow += flow_pushed;
                edges[i ^ 1].flow -= flow_pushed;
            }
        }
    }
};
