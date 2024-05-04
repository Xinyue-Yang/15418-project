#include "utils.h"

Network parse_network(const std::string& file) {
    Network network{};
    if (std::ifstream sin{file}; sin)
        sin >> network;
    else
        throw std::runtime_error("invalid input file");
    return network;
}

Network generate_random_network(
    const int num_verts,
    const int num_edges,
    const int max_cap
) {
    if (not (num_verts >= 2 and num_edges >= 0))
        throw std::runtime_error("invalid network: bad specifications");

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dis_vert(0, num_verts - 1);
    std::uniform_int_distribution dis_cap(0, max_cap);

    Network network{
        num_verts,
        dis_vert(gen),
        dis_vert(gen),
        num_edges,
        {},
        std::vector<std::vector<int>>(num_verts)
    };

    while (network.sink == network.source)
        network.sink = dis_vert(gen);

    network.edges.reserve(num_edges * 2);
    for (int i{}; i < num_edges; ++i) {
        const auto from{dis_vert(gen)};
        const auto to{dis_vert(gen)};
        const auto cap{dis_cap(gen)};

        network.edges.emplace_back(from, to, cap, 0);
        network.edges.emplace_back(to, from, cap, cap);
        network.adj[from].emplace_back(2 * i);
        network.adj[to].emplace_back(2 * i + 1);
    }

    return network;
}
