#pragma once

#include <fstream>
#include <limits>
#include <random>

#include "network.h"

Network parse_network(const std::string& file);

Network generate_random_network(
    int num_verts,
    int num_edges,
    int max_cap = std::numeric_limits<int>::max()
);

Network generate_grid_network(
    int num_rows,
    int num_cols,
    int max_cap = std::numeric_limits<int>::max()
);
