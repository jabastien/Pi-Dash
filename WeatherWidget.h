#ifndef WEATHER_H
#define WEATHER_H

#include <string>

/**
* @brief A class containing the public functions and private variables in the Weather Widget objects
* @detailed An overview of the structures behind the WeatherWidget.cpp file
* @parameters Takes multiple - outlined in the WeatherWidget.cpp documentation
* @returns Many (see WeatherWidget.cpp for more information)
* @author Mitch Mancuso
*/


class WeatherWidget {

    //h file declarations for global variables as well as getter functions

public:
    WeatherWidget();
    WeatherWidget(std::string, int, std::string, std::string, char);
    ~WeatherWidget();

    std::string getWeatherWidgetName();
    int getWeatherWidgetID();
    std::string getWeatherWidgetCountry();
    std::string getWeatherWidgetCity();
    char getTypeOfForecast();
    std::string getTypeOfForecastLong();
    std::string getIcon();
    std::string weatherAPICallResults(std::string, std::string);
    std::string getWeatherTemperature();
    std::string getWeatherTemperatures(int);
    std::string getWeatherConditionEmoji();
    std::string getWeatherCondition();
    std::string getWeatherConditions(int day);
    void printWeatherInformation();

private:
    std::string weatherWidgetName;
    int weatherWidgetID;
    std::string weatherWidgetCountry;
    std::string weatherWidgetCity;
    char typeOfForecast;
    std::string typeOfForecastLong;
    std::string icon;
    std::string weatherData;
    std::string fiveDayTemperatures[5];
    std::string fiveDayConditions[5];

};

#endif
