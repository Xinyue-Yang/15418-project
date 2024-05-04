#include <chrono>
#include <iostream>

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

using std::chrono::steady_clock, std::chrono::milliseconds;

void run_trial(
    Network& network,
    const std::function<void(Network&)>& algo,
    const std::string& name = "trial"
) {
    network.reset_flow();

    const auto start{steady_clock::now()};
    algo(network);
    const auto finish{steady_clock::now()};

    const auto max_flow{network.evaluate_flow()};
    const auto num_ms{duration_cast<milliseconds>(finish - start).count()};

    std::cout << "[" << name << "]" << std::endl;
    std::cout << "max_flow = " << max_flow << std::endl;
    std::cout << "num_ms = " << num_ms << std::endl;
    std::cout << std::endl;
}

int main(const int argc, char **const argv) {
    omp_set_num_threads(8);

    auto network{parse_network("input/diamond.network")};
    // auto network{generate_random_network(500, 250000)};

    run_trial(network, sequential::run_edmonds_karp, "seq_ek");
    run_trial(network, parallel1a::run_edmonds_karp, "par1a_ek");
    run_trial(network, parallel1b::run_edmonds_karp, "par1b_ek");
    run_trial(network, parallel1c::run_edmonds_karp, "par1c_ek");
    run_trial(network, parallel1d::run_edmonds_karp, "par1d_ek");
    run_trial(network, parallel2a::run_edmonds_karp, "par2a_ek");
    run_trial(network, parallel2b::run_edmonds_karp, "par2b_ek");

    run_trial(network, sequential::run_dinics, "seq_dn");
}
