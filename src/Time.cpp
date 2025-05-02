#include "Time.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Time Time::parse(const std::string& s)
{
    if (s.size() != 5 || s[2] != ':')
    {
        throw std::invalid_argument("Invalid time format");
    }

    int h = std::stoi(s.substr(0, 2));
    int m = std::stoi(s.substr(3, 2));
    if (h < 0 || h > 23 || m < 0 || m > 59)
    {
        throw std::invalid_argument("Invalid time value");
    }

    return {h, m};
}

std::string Time::toString() const
{
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour
        << ':' << std::setw(2) << std::setfill('0') << minute;
    return oss.str();
}

int Time::toMinutes() const noexcept
{
    return hour*60 + minute;
}