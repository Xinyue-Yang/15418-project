#pragma once

#include <unistd.h>

#include <iostream>
#include <fstream>

#include "network.h"

constexpr auto ARGS{
    "-n network_name"
};

void parse_args(
    const int argc,
    char **const argv,
    std::string& input_file,
    std::string& output_file
);

void input_network(const std::string& input_file, Network& network);
