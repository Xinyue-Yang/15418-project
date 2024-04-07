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
                throw std::runtime_error("invalid args");
            }
        }

    if (std::empty(network_name)) {
        std::cerr << "Usage: " << argv[0] << ' ' << ARGS << std::endl;
        throw std::runtime_error("invalid args");
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
    if (std::ifstream sin{input_file}; sin)
        sin >> network;
    else
        throw std::runtime_error("invalid input file");
}

void output_time(
    const steady_clock::time_point &init_start,
    const steady_clock::time_point &compute_start,
    const steady_clock::time_point &compute_finish
) {
    std::cout << "[time]" << std::endl;
    std::cout << "init_time = "
        << duration_cast<milliseconds>(compute_start - init_start).count()
        << std::endl;
    std::cout << "compute_time = "
        << duration_cast<milliseconds>(compute_finish - compute_start).count()
        << std::endl;
    std::cout << std::endl;
}

void output_network(const std::string& output_file, const Network& network) {
    std::cout << "[maximum_flow]" << std::endl;
    std::cout << "value = " << network.evaluate_flow() << std::endl;
    std::cout << std::endl;

    if (std::ofstream sout{output_file}; sout)
        sout << network;
    else
        throw std::runtime_error("invalid output file");
}
