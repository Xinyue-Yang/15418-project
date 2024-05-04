#include <omp.h>

#include "network.h"
#include "utils.h"
#include "sequential/edmonds_karp.h"
#include "parallel1a/edmonds_karp.h"
#include "parallel1b/edmonds_karp.h"
#include "parallel1c/edmonds_karp.h"
#include "parallel1d/edmonds_karp.h"
#include "parallel2a/edmonds_karp.h"
#include "parallel2b/edmonds_karp.h"
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
    omp_set_num_threads(8);

    std::string input_file;
    std::string output_file;
    parse_args(argc, argv, input_file, output_file);

    Network network;
    input_network(input_file, network);

    run_algo(network, sequential::run_edmonds_karp);
    run_algo(network, parallel1a::run_edmonds_karp);
    run_algo(network, parallel1b::run_edmonds_karp);
    run_algo(network, parallel1c::run_edmonds_karp);
    run_algo(network, parallel1d::run_edmonds_karp);
    run_algo(network, parallel2a::run_edmonds_karp);
    run_algo(network, parallel2b::run_edmonds_karp);
    run_algo(network, sequential::run_dinics);
}
