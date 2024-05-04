#include "sequential/dinics.h"

namespace {
    constexpr auto INF{std::numeric_limits<int>::max()};

    void build_layers(const Network& network, std::vector<int>& dist) {
        const auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        std::fill(std::begin(dist), std::end(dist), INF);
        std::queue<int> queue{};

        dist[source] = 0;
        queue.emplace(source);

        while (not std::empty(queue)) {
            const auto u{queue.front()};
            queue.pop();

            const auto child_dist{dist[u] + 1};
            for (const auto i: adj[u])
                if (const auto& [from, to, cap, flow]{edges[i]};
                    flow < cap and dist[to] == INF) {
                    dist[to] = child_dist;
                    queue.push(to);
                }
        }
    }

    long long push_flow(
        Network& network, std::vector<int>& dist, std::vector<int>& curr,
        const int u, const long long flow_in
    ) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        if (u == sink)
            return static_cast<int>(flow_in);

        const auto degree{static_cast<int>(std::size(adj[u]))};
        const auto child_dist{dist[u] + 1};

        long long flow_out{};
        for (auto& i{curr[u]}; i < degree; ++i) {
            const auto j{adj[u][i]};
            if (auto& [from, to, cap, flow]{edges[j]};
                flow < cap and dist[to] == child_dist) {
                const auto flow_pushed{static_cast<int>(push_flow(
                    network, dist, curr,
                    to, std::min(
                        flow_in - flow_out,
                        static_cast<long long>(cap - flow)
                    )
                ))};

                flow += flow_pushed;
                edges[j ^ 1].flow -= flow_pushed;

                if ((flow_out += flow_pushed) == flow_in) {
                    i += static_cast<int>(flow == cap);
                    return static_cast<int>(flow_out);
                }
            }
        }

        return static_cast<int>(flow_out);
    }
}

namespace sequential {
    void run_dinics(Network& network) {
        auto& [num_verts, source, sink, num_edges, edges, adj]{network};

        std::vector<int> dist(num_verts);
        std::vector<int> curr(num_verts);

        while (true) {
            if (build_layers(network, dist); dist[sink] == INF)
                return;
            std::fill(std::begin(curr), std::end(curr), 0);
            while (push_flow(network, dist, curr, source, INF) > 0ll);
        }
    }
}
