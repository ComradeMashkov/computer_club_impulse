#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string exec(const std::string& cmd) 
{
    std::array<char, 256> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

static std::string slurp(const std::string& path)
{
    std::ifstream in(path);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

static std::string normalize(const std::string& s)
{
    auto t = s;
    while (!t.empty() && (t.back() == '\n' || t.back() == '\r'))
        t.pop_back();
    return t;
}

struct E2ETest : testing::TestWithParam<std::string> {};

TEST_P(E2ETest, MatchesExpectedOutput)
{
    const auto name = GetParam();
    const std::string inp = "tests/" + name + "_in.txt";
    const std::string key = "tests/" + name + "_out.txt";

    // запускаем программу
    std::string cmd = std::string("../task ") + inp;
    std::string out = exec(cmd);

    // читаем ключи
    std::string expected = slurp(key);

    // сравниваем
    EXPECT_EQ(normalize(out), normalize(expected))
        << "Mismatch in test case \"" << name << "\"";
}

INSTANTIATE_TEST_SUITE_P(
    AllIntegrationTests,
    E2ETest,
    ::testing::Values(
        "test1",
        "test2",
        "test3"
    )
);
