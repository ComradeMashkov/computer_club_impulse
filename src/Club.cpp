#include "Club.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

Club::Club(int n, const Time& o, const Time& c, int r, const std::vector<Event>& evs)
    : numTables(n)
    , openTime(o)
    , closeTime(c)
    , hourlyRate(r)
    , events(std::move(evs))
{
    tables.reserve(numTables);
    for (int i = 1; i <= numTables; ++i)
    {
        tables.emplace_back(Table{i, false, "", {}, 0, 0});
    }
}

void Club::outEvent(const Time& t, int id, const std::vector<std::string>& args)
{
    std::cout << t.toString() << ' ' << id;
    for (const auto& a : args)
    {
        std::cout << ' ' << a;
    }
    std::cout << "\n";
}

void Club::finalizeSession(const std::string& name, const Time& t, int tableID)
{
    auto& tbl = tables[tableID - 1];
    int used = t.toMinutes() - clients[name].seatStart.toMinutes();
    tbl.totalMinutes += used;
    int hours = (used + 59) / 60;
    tbl.revenue += 1LL * hours * hourlyRate;
    tbl.occupied = false;
    clients[name].seated = false;
}

void Club::assignFromQueue(const Time& t, int tableID)
{
    if (waiting.empty()) return;
    std::string next = waiting.front(); waiting.pop();
    auto& cl = clients[next];
    cl.seated = true;
    cl.tableID = tableID;
    cl.seatStart = t;
    auto& tbl = tables[tableID - 1];
    tbl.occupied = true;
    tbl.occupant = next;
    tbl.occupiedSince = t;
    outEvent(t, 12, {next, std::to_string(tableID)});
}

void Club::handle(const Event& e)
{
    // 1 - приходим
    if (e.id == 1)
    {
        outEvent(e.time, 1, {e.args[0]});
        auto& cl = clients[e.args[0]];
        if (cl.inside)
        {
            outEvent(e.time, 13, {"YouShallNotPass"});
        }
        else if (e.time < openTime || e.time >= closeTime)
        {
            outEvent(e.time, 13, {"NotOpenYet"});
        }
        else
        {
            cl = Client{e.args[0], true, false, -1, {}};
        }
    }

    // 2 - садимся
    else if (e.id == 2)
    {
        const auto& name = e.args[0];
        int tableID = std::stoi(e.args[1]);

        outEvent(e.time, 2, {name, std::to_string(tableID)});

        auto it = clients.find(name);
        if (it == clients.end() || !it->second.inside)
        {
            outEvent(e.time, 13, {"ClientUnknown"});
            return;
        }

        auto& cl = it->second;
        if (tableID < 1 || tableID > numTables || tables[tableID - 1].occupied)
        {
            outEvent(e.time, 13, {"PlaceIsBusy"});
            return;
        }
        
        if (cl.seated)
        {
            finalizeSession(name, e.time, cl.tableID);
        }

        cl.seated = true;
        cl.tableID = tableID;
        cl.seatStart = e.time;
        auto& tbl = tables[tableID - 1];
        tbl.occupied = true;
        tbl.occupant = name;
        tbl.occupiedSince = e.time;
    }

    // 3 - ждем
    else if (e.id == 3)
    {
        outEvent(e.time, 3, {e.args[0]});
        bool anyFree = std::any_of(tables.begin(), tables.end(), [] (auto& t) { return !t.occupied; });

        if (anyFree)
        {
            outEvent(e.time, 13, {"ICanWaitNoLonger!"});
        }
        else if (static_cast<int>(waiting.size()) >= numTables)
        {
            outEvent(e.time, 11, {e.args[0]});
        }
        else
        {
            waiting.push(e.args[0]);
        }
    }

    // 4 - уходим
    else if (e.id == 4)
    {
        const auto& name = e.args[0];
        outEvent(e.time, 4, {name});

        auto it = clients.find(name);
        if (it == clients.end() || !it->second.inside)
        {
            outEvent(e.time, 13, {"ClientUnknown"});
            return;
        }

        auto& cl = it->second;
        if (cl.seated)
        {
            finalizeSession(name, e.time, cl.tableID);
            assignFromQueue(e.time, cl.tableID);
        }
        cl.inside = false;
    }
}

void Club::process()
{
    std::cout << openTime.toString() << "\n";

    for (const auto& e : events)
    {
        handle(e);
    }
    
    for (const auto& [name, cl] : clients)
    {
        if (cl.inside && cl.seated)
        {
            finalizeSession(name, closeTime, cl.tableID);
        }
    }

    std::vector<std::string> remaining;
    for (const auto& [name, cl] : clients)
    {
        if (cl.inside) remaining.push_back(name);
    }

    std::sort(remaining.begin(), remaining.end());

    for (const auto& name : remaining)
    {
        outEvent(closeTime, 11, {name});
    }

    std::cout << closeTime.toString() << "\n";

    for (const auto& tbl : tables)
    {
        int h = tbl.totalMinutes / 60;
        int m = tbl.totalMinutes % 60;
        char buf[6];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
        std::cout << tbl.id << ' '
                  << tbl.revenue << ' '
                  << buf << "\n";
    }
}