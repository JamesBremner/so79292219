#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct shift
{
    int start;
    int end;
};
struct record
{
    int in;
    int out;
    std::string name;
};
struct event
{
    int time;
    std::string name;

    event(int t)
        : time(t)
    {
    }
    event(int t, const std::string &n)
        : time(t),
          name(n)
    {
    }
};

std::vector<shift> vshift;
std::vector<record> vrecord;
std::vector<event> vevent;

void gen1()
{
    vshift = {
        {900, 1000},
        {1000, 1100},
        {1100, 1200}};

    vrecord = {
        {910, 930, "E1"},
        {920, 1010, "E2"},
        {1010, 1100, "E3"}};
}

void makeEvents(const shift &s)
{
    vevent.emplace_back(s.start);
    vevent.emplace_back(s.end);
}
void makeEvents(const record &r)
{
    vevent.emplace_back(r.in, "in__" + r.name);
    vevent.emplace_back(r.out, "out_" + r.name);
}
void makeEvents()
{
    for (auto &s : vshift)
        makeEvents(s);
    for (auto &r : vrecord)
        makeEvents(r);
}

void makeChunk(
    std::vector<std::string> &vpresent,
    event &s,
    event &e)
{
    // std::cout << "(( " << s.time << " " << s.name << " , " << e.time << " " << e.name << "))\n";

    if (s.time != e.time)
        std::cout << s.time << " to " << e.time << " ";

    // check if anyone arrived or left
    if (!s.name.empty())
    {
        if (s.name.find("in") != -1) {
            // arrival
            vpresent.push_back(s.name.substr(4));
        }
        else
        {
            // departure
            auto it = std::find(
                vpresent.begin(), vpresent.end(),
                s.name.substr(4));
            if (it != vpresent.end())
                vpresent.erase(it);
        }
    }

    if (s.time != e.time)
    {
        for (auto &p : vpresent)
            std::cout << p << " ";
        std::cout << "\n";
    }
}

void makeChunks()
{
    // keep track of who is present
    std::vector<std::string> vpresent;

    // create 'chunks' from each pair of events
    for (int k = 0; k < vevent.size() - 1; k++)
        makeChunk(vpresent, vevent[k], vevent[k + 1]);
}

main()
{
    // generate example problem
    gen1();

    // make all the events
    makeEvents();

    // sort events by time
    std::sort(
        vevent.begin(), vevent.end(),
        [](const event &a, const event &b)
        {
            return a.time < b.time;
        });

    // make and display the 'chunks'
    makeChunks();

    return 0;
}
