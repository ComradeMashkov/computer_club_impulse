#pragma once

#include <string>
#include "Time.hpp"

struct Client
{
    std::string name;
    bool inside = false;
    bool seated = false;
    int tableID = -1;
    Time seatStart;
};