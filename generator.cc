#include <unistd.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

using std::chrono::steady_clock, std::chrono::milliseconds;

constexpr auto ARGS{
    "-n network_name "
    "-v num_verts "
    "-e num_edges "
    "[-c max_cap]"
};

int main(const int argc, char **const argv) {
    const auto start{steady_clock::now()};

    std::string network_name{};
    int num_verts{};
    int num_edges{};
    auto max_cap{std::numeric_limits<int>::max()};
    for (int opt; (opt = getopt(argc, argv, "n:v:e:c:")) != -1;)
        switch (opt) {
            case 'n': {
                network_name = optarg;
                break;
            }
            case 'v': {
                num_verts = std::stoi(optarg);
                break;
            }
            case 'e': {
                num_edges = std::stoi(optarg);
                break;
            }
            case 'c': {
                max_cap = std::stoi(optarg);
                break;
            }
            default: {
                std::cerr << "Usage: " << argv[0] << ' ' << ARGS << std::endl;
                throw std::runtime_error("invalid args");
            }
        }

    if (not (
        not std::empty(network_name) and num_verts >= 2 and num_edges >= 0
    )) {
        std::cerr << "Usage: " << argv[0] << ' ' << ARGS << std::endl;
        throw std::runtime_error("invalid args");
    }

    const auto input_file{"input/" + network_name + ".network"};

    std::cout << "[args]" << std::endl;
    std::cout << "network_name = \"" << network_name << "\"" << std::endl;
    std::cout << "input_file = \"" << input_file << "\"" << std::endl;
    std::cout << "num_verts = " << num_verts << std::endl;
    std::cout << "num_edges = " << num_edges << std::endl;
    std::cout << "max_cap = " << max_cap << std::endl;
    std::cout << std::endl;

    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution dis_vert(0, num_verts - 1);
    std::uniform_int_distribution dis_cap(0, max_cap);

    if (std::ofstream sout{input_file}; sout) {
        const auto source{dis_vert(gen)};
        int sink;
        do {
            sink = dis_vert(gen);
        } while (sink == source);

        sout << num_verts << ' ' << source << ' ' << sink << ' '
            << num_edges << '\n';

        for (int i{}; i < num_edges; ++i)
            sout << dis_vert(gen) << ' ' << dis_vert(gen) << ' ' << dis_cap(gen)
                << ' ' << 0 << '\n';
    } else
        throw std::runtime_error("invalid output file");

    const auto finish{steady_clock::now()};

    std::cout << "[time]" << std::endl;
    std::cout << "generate_time = " <<
        duration_cast<milliseconds>(finish - start).count() << std::endl;
    std::cout << std::endl;
}
