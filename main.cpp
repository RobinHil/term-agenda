#include<iostream>
#include<string>
#include<filesystem>
#include<system_error>

#include"includes/agenda.hpp"

// demander des informations à l'utilisateur
void askUser(char& c, const std::string& str){
    std::cout << str << " : ";
    std::cin >> c;
    std::cout << std::endl;
}

// menu des agendas (lorsqu'on en ouvre un)
void agendaMenu(agenda::Agenda& ag){
    char c = ' ';
    do
    {
        
        std::cout
        << std::endl << "---- Menu de l'agenda ----" << std::endl << std::endl
        << "Que souhaitez-vous faire dans l'agenda ?" << std::endl
        << " 1. afficher le contenu de l'agenda" << std::endl
        << " 2. exporter l'agenda au format HTML" << std::endl
        << " 3. ajouter un événement" << std::endl
        << " 4. supprimer un événement" << std::endl
        << " Q. fermer l'agenda" << std::endl << std::endl;
    
        askUser(c, "Votre choix");
        while (c != '1' && c != '2' && c != '3' && c != '4' && c != 'Q'){
            std::cout << "Erreur : veuillez saisir une valeur correcte." << std::endl;
            askUser(c, "Votre choix");
        }

        switch (c){
            case '1':
                agenda::showAgenda(ag);
                break;

            case '2':
                agenda::saveAgendaHTML(ag);
                break;

            case '3':
                event::createEvent(ag.events);
                break;

            case '4':
                event::rmEvent(ag.events);
                break;
        
            default:
                break;
        }

    } while (c != 'Q');

    askUser(c, "Voulez vous enregistrer les changements [o/n]");
    while (c != 'o' && c != 'n'){
        std::cout << "Erreur : veuillez saisir une valeur correcte." << std::endl;
        askUser(c, "Voulez vous enregistrer les changements [o/n]");
    }

    if (c == 'o')
        agenda::saveAgenda(ag);
}

// menu principal
void mainMenu(){
    char c = ' ';
    do{
        
        std::cout
        << "---- Menu principal ----" << std::endl << std::endl
        << "Que souhaitez-vous faire ?" << std::endl
        << " 1. charger un agenda" << std::endl
        << " 2. créer un agenda vide" << std::endl
        << " 3. supprimer un agenda" << std::endl
        << " Q. quitter l'application" << std::endl << std::endl;

        askUser(c, "Votre choix");
        while (c != '1' && c != '2' && c != '3' && c != 'Q'){
            std::cout << "Erreur : veuillez saisir une valeur correcte." << std::endl;
            askUser(c, "Votre choix");
        }

        agenda::Agenda ag;
        switch (c){
            case '1':
                if (agenda::isAgDirEmpty()){
                    agenda::emptyAgDir();
                } else{
                    ag = agenda::chooseAgenda();
                    agendaMenu(ag);
                }
                break;

            case '2':
                ag = agenda::createAgenda();
                agendaMenu(ag);
                break;

            case '3':
                if (agenda::isAgDirEmpty()){
                    agenda::emptyAgDir();
                } else{
                    ag = agenda::chooseAgenda();
                    agenda::rmAgenda(ag);
                }
                break;
        
            default:
                break;
        }
    } while (c != 'Q');
}

// main
int main(){
    std::error_code ec;
    
    // Création des dossiers nécessaires s'ils n'existent pas
    if(!std::filesystem::exists("agenda")) {
        std::filesystem::create_directory("agenda", ec);
    }
    if(!std::filesystem::exists("agenda/events")) {
        std::filesystem::create_directory("agenda/events", ec);
    }
    if(!std::filesystem::exists("agenda/html")) {
        std::filesystem::create_directory("agenda/html", ec);
    }

    mainMenu();

    return EXIT_SUCCESS;
}