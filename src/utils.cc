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
    if (not (num_verts >= 2 and num_edges >= 0 and max_cap >= 0))
        throw std::runtime_error("invalid network: bad specifications");

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dis_vert(0, num_verts - 1);
    std::uniform_int_distribution dis_cap(0, max_cap);

    Network network{
        num_verts, dis_vert(gen), dis_vert(gen), num_edges,
        {}, std::vector<std::vector<int>>(num_verts)
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

Network generate_grid_network(
    const int num_rows,
    const int num_cols,
    const int max_cap
) {
    if (not (num_rows > 0 and num_cols > 0 and max_cap >= 0))
        throw std::runtime_error("invalid network: bad specifications");

    const auto num_verts{num_rows * num_cols + 2};
    const auto num_edges{num_cols * num_cols * (num_rows - 1) + num_cols * 2};
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dis_cap(0, max_cap);

    std::vector<int> perm(num_verts);
    std::iota(std::begin(perm), std::end(perm), 0);
    std::shuffle(std::begin(perm), std::end(perm), gen);

    const auto source{perm[num_verts - 2]};
    const auto sink{perm[num_verts - 1]};

    Network network{
        num_verts, source, sink, num_edges,
        {}, std::vector<std::vector<int>>(num_verts)
    };

    network.edges.reserve(num_edges * 2);
    for (int i{}; i < num_rows - 1; ++i)
        for (int j{}; j < num_cols; ++j)
            for (int k{}; k < num_cols; ++k) {
                const auto from{perm[num_cols * i + j]};
                const auto to{perm[num_cols * (i + 1) + k]};
                const auto cap{dis_cap(gen)};

                network.adj[from].emplace_back(std::size(network.edges));
                network.edges.emplace_back(from, to, cap, 0);
                network.adj[to].emplace_back(std::size(network.edges));
                network.edges.emplace_back(to, from, cap, cap);
            }
    for (int j{}; j < num_cols; ++j) {
        const auto from{source};
        const auto to{perm[j]};
        const auto cap{dis_cap(gen)};

        network.adj[from].emplace_back(std::size(network.edges));
        network.edges.emplace_back(from, to, cap, 0);
        network.adj[to].emplace_back(std::size(network.edges));
        network.edges.emplace_back(to, from, cap, cap);
    }
    for (int j{}; j < num_cols; ++j) {
        const auto from{perm[num_cols * (num_rows - 1) + j]};
        const auto to{sink};
        const auto cap{dis_cap(gen)};

        network.adj[from].emplace_back(std::size(network.edges));
        network.edges.emplace_back(from, to, cap, 0);
        network.adj[to].emplace_back(std::size(network.edges));
        network.edges.emplace_back(to, from, cap, cap);
    }

    return network;
}
