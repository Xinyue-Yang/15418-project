#include "network.h"
#include "utils.h"

int main(const int argc, char **const argv) {
    std::string input_file;
    std::string output_file;
    parse_args(argc, argv, input_file, output_file);

    Network network;
    input_network(input_file, network);
}
