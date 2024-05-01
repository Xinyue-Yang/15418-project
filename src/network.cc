#include "network.h"

Edge::Edge(int from, int to, int cap, int flow)
: from{from}, to{to}, cap{cap}, flow{flow} {}

std::istream& operator>>(std::istream& sin, Network& network) {
    auto& [num_verts, source, sink, num_edges, edges, adj]{network};

    if (sin >> num_verts >> source >> sink >> num_edges; not (
        num_verts >= 0
        and 0 <= source and source < num_verts
        and 0 <= sink and sink < num_verts
        and source != sink
        and num_edges >= 0
    ))
        throw std::runtime_error("invalid network");

    edges.clear();
    edges.reserve(num_edges * 2);
    adj.clear();
    adj.resize(num_verts);
    for (int i{}; i < num_edges; ++i) {
        int from;
        int to;
        int cap;
        int flow;
        if (sin >> from >> to >> cap >> flow; not (
            0 <= from and from < num_verts
            and 0 <= to and to < num_verts
            and cap >= 0
            and 0 <= flow and flow <= cap
        ))
            throw std::runtime_error("invalid network");

        edges.emplace_back(from, to, cap, flow);
        edges.emplace_back(to, from, cap, cap - flow);
        adj[from].emplace_back(2 * i);
        adj[to].emplace_back(2 * i + 1);
    }

    network.evaluate_flow();

    return sin;
}

std::ostream& operator<<(std::ostream& sout, const Network& network) {
    const auto& [num_verts, source, sink, num_edges, edges, adj]{network};

    sout << num_verts << ' ' << source << ' ' << sink << ' ' << num_edges
        << '\n';

    for (int i{}; i < num_edges; ++i) {
        const auto& [from, to, cap, flow]{edges[2 * i]};
        sout << from << ' ' << to << ' ' << cap << ' ' << flow << '\n';
    }

    return sout;
}

void Network::reset_flow() {
    for (int i{}; i < num_edges; ++i) {
        edges[2 * i].flow = 0;
        edges[2 * i + 1].flow = edges[2 * i + 1].cap;
    }
}

long long Network::evaluate_flow() const {
    for (int i{}; i < num_edges; ++i)
        if (auto cap{edges[2 * i].cap},
            flow{edges[2 * i].flow},
            resid{edges[2 * i + 1].flow};
            not (0 <= flow and flow <= cap and flow + resid == cap))
            throw std::runtime_error(
                "invalid flow: cap constraint violated or bad residual flow"
            );

    std::vector<long long> flow_io(num_verts);
    for (int i{}; i < num_edges; ++i) {
        const auto& [from, to, cap, flow]{edges[2 * i]};
        flow_io[from] -= flow;
        flow_io[to] += flow;
    }

    for (int u{}; u < num_verts; ++u)
        if (not (u == source or u == sink or flow_io[u] == 0ll))
            throw std::runtime_error(
                "invalid flow: conservation constraint violated"
            );

    return -flow_io[source];
}
