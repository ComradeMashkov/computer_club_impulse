#include "Club.hpp"
#include "Time.hpp"
#include "Event.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in)
    {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::string line;
    int lineNo = 0;
    
    try
    {
        std::getline(in, line); ++lineNo;
        std::istringstream iss1(line);
        int numTables;
        if (!(iss1 >> numTables) || !(iss1 >> std::ws).eof() || numTables <= 0)
        {
            throw line;
        }

        std::getline(in, line); ++lineNo;
        std::istringstream iss2(line);
        std::string openS, closeS;
        if (!(iss2 >> openS >> closeS) || !(iss2 >> std::ws).eof())
        {
            throw line;
        }
        auto openT = Time::parse(openS);
        auto closeT = Time::parse(closeS);

        std::getline(in, line); ++lineNo;
        std::istringstream iss3(line);
        int rate;
        if(!(iss3 >> rate) || !(iss3 >> std::ws).eof() || rate <= 0)
        {
            throw line;
        }

        std::vector<Event> events;
        Time prev{0, 0};
        while (std::getline(in, line))
        {
            if (line.empty()) continue;
            ++lineNo;
            std::istringstream is(line);
            std::string tstr; int id;
            if (!(is >> tstr >> id)) throw line;
            auto t = Time::parse(tstr);
            if (t < prev) throw line;
            prev = t;
            Event e{t, id, {}};
            if (id == 1 || id == 3 || id == 4)
            {
                std::string name;
                if (!(is >> name) || !(is >> std::ws).eof()) throw line;
                e.args.emplace_back(name);
            }
            else if (id == 2)
            {
                std::string name; int tableID;
                if (!(is >> name >> tableID) || !(is >> std::ws).eof()) throw line;
                if (tableID < 1 || tableID > numTables) throw line;
                e.args = {name, std::to_string(tableID)};
            }
            else throw line;

            events.emplace_back(e);
        }

        Club club(numTables, openT, closeT, rate, std::move(events));
        club.process();
    }
    catch(const std::string& badLine)
    {
        std::cout << badLine << "\n";
        return 0;
    }
    catch(...)
    {
        std::cout << line << "\n";
        return 0;
    }
    
    return 0;
}