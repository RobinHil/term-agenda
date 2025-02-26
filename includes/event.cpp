#include <random>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "event.hpp"

namespace fs = std::filesystem;

namespace event{

    // créer un identifiant aléatoire pour un événement
    std::string getUID(std::size_t len){
        std::vector<char> x = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        std::string uid(len, '\0');
        static std::mt19937 gen(std::random_device{}());

        std::uniform_int_distribution < > dis(0, x.size()-1);
        for (char& c : uid){
            c = x[dis(gen)];
        }

        return uid;
    }

    // demander des informations à l'utilisateur
    void askUser(std::string& c, const std::string& str){
        std::cin.ignore();
        std::cout << str << " : ";
        std::getline(std::cin, c);
    }

    void askUser(int& c, const std::string& str){
        std::cout << str << " : ";
        std::string i;
        std::cin >> i;
        bool isDigit = true;
        for (char ch : i){
            isDigit = std::isdigit(ch);
            if (!isDigit)
                break;
        }
        while (!isDigit){
            std::cout << "Erreur : veuillez saisir un nombre." << std::endl;
            std::cout << str << " : ";
            std::cin >> i;
            for (char ch : i){
                isDigit = std::isdigit(ch);
                if (!isDigit)
                    break;            
            }
        }
        c = std::stoi(i);
        std::cout << std::endl;
    }

    void askUser(char& c, const std::string& str){
        std::cout << str << " : ";
        std::cin >> c;
        std::cout << std::endl;
    }

    // charger un événement depuis un fichier qui le contient
    Event loadEvent(const fs::path &path){

        Event ev;

        std::string str;
        std::ifstream ifs(path);
        std::vector<std::string> v;
        while (std::getline(ifs, str)){
            v.push_back(str);
        }

        ev.id = v[0];
        ev.title = v[1];
        ev.description = v[2];

        ev.begin.day = v[3];
        ev.begin.month = v[4];
        ev.begin.year = v[5];
        ev.begin.hour = v[6];
        ev.begin.min = v[7];

        ev.end.day = v[8];
        ev.end.month = v[9];
        ev.end.year = v[10];
        ev.end.hour = v[11];
        ev.end.min = v[12];

        return ev;
    }

    // charger tous les événements d'un vecteur depuis leurs fichiers contenant des identifiants d'event
    std::vector<Event> loadAllEvents(std::vector<std::string> v){

        std::vector<Event> vEvent;

        fs::path path = "agenda/events";
        fs::directory_iterator di(path);
        for (const fs::path &de : di){
            for (std::string eid : v){
                if (de.stem() == eid)
                    vEvent.push_back(loadEvent(de));
            }
        }
        return vEvent;
    }

    // demander à l'utilisateur de choisir un événement parmis ceux d'un vecteur
    Event chooseEvent(std::vector<Event> vEvents){

        std::string choice;

        std::cout << "---- Sélectionner un événement ----" << std::endl << std::endl;

        askUser(choice, "Saisissez le titre de l'événement à sélectionner (Q pour annuler)");
        std::cout << std::endl;

        Event ret;
        if (choice == "Q")
            return ret;

        std::vector<Event> allEvents;
        for (Event ev : vEvents){
            if (choice == ev.title)
                allEvents.push_back(ev);
        }

        if (allEvents.empty()){
            std::cout << "aucun événement" << std::endl << std::endl;
            return ret;
        } else{
            for (int i=0; i<(int)allEvents.size(); i++){
                std::cout
                    << "Événements n°" << i + 1 << " :" << std::endl
                    << "  Id de l'événement : " << allEvents[i].id << std::endl
                    << "    Titre : " << allEvents[i].title << std::endl
                    << "    Description : " << allEvents[i].description << std::endl
                    << "    Début : " << allEvents[i].begin.day << "/" << allEvents[i].begin.month << "/" << allEvents[i].begin.year << " " << allEvents[i].begin.hour << ":" << allEvents[i].begin.min << std::endl
                    << "    Fin : " << allEvents[i].end.day << "/" << allEvents[i].end.month << "/" << allEvents[i].end.year << " " << allEvents[i].end.hour << ":" << allEvents[i].end.min << std::endl
                    << std::endl;
            }
        }
        std::cin.putback('\n');

        int iChoice = 100000;

        askUser(iChoice, "Saisissez le numéro de l'événement à sélectionner (0 pour annuler)");

        while (iChoice<0 || iChoice>(int)allEvents.size()){
            std::cout << "Erreur : veuillez saisir un nombre valide." << std::endl;
            askUser(iChoice, "Saisissez le numéro de l'événement à sélectionner (0 pour annuler)");
        }
        
        if (iChoice == 0)
            return ret;

        char c = ' ';
        askUser(c, "Êtes-vous vraiment sûr de vouloir supprimer cet événement [o/n]");
        while (c != 'o' && c != 'n'){
            std::cout << "Erreur : veuillez saisir une valeur valide." << std::endl;
            askUser(c, "Êtes-vous vraiment sûr de vouloir supprimer cet événement [o/n]");
        }
        if (c == 'n')
            return ret;

        return allEvents[iChoice-1];
    }

    // demander à l'utilisateur les informations pour créer un événement
    void createEvent(std::vector<Event> &ag){

        Event ev;

        ev.id = getUID(80);

        std::cout << "---- Nouveau événement ----" << std::endl << std::endl;

        std::cout << "Saisissez un titre" << std::endl;
        askUser(ev.title, "Titre");
        std::cin.putback('\n');

        std::cout << std::endl << "Saisissez une description" << std::endl;
        askUser(ev.description, "Description");
        std::cin.putback('\n');

        date::askForDate(ev.begin, "de début");
        date::askForDate(ev.end, "de fin");

        ag.push_back(ev);
    }

    // sauvegarder un événement dans un fichier
    void saveEvent(const Event &ev){

        fs::path path = "agenda/events/" + ev.id + ".txt";

        std::ofstream ofs(path);
        ofs << ev.id << std::endl
            << ev.title << std::endl
            << ev.description << std::endl
            << ev.begin.day << std::endl
            << ev.begin.month << std::endl
            << ev.begin.year << std::endl
            << ev.begin.hour << std::endl
            << ev.begin.min << std::endl
            << ev.end.day << std::endl
            << ev.end.month << std::endl
            << ev.end.year << std::endl
            << ev.end.hour << std::endl
            << ev.end.min << std::endl;
    }

    // sauvegarder dans des fichiers tous les événements d'un vecteur
    void saveAllEvents(const std::vector<Event> &vEvent){
        
        for (int i = 0; i < (int)vEvent.size(); i++)
            saveEvent(vEvent[i]);
    }

    // supprimer les sauvegardes des événements qui ne sont présents dans aucun agenda
    void rmNonExistEvent(const std::vector<Event> &vEvent){

        bool save;
        fs::path path = "agenda/events";

        fs::directory_iterator di(path);
        for (fs::directory_entry de : di){
            path = de;
            for (Event E : vEvent){
                save = path.stem() == E.id;
                if (save)
                    break;
            }
            if (!save)
                remove(path);
        }
    }

    // supprimer un événement d'un vecteur
    void rmEvent(std::vector<Event> &ag){

        if (!ag.empty()){
            Event ev = chooseEvent(ag);

            for (int i = 0; i < (int)ag.size(); i++){
                if (ag[i].id == ev.id){
                    std::vector<Event>::iterator it = ag.begin() + i;
                    ag.erase(it);
                }
            }

            std::cout << "Événement supprimé avec succès !" << std::endl << std::endl;
        } else if (ag.empty()){
            std::cout
            << "Erreur : aucun événement à supprimer." << std::endl;
            char c = ' ';
            askUser(c, "Voulez-vous créer un nouveau événement [o/n]");
            while (c != 'o' && c != 'n'){
                std::cout << "Erreur : veuillez saisir une valeur correcte." << std::endl;
                askUser(c, "Voulez-vous créer un nouveau événement [o/n]");
            }
            if (c == 'o'){
                createEvent(ag);
            }
        }
    }

} // namespace event