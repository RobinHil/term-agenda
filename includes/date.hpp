#ifndef __DATE_HPP__
#define __DATE_HPP__

#include<string>

namespace date{

    //structure des dates
    struct Date{
        std::string day, month, year, hour, min;
    };

    Date askForDate(Date& date, std::string ask);
}// namespace date

#endif