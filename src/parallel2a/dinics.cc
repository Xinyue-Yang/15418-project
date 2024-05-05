#include "parallel2a/dinics.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto INF{std::numeric_limits<int>::max()};
}

namespace parallel2a {
    void run_dinics(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<int> dist(num_verts);

        bool empty_frontier{};
        std::vector<int> frontier(num_verts);
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
            const auto child_dist{dist[u] + 1};

            int flow_out{};
            for (auto& i{curr[u]}; i < degree; ++i) {
                const auto j{adj[u][i]};
                if (auto& [from, to, cap, flow]{edges[j]};
                    flow < cap and dist[to] == child_dist) {
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
                dist[u] = NONE;

            dist[source] = 0;
            empty_frontier = false;
            frontier[source] = true;

            for (auto dist_v{1}; not empty_frontier; ++dist_v) {
                empty_frontier = true;

#pragma omp parallel for default(none) shared(num_verts, edges, adj, dist, \
empty_frontier, frontier, new_frontier, dist_v)
                for (int v = 0; v < num_verts; ++v)
                    if (new_frontier[v] = false; dist[v] == NONE)
                        for (const auto i: adj[v])
                            if (const auto& [from, to, cap, flow]{edges[i]};
                                frontier[to] and flow > 0) {
                                dist[v] = dist_v;
                                empty_frontier = false;
                                new_frontier[v] = true;
                                break;
                            }

                std::swap(frontier, new_frontier);
            }

            if (dist[sink] == NONE)
                return;

            std::fill(std::begin(curr), std::end(curr), 0);
            while (push_flow(source, INF) > 0);
        }
    }
}
