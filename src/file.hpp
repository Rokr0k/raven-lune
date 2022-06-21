#pragma once

#include <vector>
#include <future>
#include "bmsparser/bmsparser.hpp"

namespace rl
{
    namespace file
    {
        extern std::vector<bms::Chart*> charts;
        extern std::future<void> loading;

        void initialise();

        void release();
    }
}