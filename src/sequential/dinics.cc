#include "sequential/dinics.h"

namespace sequential {
    void run_dinics(Network& network) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        constexpr auto inf{std::numeric_limits<int>::max()};
        std::vector<int> dist(num_verts);
        std::queue<int> queue{};
        std::vector<int> curr(num_verts);

        std::function<long long(int, long long)>
        dfs{[&dfs, &network, &dist, &curr](
            const int u,
            const long long flow_in
        ) -> int {
            if (u == network.sink)
                return flow_in;

            const auto degree{static_cast<int>(std::size(network.adj[u]))};
            const auto child_dist{dist[u] + 1};

            long long flow_out{};
            for (auto& i{curr[u]}; i < degree; ++i) {
                const auto j{network.adj[u][i]};
                if (auto& [from, to, cap, flow]{network.edges[j]};
                    flow < cap and dist[to] == child_dist) {
                    const auto flow_pushed{static_cast<int>(dfs(to, std::min(
                        flow_in - flow_out,
                        static_cast<long long>(cap - flow)
                    )))};

                    flow += flow_pushed;
                    network.edges[j ^ 1].flow -= flow_pushed;

                    if ((flow_out += flow_pushed) == flow_in) {
                        i += static_cast<int>(flow == cap);
                        return flow_out;
                    }
                }
            }

            return flow_out;
        }};

        while (true) {
            std::fill(std::begin(dist), std::end(dist), inf);
            dist[source] = 0;
            queue.emplace(source);

            while (not std::empty(queue)) {
                const auto u{queue.front()};
                queue.pop();

                const auto child_dist{dist[u] + 1};
                for (const auto i: adj[u])
                    if (const auto& [from, to, cap, flow]{edges[i]};
                        flow < cap and dist[to] == inf) {
                        dist[to] = child_dist;
                        queue.push(to);
                    }
            }

            if (dist[sink] == inf)
                return;

            std::fill(std::begin(curr), std::end(curr), 0);

            while (dfs(source, inf) > 0ll);
        }
    }
};
