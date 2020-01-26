#include "Preferences.h"
using namespace std;

/**
* @brief Default empty constructor for User Preferences
* @detailed Creates an empty Preferences instance as the default configuration
* @parameters Takes no parameters
* @returns An Empty Preferences object
* @author Zhaohe Zhang
*/
Preferences::Preferences() {
    name = "";
    country = "";
    city = "";
    finance = 1;
    weather = 1;
    news = 1;
    date = 1;
}

/**
* @brief Constructor for User Preferences with paramaters passed
* @detailed Sets the user preferences based on a constructed object with the name, country, and city as input
* @parameters string name, string country, string city
* @returns A Preferences object
* @author Zhaohe Zhang
*/
Preferences::Preferences(string name, string country, string city) {
    this->name = name;
    this->country = country;
    this->city = city;
    finance = 1;
    weather = 1;
    news = 1;
    date = 1;
}

/**
* @brief Constructor for User Preferences with extended paramaters passed
* @detailed Sets the user preferences based on a constructed object with more preferences the name, country, and city, finance, weather, news, date widget status as input
* @parameters string name, string country, string city, int finance, int weather, int news, int date
* @returns A Preferences object
* @author Zhaohe Zhang
*/
Preferences::Preferences(string name, string country, string city, int finance, int weather, int news, int date) {
    this->name = name;
    this->country = country;
    this->city = city;
    this->finance = finance;
    this->weather = weather;
    this->news = news;
    this->date = date;
}

/**
* @brief Default empty destructor for Preferences
* @detailed Destroys Preferences Object in memory
* @parameters Takes no parameters
* @author Zhaohe Zhang
*/
Preferences::~Preferences() {
    this->name = "";
    this->country = "";
    this->city = "";
}

/**
* @brief Returns a status of -1 or 0 when setting a new preference name attribute
* @detailed A setter function to change the name within the preferences object
* @parameters string newName
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::changeName(string newName) {
    if(newName.empty()) {
        return -1;
    }
    this->name = newName;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting a new preference country attribute
* @detailed A setter function to change the country within the preferences object
* @parameters string newCountry
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::changeCountry(string newCountry) {
    if(newCountry.empty()) {
        return -1;
    }
    this->country = newCountry;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting a new preference city attribute
* @detailed A setter function to change the city within the preferences object
* @parameters string newCity
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::changeCity(string newCity) {
    if(newCity.empty()) {
        return -1;
    }
    cout << "Inside: " << newCity << endl;
    this->city = newCity;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting the finance widget to on or off
* @detailed A setter function to change the finance widget status within the preferences object
* @parameters int status
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::turnFinance(int status) {
    if(status != 0 && status != 1) {
        return -1;
    }
    finance = status;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting the weather widget to on or off
* @detailed A setter function to change the weather widget status within the preferences object
* @parameters int status
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::turnWeather(int status) {
    if(status != 0 && status != 1) {
        return -1;
    }
    weather = status;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting the news widget to on or off
* @detailed A setter function to change the news widget status within the preferences object
* @parameters int status
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::turnNews(int status) {
    if(status != 0 && status != 1) {
        return -1;
    }
    news = status;
    return 0;
}

/**
* @brief Returns a status of -1 or 0 when setting the time and date widget to on or off
* @detailed A setter function to change the time and date widget status within the preferences object
* @parameters int status
* @returns An int, 0 for successful change made -1 for an error displayed
* @author Zhaohe Zhang
*/
int Preferences::turnDate(int status) {
    if(status != 0 && status != 1) {
        return -1;
    }
    date = status;
    return 0;
}

/**
* @brief Returns the name of the Preferences Object
* @detailed A getter function to return the name of the Preferences Object
* @parameters No parameters
* @returns A string containing the Preferences Object Name attribute
* @author Zhaohe Zhang
*/
string Preferences::getName() {
    return name;
}

/**
* @brief Returns the country of the Preferences Object
* @detailed A getter function to return the country of the Preferences Object
* @parameters No parameters
* @returns A string containing the Preferences Object country attribute
* @author Zhaohe Zhang
*/
string Preferences::getCountry() {
    return country;
}

/**
* @brief Returns the city of the Preferences Object
* @detailed A getter function to return the city of the Preferences Object
* @parameters No parameters
* @returns A string containing the Preferences Object city attribute
* @author Zhaohe Zhang
*/
string Preferences::getCity() {
    return city;
}

/**
* @brief Returns the finance widget status of the Preferences Object
* @detailed A getter function to return the finance widget status of the Preferences Object
* @parameters No parameters
* @returns An integer that returns the status of the finance widget
* @author Zhaohe Zhang
*/
int Preferences::getFinanceStatus() {
    return finance;
}

/**
* @brief Returns the weather widget status of the Preferences Object
* @detailed A getter function to return the weather widget status of the Preferences Object
* @parameters No parameters
* @returns An integer that returns the status of the weather widget
* @author Zhaohe Zhang
*/
int Preferences::getWeatherStatus() {
    return weather;
}

/**
* @brief Returns the news widget status of the Preferences Object
* @detailed A getter function to return the news widget status of the Preferences Object
* @parameters No parameters
* @returns An integer that returns the status of the news widget
* @author Zhaohe Zhang
*/
int Preferences::getNewsStatus() {
    return news;
}

/**
* @brief Returns the time and date widget status of the Preferences Object
* @detailed A getter function to return the time and datewidget status of the Preferences Object
* @parameters No parameters
* @returns An integer that returns the status of the time and date widget
* @author Zhaohe Zhang
*/
int Preferences::getDateStatus() {
    return date;
}

/**
* @brief Returns the list of all set preferences from the Preferences Object
* @detailed A getter function to return the complete set of all preferences in a combined string of the Preferences Object
* @parameters No parameters
* @returns A string that stores the concatenation of all preference settings
* @author Zhaohe Zhang
*/
string Preferences::getPreferences() {
    string result;
    result =to_string(finance) + to_string(weather) + to_string(news) + to_string(date);
    return result;
}

/**
* @brief Returns string representation of all set preferences from the Preferences Object
* @detailed A toString method that allows for the formatting of the preference items from the preference objects seperated by a pipe '|'
* @parameters No parameters
* @returns A string that is the print friendly version of the getter for preferences.
* @author Zhaohe Zhang
*/
string Preferences::toString() {
    string result = name + "|" + country + "|" + city + "|" + to_string(finance) + "|" + to_string(weather) + "|" + to_string(news) + "|" + to_string(date);
    return result;
}