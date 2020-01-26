
/**
* @brief A header containing the public functions and private variables in the Preferences instance
* @detailed An overview of the structures behind the Preferences.cpp file
* @parameters Takes multiple - outlined in the Preferences.cpp documentation
* @returns Many (see Preferences.cpp for more information)
* @author Zhaohe Zhang
*/

#ifndef INC_3307GROUP_PREFERENCES_H
#define INC_3307GROUP_PREFERENCES_H

#include <string>
#include <iostream>
using namespace std;

class Preferences {
private:
    string name;
    string country;
    string city;
    int finance;
    int weather;
    int news;
    int date;

public:
    Preferences();
    Preferences(string name, string country, string city);
    Preferences(string name, string country, string city, int finance, int weather, int news, int date);
    ~Preferences();
    int changeName(string newName);
    int changeCountry(string newCountry);
    int changeCity(string newCity);
    int turnFinance(int status);
    int turnWeather(int status);
    int turnNews(int status);
    int turnDate(int status);
    string getName();
    string getCountry();
    string getCity();
    int getFinanceStatus();
    int getWeatherStatus();
    int getNewsStatus();
    int getDateStatus();

    string getPreferences();
    string toString();
};


#endif //INC_3307GROUP_PREFERENCES_H