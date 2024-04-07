#include "network.h"
#include "utils.h"
#include "sequential/dinics.h"

int main(const int argc, char **const argv) {
    std::string input_file;
    std::string output_file;
    parse_args(argc, argv, input_file, output_file);

    Network network;
    input_network(input_file, network);

    sequential::run_dinics(network);

    output_network(output_file, network);
}
