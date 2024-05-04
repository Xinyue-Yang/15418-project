#pragma once

#include <atomic>
#include <limits>

#include "network.h"

namespace parallel2 {
    void run_edmonds_karp(Network& network);
};
