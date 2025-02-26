#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include<string>
#include<vector>

#include"date.hpp"

namespace event{

    // structure d'un événement
    struct Event{
        std::string id, title, description;
        date::Date begin, end;
    };

    std::vector<Event> loadAllEvents(std::vector<std::string> v);
    void saveAllEvents(const std::vector<Event>& vEvent);
    void createEvent(std::vector<Event>& ag);
    void rmEvent(std::vector<Event>& ag);
    void rmNonExistEvent(const std::vector<Event> &vEvent);
}// namespace event

#endif