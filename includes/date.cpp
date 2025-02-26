#include<string>
#include<iostream>
#include<regex>

#include"date.hpp"

namespace date{

    // demander des informations à l'utilisateur
    void askUser(std::string& c, const std::string& str){
        std::cin.ignore();
        std::cout << str << " : ";
        std::getline(std::cin, c);
    }

    // vérifier si une année est bissextile
    bool isLeapYear(const unsigned short int &year){
        return (year%400==0 || (year%4==0 && year%100!=0));
    }

    // vérifier si une date est correcte (max 24h, max 12mois, pas de valeurs négatives, etc...)
    bool isRightDate(const Date& date){
        return !(stoi(date.day)<1 || stoi(date.day)>31 || stoi(date.month)<1 || stoi(date.month)>12 || stoi(date.hour)<0 || stoi(date.hour)>23 || stoi(date.min)<0 || stoi(date.min)>59 || (stoi(date.month) == 2 && !isLeapYear(stoi(date.year)) && stoi(date.day)>28) || (stoi(date.month) == 2 && stoi(date.day)>29));
    }

    // vérifier si une date est écrite au bon format (jj/mm/aaaa hh:mm)
    bool isRightFormat(const std::string& date) {
        std::regex format("\\d\\d/\\d\\d/\\d\\d\\d\\d \\d\\d:\\d\\d");
        return std::regex_match(date, format);
    }

    // demander à l'utilisateur de saisir une date
    Date askForDate(Date& date, std::string ask){

        std::cout << std::endl;
        ask = "Saisissez une date " + ask + " (jj/mm/aaaa hh:mm)";

        do{
            std::string line;
            askUser(line, ask);
            std::cin.putback('\n');
            while (!isRightFormat(line)){
                std::cout << "Erreur : veuillez utiliser un format correct (jj/mm/aaaa hh:mm)." << std::endl;
                askUser(line, ask);
                std::cin.putback('\n');
            }
            std::string str;
            std::vector<std::string> vDate;
            std::vector<std::string> tmp;
            std::stringstream ssBegin(line);
            while (getline(ssBegin, str, ' '))
            {
                tmp.push_back(str);
            }
            std::stringstream ssBeginDate(tmp[0]);
            while (getline(ssBeginDate, str, '/'))
            {
                vDate.push_back(str);
            }
            std::stringstream ssBeginHour(tmp[1]);
            while (getline(ssBeginHour, str, ':'))
            {
                vDate.push_back(str);
            }

            date.day = vDate[0];
            date.month = vDate[1];
            date.year = vDate[2];
            date.hour = vDate[3];
            date.min = vDate[4];

            if (!isRightDate(date)){
                std::cout << "Erreur : veuillez saisir une date correcte (jj/mm/aaaa hh:mm)." << std::endl;
            }
            
        } while (!isRightDate(date));
        
        return date;
    }

}// namespace date