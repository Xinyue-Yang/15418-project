#include "parallel1d/dinics.h"

namespace {
    constexpr auto RELAXED{std::memory_order_relaxed};
    constexpr auto NONE{-1};
    constexpr auto INF{std::numeric_limits<int>::max()};
}

namespace parallel1d {
    void run_dinics(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<std::atomic<int>> dist(num_verts);

        int frontier_size{};
        std::vector<int> frontier(num_verts);
        std::atomic<int> new_frontier_size{};
        std::vector<int> new_frontier(num_verts);

        std::vector<int> curr(num_verts);

        const std::function<int(int, int)>
        push_flow{[&push_flow, &sink, &edges, &adj, &dist, &curr](
            const int u,
            const int flow_in
        ) -> int {
            if (u == sink)
                return flow_in;

            const auto degree{static_cast<int>(std::size(adj[u]))};
            const auto child_dist{dist[u].load(RELAXED) + 1};

            int flow_out{};
            for (auto& i{curr[u]}; i < degree; ++i) {
                const auto j{adj[u][i]};
                if (auto& [from, to, cap, flow]{edges[j]};
                    flow < cap and dist[to].load(RELAXED) == child_dist) {
                    const auto flow_pushed{
                        push_flow(to, std::min(flow_in - flow_out, cap - flow))
                    };

                    flow += flow_pushed;
                    edges[j ^ 1].flow -= flow_pushed;

                    if ((flow_out += flow_pushed) == flow_in) {
                        i += static_cast<int>(flow == cap);
                        return flow_out;
                    }
                }
            }

            return flow_out;
        }};

        while (true) {
#pragma omp parallel for default(none) shared(num_verts, dist)
            for (int u = 0; u < num_verts; ++u)
                dist[u].store(NONE, RELAXED);

            dist[source].store(0, RELAXED);
            frontier[frontier_size++] = source;

            while (frontier_size > 0) {
#pragma omp parallel for default(none) shared(adj, edges, dist, \
frontier_size, frontier, new_frontier_size, new_frontier)
                for (int i = 0; i < frontier_size; ++i) {
                    const auto u{frontier[i]};
                    const auto dist_v{dist[u].load(RELAXED) + 1};
                    for (const auto j: adj[u])
                        if (const auto& [from, to, cap, flow]{edges[j]};
                            flow < cap and dist[to].load(RELAXED) == NONE)
                            if (auto none{NONE};
                                dist[to].compare_exchange_strong(
                                    none, dist_v, RELAXED, RELAXED
                                ))
                                new_frontier[
                                    new_frontier_size.fetch_add(1, RELAXED)
                                ] = to;
                }

                frontier_size = new_frontier_size.load(RELAXED);
                new_frontier_size.store(0, RELAXED);
                std::swap(frontier, new_frontier);
            }

            if (dist[sink].load(RELAXED) == NONE)
                return;

            std::fill(std::begin(curr), std::end(curr), 0);
            while (push_flow(source, INF) > 0);
        }
    }
}
