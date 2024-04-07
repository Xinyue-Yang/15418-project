#include "network.h"
#include "utils.h"
#include "sequential/dinics.h"

int main(const int argc, char **const argv) {
    const auto init_start{steady_clock::now()};

    std::string input_file;
    std::string output_file;
    parse_args(argc, argv, input_file, output_file);

    Network network;
    input_network(input_file, network);

    const auto compute_start{steady_clock::now()};

    sequential::run_dinics(network);

    const auto compute_finish{steady_clock::now()};

    output_time(init_start, compute_start, compute_finish);
    output_network(output_file, network);
}
