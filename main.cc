#include "network.h"
#include "utils.h"
#include "sequential/edmonds_karp.h"
#include "sequential/dinics.h"

void run_algo(Network& network, const std::function<void(Network&)>& algo) {
    network.reset_flow();

    const auto compute_start{steady_clock::now()};

    algo(network);

    const auto compute_finish{steady_clock::now()};

    output_time(compute_start, compute_finish);
    output_maximum_flow(network);
}

int main(const int argc, char **const argv) {
    const auto init_start{steady_clock::now()};

    std::string input_file;
    std::string output_file;
    parse_args(argc, argv, input_file, output_file);

    Network network;
    input_network(input_file, network);

    run_algo(network, sequential::run_edmonds_karp);
    run_algo(network, sequential::run_dinics);
}
