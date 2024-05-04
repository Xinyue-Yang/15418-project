#pragma once

#include <limits>

#include <omp.h>

#include "network.h"

namespace parallel4 {
    void run_edmonds_karp(Network& network);
};
