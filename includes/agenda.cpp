#include<vector>
#include<iostream>
#include<filesystem>
#include<fstream>
#include<sstream>
#include<random>
#include<iterator>

#include"agenda.hpp"

namespace fs = std::filesystem;

namespace agenda{

    // créer un identifiant aléatoire pour un agenda
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

    // charger un agenda
    Agenda loadAgenda(const fs::path& path){

        Agenda ag;

        std::string str;
        std::ifstream ifs(path);
        std::vector<std::string> v;
        while (std::getline(ifs, str)){
            v.push_back(str);
        }

        ag.id = v[0];
        ag.title = v[1];
        ag.description = v[2];

        std::vector<std::string> vEvents;
        for (int i=3; i<(int)v.size(); i++)
            vEvents.push_back(v[i]);

        ag.events = event::loadAllEvents(vEvents);

        return ag;
    }

    // charger tous les agendas du dossier contenant les agendas
    std::vector<Agenda> loadAllAgenda(){
        
        std::vector<Agenda> vAgenda;

        fs::path path = "agenda";
        fs::directory_iterator di(path);
        for (fs::directory_entry de : di){
            if (de.is_regular_file())
                vAgenda.push_back(loadAgenda(de));
        }

        return vAgenda;
    }

    // choisir un agenda à charger parmis ceux du dossier contenant les agendas
    Agenda chooseAgenda(){

        std::vector<Agenda> vAg = loadAllAgenda();
        
        int choice = 100000;
        
        std::cout
        << std::endl << "---- Sélectionner un agenda ----" << std::endl << std::endl;

        for (int i=0; i<(int)vAg.size(); i++){
            std::cout
            << "Agenda n°" << i+1 << " :" << std::endl
            << "  Titre : " << vAg[i].title << std::endl
            << "  Description : " << vAg[i].description << std::endl
            << "  id : " << vAg[i].id << std::endl << std::endl;
        }

        askUser(choice, "Saisissez le numéro de l'agenda à sélectionner");

        while (choice<1 || choice>(int)vAg.size()){
            std::cout << "Erreur : veuillez saisir un nombre valide." << std::endl;
            askUser(choice, "Saisissez le numéro de l'agenda à sélectionner");
        }

        return vAg[choice-1];
    }

    // créer un agenda
    Agenda createAgenda(){
        Agenda ag;
        
        ag.id = getUID(80);

        std::vector<event::Event> events;
        ag.events = events;

        std::cout << "---- Nouveau agenda ----" << std::endl << std::endl;
        
        std::cout << "Saisissez un titre" << std::endl;
        askUser(ag.title, "Titre");

        std::cin.putback('\n');

        std::cout << std::endl << "Saisissez une description" << std::endl;
        askUser(ag.description, "Description");

        return ag;
    }

    // sauvegarder un agenda dans un fichier + ses événements
    void saveAgenda(const Agenda& ag){
        
        fs::path path = "agenda";
        fs::directory_iterator di(path);
        for (fs::directory_entry de : di){
            fs::path tmp = de;
            if (tmp.stem() == ag.id)
                remove(tmp);
        }

        path = "agenda/" + ag.id + ".txt";

        std::ofstream ofs(path);
        ofs << ag.id << std::endl << ag.title << std::endl << ag.description << std::endl;
        for (int i=0; i<(int)ag.events.size(); i++)
            ofs << ag.events[i].id << std::endl;

        event::saveAllEvents(ag.events);    
        
        std::vector<Agenda> vAg = loadAllAgenda();
        std::vector<event::Event> vEvent;
        for (Agenda A : vAg){
            for (event::Event E : A.events){
                vEvent.push_back(E);
            }
        }
        event::rmNonExistEvent(vEvent);
    }
    
    // sauvegarder un agenda au format html
    void saveAgendaHTML(const Agenda& ag){

        fs::path path = "agenda/html";
        fs::directory_iterator di(path);
        for (fs::directory_entry de : di){
            fs::path tmp = de;
            if (tmp.stem() == ag.id)
                remove(tmp);
        }

        path = "agenda/html/" + ag.title + "_" + ag.id + ".html";

        std::ofstream html(path);
        html << "<!DOCTYPE html>" << std::endl << std::endl;
        html << "<html lang=\"fr\">" << std::endl;
        html << "   <head>" << std::endl;
        html << "       <title>Agenda</title>" << std::endl;
        html << "       <meta charset=\"utf-8\">" << std::endl;
        html << "       <style>" << std::endl;
        html << "           body {" << std::endl;
        html << "               font-family: arial;" << std::endl;
        html << "               text-align: center;" << std::endl;
        html << "           }" << std::endl;
        html << "           h2 {" << std::endl;
        html << "               text-align: left;" << std::endl;
        html << "           }" << std::endl;
        html << "           table {" << std::endl;
        html << "               border-collapse: collapse;" << std::endl;
        html << "               width: 100%;" << std::endl;
        html << "           }" << std::endl;
        html << "           th, td {" << std::endl;
        html << "               border: 1px solid #dddddd;" << std::endl;
        html << "               text-align: left;" << std::endl;
        html << "               padding: 8px;" << std::endl;
        html << "           }" << std::endl;
        html << "           tr:nth-child(even) {" << std::endl;
        html << "               background-color: #dddddd;" << std::endl;
        html << "           }" << std::endl;
        html << "       </style>" << std::endl;
        html << "   </head>" << std::endl;
        html << "   <body>" << std::endl;
        html << "       <h1>" << ag.title << "</h1>" << std::endl;
        html << "       <h3>" << "\"" << ag.description << "\"" << "</h3>" << std::endl;
        html << "       <h4>" << ag.id << "</h4>" << std::endl;
        html << "       <h2>Événements de l'agenda :</h2>" << std::endl;
        html << "       <table>" << std::endl;
        html << "           <tr>" << std::endl;
        html << "               <th>Titre</th>" << std::endl;
        html << "               <th>Description</th>" << std::endl;
        html << "               <th>Date de début</th>" << std::endl;
        html << "               <th>Date de fin</th>" << std::endl;
        html << "           </tr>" << std::endl;

        if (ag.events.empty()){
            html << "           <tr>" << std::endl;
            html << "               <td>aucun événement</td>" << std::endl;
            html << "               <td>aucun événement</td>" << std::endl;
            html << "               <td>aucun événement</td>" << std::endl;
            html << "               <td>aucun événement</td>" << std::endl;
            html << "           </tr>" << std::endl;
        }
        else{
            for (int i=0; i<(int)ag.events.size(); i++){
                html << "           <tr>" << std::endl;
                html << "               <td>" << ag.events[i].title << "</td>" << std::endl;
                html << "               <td>" << ag.events[i].description << "</td>" << std::endl;
                html << "               <td>" << ag.events[i].begin.day << "/" << ag.events[i].begin.month << "/" << ag.events[i].begin.year << " " << ag.events[i].begin.hour << ":" << ag.events[i].begin.min << "</td>" << std::endl;
                html << "               <td>" << ag.events[i].end.day << "/" << ag.events[i].end.month << "/" << ag.events[i].end.year << " " << ag.events[i].end.hour << ":" << ag.events[i].end.min << "</td>" << std::endl;
                html << "           </tr>" << std::endl;
            }
        }

        html << "       </table>" << std::endl;
        html << "   </body>" << std::endl;
        html << "</html>" << std::endl;
    }

    // afficher les informations d'un agenda
    void showAgenda(const Agenda& ag){

        char exit = ' ';
        do{
            
            std::cout
            << "---- Agenda ----" << std::endl << std::endl
            << "id : " << ag.id << std::endl
            << "Titre : " << ag.title << std::endl
            << "Description : " << ag.description << std::endl << std::endl
            << "Événements :" << std::endl;
            
            if (ag.events.empty())
                std::cout << " aucun événement" << std::endl << std::endl;

            else{
                for (int i=0; i<(int)ag.events.size(); i++){
                    std::cout
                    << "  Id de l'événement : " << ag.events[i].id << std::endl
                    << "    Titre : " << ag.events[i].title << std::endl
                    << "    Description : " << ag.events[i].description << std::endl
                    << "    Début : " << ag.events[i].begin.day << "/" << ag.events[i].begin.month << "/" << ag.events[i].begin.year << " " << ag.events[i].begin.hour << ":" << ag.events[i].begin.min << std::endl
                    << "    Fin : " << ag.events[i].end.day << "/" << ag.events[i].end.month << "/" << ag.events[i].end.year << " " << ag.events[i].end.hour << ":" << ag.events[i].end.min << std::endl << std::endl;
                }
            }

            askUser(exit, "Entrez 'Q' pour revenir au menu de l'agenda");

        } while (exit != 'Q');
    }

    // supprimer un agenda et ses événements
    void rmAgenda(const Agenda& ag){

        char c = ' ';
        
        askUser(c, "Êtes-vous vraiment sûr de vouloir supprimer cet agenda [o/n]");

        while (c!='o' && c!='n'){
            std::cout << "Erreur : veuillez saisir un nombre valide." << std::endl;
            askUser(c, "Êtes-vous vraiment sûr de vouloir supprimer cet agenda [o/n]");
        }

        if (c == 'o'){
            fs::path path = "agenda/events";

            fs::directory_iterator di(path);
            for (fs::directory_entry de : di){
                for (int i=0; i<(int)ag.events.size(); i++){
                    path = de;
                    if (path.stem() == ag.events[i].id)
                        remove(path);
                }
            }

            path = "agenda/" + ag.id + ".txt";
            remove(path);
        }
    }

    // vérifier si le dossier contenant les agendas est vide (s'il existe des agendas)
    bool isAgDirEmpty(){

        std::vector<Agenda> v = loadAllAgenda();

        return v.empty();
    }

    // afficher une erreur si le dossier contenant les agendas est vide
    void emptyAgDir(){
        char c = ' ';

        std::cout << "Erreur : aucun agenda trouvé. " << "Veuillez en créer un pour continuer." << std::endl;
        askUser(c, "Tapez 'Q' pour quitter");

        while (c != 'Q'){
            std::cout << "Erreur : veuillez saisir une valeur correcte." << std::endl;
            askUser(c, "Tapez 'Q' pour quitter");
        }
    }

}// namespace agenda