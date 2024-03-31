#include "utils.h"

void parse_args(
    const int argc,
    char **const argv,
    std::string& input_file,
    std::string& output_file
) {
    std::string network_name;
    for (int opt; (opt = getopt(argc, argv, "n:")) != -1;)
        switch (opt) {
            case 'n': {
                network_name = optarg;
                break;
            }
            default: {
                std::cerr << "Usage: " << argv[0] << ' ' << ARGS << std::endl;
                throw std::runtime_error("invalid arguments");
            }
        }

    if (std::empty(network_name)) {
        std::cerr << "Usage: " << argv[0] << ' ' << ARGS << std::endl;
        throw std::runtime_error("invalid arguments");
    }

    input_file = "input/" + network_name + ".network";
    output_file = "output/" + network_name + ".flow";

    std::cout << "[args]" << std::endl;
    std::cout << "network_name = \"" << network_name << "\"" << std::endl;
    std::cout << "input_file = \"" << input_file << "\"" << std::endl;
    std::cout << "output_file = \"" << output_file << "\"" << std::endl;
    std::cout << std::endl;
}

void input_network(const std::string& input_file, Network& network) {
    if (not (std::ifstream{input_file} >> network))
        throw std::runtime_error("invalid input file");
}
