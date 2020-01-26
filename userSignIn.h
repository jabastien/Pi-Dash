/**
* @brief A header containing the public functions and private variables in the userSignIn instance
* @detailed An overview of the structures behind the userSignIn.cpp file
* @parameters Takes multiple - outlined in the userSignIn.cpp documentation
* @returns Many (see userSignIn.cpp for more information)
* @author Zhaohe Zhang
*/

#ifndef CS3307PROJECTDEMO_USERSIGNIN_H
#define CS3307PROJECTDEMO_USERSIGNIN_H


#include <iostream>
#include <string>
#include <fstream>
#include "Preferences.h"
using namespace std;

class userSignIn {
private:
    string username;
    string password;
    int signInStatus;
    int userExists;
    string name;
    string country;
    string city;
    string timeZone;
    int finance;
    int news;
    int weather;
    int date;
    string errorMessage;
    Preferences preference;
public:
    userSignIn();
    userSignIn(string, string);
    userSignIn(string, string, string, string, string, string, int, int, int, int);
    ~userSignIn();
    void setUserName(string);
    void setPassword(string);
    int getSignInStatus();
    int getUserExists(string);
    string getUserName();
    string getPassword();
    string getName();
    string getCountry();
    string getCity();
    string getTimeZone();
    int getFinance();
    int getWeather();
    int getNews();
    int getDate();
    string getErrorMessage();
    Preferences getPreference();
    void changeName(string newName);
    void changeCountry(string newCountry);
    void changeCity(string newCity);
    void turnFinance(int status);
    void turnWeather(int status);
    void turnNews(int status);
    void turnDate(int status);
    int writeToFile();
};

#endif