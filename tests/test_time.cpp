#include <gtest/gtest.h>
#include "../src/Time.hpp"

TEST(TimeParse, Valid) {
    auto t = Time::parse("09:05");
    EXPECT_EQ(t.hour, 9);
    EXPECT_EQ(t.minute, 5);
}

TEST(TimeParse, InvalidFormat) {
    EXPECT_THROW(Time::parse("9:5"), std::invalid_argument);
}

TEST(TimeCompare, LessThan) {
    auto a = Time{8, 30};
    auto b = Time{9, 0};
    EXPECT_TRUE(a < b);
}
