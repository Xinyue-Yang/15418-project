#pragma once

#include <atomic>
#include <limits>

#include <omp.h>

#include "network.h"

namespace parallel1d {
    void run_edmonds_karp(Network& network);
}
