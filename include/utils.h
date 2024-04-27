#pragma once

#include <unistd.h>

#include <chrono>
#include <iostream>
#include <fstream>

#include "network.h"

using std::chrono::steady_clock, std::chrono::milliseconds;

constexpr auto ARGS{
    "-n network_name"
};

void parse_args(
    int argc,
    char **argv,
    std::string& input_file,
    std::string& output_file
);

void input_network(const std::string& input_file, Network& network);

void output_time(
    const steady_clock::time_point& init_start,
    const steady_clock::time_point& compute_start,
    const steady_clock::time_point& compute_finish
);

void output_network(const std::string& output_file, const Network& network);
