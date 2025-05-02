#pragma once

#include "Time.hpp"
#include "Event.hpp"
#include "Client.hpp"
#include "Table.hpp"

#include <vector>
#include <map>
#include <queue>
#include <string>

class Club
{
public:
    Club(int numTables, const Time& openT, const Time& closeT, int rate, const std::vector<Event>& evs);

    void process();

private:
    int numTables;
    Time openTime;
    Time closeTime;
    int hourlyRate;
    std::vector<Event> events;

    std::map<std::string, Client> clients;
    std::vector<Table> tables;
    std::queue<std::string> waiting;

    void handle(const Event& e);
    void outEvent(const Time& t, int id, const std::vector<std::string>& args);
    void finalizeSession(const std::string& name, const Time& t, int tableID);
    void assignFromQueue(const Time& t, int tableID);
};