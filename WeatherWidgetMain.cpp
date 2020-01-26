#include <iostream>
#include "WeatherWidget.h"

using namespace std;

/**
* @brief A test function set for the weather widgets
* @detailed Tests the weather widget's ability to test and pull information from the API
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Mitch Mancuso
*/
int main() {
    //Stores the user input strings
    string userCountryInput, userCityInput, typeOfForecastLong;
    //Stores the type of forecast variable
    char typeOfForecast;


    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "------Please enter your 2 letter ISO standard country code (ex CA for Canada)-------" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;

    //Gets the country code from the user
    getline(cin, userCountryInput);

    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "---------Now enter the city that you would like to display the weather for----------" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;

    //Gets the city from the user
    getline(cin, userCityInput);

    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "-----------Please enter C for a Current Forecast or F for a 5-Day Forecast----------" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;

    //Gets the type of forecast from the user
    cin >> typeOfForecast;

    //Formats the typeOfForecast into a friendly string
    if (typeOfForecast == 'C' || typeOfForecast == 'c'){
        typeOfForecastLong = "current";
    }
    else{
        typeOfForecastLong = "five-day";
    }

    //Prints out the weather by starting to create the widgets and run the functions
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "Retrieving weather for "  << userCityInput << "," << userCountryInput << " (" << typeOfForecastLong << ") forecast" << endl;
    cout << "Please wait a moment..." << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    //Creates the weather widget
    WeatherWidget w = WeatherWidget("Test Widget",3,userCountryInput, userCityInput, typeOfForecast);
    //Tests getter classes and that information has been pulled correctly
    cout << "Widget name is: " << w.getWeatherWidgetName() << endl;
    cout << "Widget ID is: " << w.getWeatherWidgetID() << endl;
    cout << "Widget country is: " << w.getWeatherWidgetCountry() << endl;
    cout << "Widget city is: " << w.getWeatherWidgetCity() << endl;
    cout << "Widget forecast type is: " << w.getTypeOfForecast() << endl;
    cout << "Widget forecast (long) type is: " << w.getTypeOfForecastLong() << endl;
    if(typeOfForecastLong == "current") {
        cout << "The weather's current temperature is: " << w.getWeatherTemperature() << endl;
        cout << "The weather's current condition is: " << w.getWeatherCondition() << "\n\n";
        cout << "The weather's current condition is: " << w.getWeatherConditionEmoji() << "\n\n";
    }
    else {
        cout << "The weather's 0 day(s) from now temperature is: " << w.getWeatherTemperatures(0) << "\n\n";
        cout << "The weather's 0 day(s) from now condition is: " << w.getWeatherConditions(0) << "\n\n";
        cout << "The weather's 1 day(s) from now temperature is: " << w.getWeatherTemperatures(1) << "\n\n";
        cout << "The weather's 1 day(s) from now condition is: " << w.getWeatherConditions(1) << "\n\n";
        cout << "The weather's 2 day(s) from now temperature is: " << w.getWeatherTemperatures(2) << "\n\n";
        cout << "The weather's 2 day(s) from now condition is: " << w.getWeatherConditions(2) << "\n\n";
        cout << "The weather's 3 day(s) from now temperature is: " << w.getWeatherTemperatures(3) << "\n\n";
        cout << "The weather's 3 day(s) from now condition is: " << w.getWeatherConditions(3) << "\n\n";
        cout << "The weather's 4 day(s) from now temperature is: " << w.getWeatherTemperatures(4) << "\n\n";
        cout << "The weather's 4 day(s) from now condition is: " << w.getWeatherConditions(4) << "\n\n";
    }
    //Prints the weather information from the void function
    //w.printWeatherInformation();
}
