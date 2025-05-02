#pragma once
#include <string>

struct Time
{
    int hour;
    int minute;

    static Time parse(const std::string& s);
    std::string toString() const;
    int toMinutes() const noexcept;

    bool operator< (const Time& o) const noexcept { return toMinutes() <  o.toMinutes(); }
    bool operator<=(const Time& o) const noexcept { return toMinutes() <= o.toMinutes(); }
    bool operator>=(const Time& o) const noexcept { return toMinutes() >= o.toMinutes(); }
    bool operator==(const Time& o) const noexcept { return hour==o.hour && minute==o.minute; }
};