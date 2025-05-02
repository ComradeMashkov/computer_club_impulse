#pragma once

#include <string>
#include "Time.hpp"

struct Table
{
    int id;
    bool occupied = false;
    std::string occupant;
    Time occupiedSince{};
    long long revenue = 0;
    int totalMinutes = 0;
};