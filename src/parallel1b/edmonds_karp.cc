#include "parallel1b/edmonds_karp.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto DUMMY{-2};
    constexpr auto INF{std::numeric_limits<int>::max()};
};

namespace parallel1b {
    void run_edmonds_karp(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<int> edge_in(num_verts);
        std::vector<int> flow_in(num_verts);

        int frontier_size{};
        std::vector<int> frontier(num_verts);
        int new_frontier_size{};
        std::vector<int> new_frontier(num_verts);

        std::vector<omp_lock_t> lock(num_verts);
#pragma omp parallel for default(none) shared(num_verts, lock)
        for (int i = 0; i < num_verts; ++i)
            omp_init_lock(&lock[i]);

        while (true) {
#pragma omp parallel for default(none) shared(num_verts, edge_in)
            for (int u = 0; u < num_verts; ++u)
                edge_in[u] = NONE;

            edge_in[source] = DUMMY;
            flow_in[source] = INF;
            frontier[frontier_size++] = source;

            while (frontier_size > 0) {
#pragma omp parallel for default(none) shared(adj, edges, edge_in, flow_in, \
frontier_size, frontier, new_frontier_size, new_frontier, lock)
                for (int i = 0; i < frontier_size; ++i) {
                    const auto u{frontier[i]};
                    const auto flow_in_u{flow_in[u]};
                    for (const auto j: adj[u])
                        if (const auto& [from, to, cap, flow]{edges[j]};
                            flow < cap
                            and edge_in[to] == NONE
                            and omp_test_lock(&lock[to])) {
                            if (edge_in[to] == NONE) {
                                edge_in[to] = j;
                                flow_in[to] = std::min(flow_in_u, cap - flow);
                                int index{};
#pragma omp atomic capture
                                index = new_frontier_size++;
                                new_frontier[index] = to;
                            }
                            omp_unset_lock(&lock[to]);
                        }
                }

                frontier_size = new_frontier_size;
                new_frontier_size = 0;
                std::swap(frontier, new_frontier);
            }

            if (edge_in[sink] == NONE)
                break;
            const auto flow_pushed{flow_in[sink]};

            for (int i{edge_in[sink]}; i != DUMMY; i = edge_in[edges[i].from]) {
                edges[i].flow += flow_pushed;
                edges[i ^ 1].flow -= flow_pushed;
            }
        }

#pragma omp parallel for default(none) shared(num_verts, lock)
        for (int i = 0; i < num_verts; ++i)
            omp_destroy_lock(&lock[i]);
    }
};
