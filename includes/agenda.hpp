#ifndef __AGENDA_HPP__
#define __AGENDA_HPP__

#include<string>
#include<vector>

#include"event.hpp"

namespace agenda{

    // structure d'un agenda
    struct Agenda{
        std::string id, title, description;
        std::vector<event::Event> events;
    };

    Agenda createAgenda();
    void rmAgenda(const Agenda& ag);
    void saveAgenda(const Agenda& ag);
    void showAgenda(const Agenda& ag);
    void saveAgendaHTML(const Agenda& ag);
    bool isAgDirEmpty();
    void emptyAgDir();
    Agenda chooseAgenda();
}// namespace agenda

#endif