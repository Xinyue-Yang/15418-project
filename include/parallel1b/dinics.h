#pragma once

#include <functional>
#include <limits>

#include <omp.h>

#include "network.h"

namespace parallel1b {
    void run_dinics(Network& network);
}
