#include "parallel1a/dinics.h"

namespace {
    constexpr auto NONE{-1};
    constexpr auto INF{std::numeric_limits<int>::max()};
}

namespace parallel1a {
    void run_dinics(Network& network) {
        const auto num_verts{network.num_verts};
        const auto source{network.source};
        const auto sink{network.sink};
        auto& edges{network.edges};
        const auto& adj{network.adj};

        std::vector<int> dist(num_verts);
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
            std::fill(std::begin(dist), std::end(dist), NONE);
            std::queue<int> queue{};

            dist[source] = 0;
            queue.emplace(source);

            while (not std::empty(queue)) {
                const auto u{queue.front()};
                queue.pop();

                const auto child_dist{dist[u] + 1};
                for (const auto i: adj[u])
                    if (const auto& [from, to, cap, flow]{edges[i]};
                        flow < cap and dist[to] == NONE) {
                        dist[to] = child_dist;
                        queue.push(to);
                    }
            }

            if (dist[sink] == NONE)
                return;

            std::fill(std::begin(curr), std::end(curr), 0);
            while (push_flow(source, INF) > 0);
        }
    }
}
