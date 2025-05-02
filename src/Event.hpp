#pragma once

#include "Time.hpp"
#include <string>
#include <vector>

struct Event
{
    Time time;
    int id;
    std::vector<std::string> args;
};