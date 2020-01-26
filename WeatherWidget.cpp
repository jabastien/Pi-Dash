#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <locale.h>
#include <curl/curl.h>

#include "WeatherWidget.h"

using namespace std;

/**
* @brief Default empty constructor for Weather Widget
* @detailed Creates an empty Weather Widget as the default widget configuration
* @parameters Takes no parameters
* @returns An Empty Weather Widget
* @author Mitch Mancuso
*/
//The constructor for the empty stock widget object, sets parameters to be 0 or empty.
WeatherWidget::WeatherWidget() {
    weatherWidgetName = "";
    weatherWidgetID = 0;
    weatherWidgetCountry = "";
    weatherWidgetCity = "";
    typeOfForecast = ' ';
}

/**
* @brief Creates a WeatherWidget object that stores the user's set information in order to return data
* @detailed Standard WeatherWidget object - stores the weatherWidgetName, its ID for reference on the dashboard, the user's entered country, city, the type of forecast (current or 5 day) and finds the weatherData
* @parameters string WeatherWidgetName , int weatherWidgetID, string weatherWidgetCountry, string weatherWidgetCity, char typeOfForecast
* @returns A weather widget with properties set by the constructor's parameters
* @author Mitch Mancuso
*/
WeatherWidget::WeatherWidget(string weatherWidgetName, int weatherWidgetID, string weatherWidgetCountry, string weatherWidgetCity, char typeOfForecast) {
    //Set the global variables to the local constructors
    this->weatherWidgetName = weatherWidgetName;
    this->weatherWidgetID = weatherWidgetID;
    this->weatherWidgetCountry = weatherWidgetCountry;
    this->weatherWidgetCity = weatherWidgetCity;
    this->typeOfForecast = typeOfForecast;
    //Sets a friendly name for the type of forecast which is input by the user as a char.
    if (typeOfForecast == 'C' || typeOfForecast == 'c'){
        this->typeOfForecastLong = "current";
    }
    else{
        this->typeOfForecastLong = "five-day";
    }
    //Calls the API function and saves the resulting string in the weatherData string. Makes use of the user's entered country and city.
    this->weatherData = weatherAPICallResults(weatherWidgetCountry,weatherWidgetCity);
}

/**
* @brief Default empty destructor for Weather Widget
* @detailed Destroys Weather Widget in memory
* @parameters Takes no parameters
* @author Mitch Mancuso
*/
//Deconstructor method
WeatherWidget::~WeatherWidget(){
}

/**
* @brief Finds multiple instances of data from the API calls, will pick out multiple temperatures, conditions
* @detailed nthsubstr is a function that allows for some classes (like news) that have repeated lines in the API call - to pick out instances.
* @parameters int n - number of repititions, string s - address of string to find, string p - string content to replace
* @author Mitch Mancuso
*/
//nthsubstr is a function that allows for some classes (like news) that have repeated lines in the API call - to pick out instances.
//For example, since headline: is repeated multiple times in the json return, nthsubstr keeps track and can pick out the 2nd, 3rd, 4th, nth instance of it.
int nthWeatherSubstr(int n, const string& s, const string& p) {
    //uses the find function to find the string
    string::size_type i = s.find(p);
    int j;
    //traverses the string to find the instance designated by the user in variable n, where n is the instance of it - 1 = 1st, 2 = 2nd, etc
    for (j = 1; j < n && i != string::npos; ++j)
        i = s.find(p, i+1);
    // returns the location of the string, if it exists
    if (j == n)
        return(i);
    // returns -1 to let the calling function know that there are no further instances
    else
        return(-1);
}

/**
* @brief This function is used as a helper to CURL in order to use the pulled information as a string
* @detailed This function will parse and allow for the results of the CURL callback to be written to a string
* @parameters contents, size, nmemb, *userp
* @author Mitch Mancuso
*/
//This callback function gets called by libcurl as soon as there is data received that needs to be saved.
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
* @brief Calls the weatherAPI and stores the results in a string
* @detailed Utilizes CURL to pull weather data from the openWeather API and store in in a string to be returned. Uses the user's input country and city to put in the URL.
* @parameters string weatherWidgetCountry, string weatherWidgetCity
* @returns A string with the block of text containing the weatherAPI results for the
* country and city the user is in. Returns errors depending on the issues that could be faced.
* For example, an error is returned if the network is not connected, and also if the city / country information is incorrect.
* @author Mitch Mancuso
*/
//The weatherAPICallResults pulls the information from the API and transforms the data into a string.
//As well, this function removes " characters from the pull and replaces them with ~, as " are special characters in C, C++ - just to make things easier
string WeatherWidget::weatherAPICallResults(string weatherWidgetCountry, string weatherWidgetCity) {
    //Mandatory calls in order to use cURL, which allows for data retrieval from the server
    string readBuffer;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    //If curl can be launched used the API key and URL
    if(curl) {
        string APIKEY = "654722f07c0223d34aca11890f9653aa";
        string baseURL;
        //set the curlURL based on the baseURL and API key
        if(typeOfForecastLong == "current"){
            baseURL = "http://api.openweathermap.org/data/2.5/weather?q=";
        }
        else{
            baseURL = "https://api.openweathermap.org/data/2.5/forecast?q=";
        }
        string curlURL = baseURL + weatherWidgetCity + "," + weatherWidgetCountry + "&units=metric&appid=" + APIKEY;
        //set up variables to store information from curl (readBuffer, and curlURL as a source)
        curl_easy_setopt(curl, CURLOPT_URL, curlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //Start curl process to pull date

        res = curl_easy_perform(curl);
        //Show user an error if there is a network issue
        if(res!= CURLE_OK){
            this->weatherWidgetID = -1;
            return "ERROR! Network Not Connected!";
        }

        curl_easy_cleanup(curl);

        //create a new string equal to the entire result of the curl
        string formattedString = readBuffer;

        //use regex to replace the special " character with a ~
        formattedString = regex_replace(formattedString, regex("\""), "~");
        //       cout << formattedString << endl;
        //return the formattedString to the function
        return formattedString;
    }
    return "ERROR. CURL NOT INITIALIZED.";
}

/**
* @brief Returns the name of the WeatherWidget
* @detailed A getter function to return the name of the WeatherWidget
* @parameters No parameters
* @returns A string containing the Weather Widget's name
* @author Mitch Mancuso
*/
//Standard getter for the widgetName, returns the name as a string
string WeatherWidget::getWeatherWidgetName(){
    return this->weatherWidgetName;
}

/**
* @brief Returns the ID of the WeatherWidget
* @detailed A getter function to return the ID of the WeatherWidget
* @parameters No parameters
* @returns A string containing the Weather Widget's ID
* @author Mitch Mancuso
*/
//Standard getter for the widgetID
int WeatherWidget::getWeatherWidgetID(){
    return this->weatherWidgetID;
}

/**
* @brief Returns the user's set country of the WeatherWidget
* @detailed A getter function to return the user's set country of the WeatherWidget
* @parameters No parameters
* @returns A string containing the Weather Widget's user's set country
* @author Mitch Mancuso
*/
//Getter that returns the user's entered country to retrieve weather data from
string WeatherWidget::getWeatherWidgetCountry(){
    return this->weatherWidgetCountry;
}

/**
* @brief Returns the user's set city of the WeatherWidget
* @detailed A getter function to return the user's set city of the WeatherWidget
* @parameters No parameters
* @returns A string containing the Weather Widget's user's set city
* @author Mitch Mancuso
*/
//Getter that returns the user's entered city to retrieve weather data from
string WeatherWidget::getWeatherWidgetCity(){
    return this->weatherWidgetCity;
}

/**
* @brief Returns the user's set city of the WeatherWidget
* @detailed A getter function to return the user's set city of the WeatherWidget
* @parameters No parameters
* @returns A string containing the Weather Widget's user's set city
* @author Mitch Mancuso
*/
//Getter that sends back the type of forecast the user selected (current or 5 day)
char WeatherWidget::getTypeOfForecast(){
    return this->typeOfForecast;
}

/**
* @brief Returns the user's type of forecast from the WeatherWidget
* @detailed Returns the friendly name for the type of forecast the user selected. This standardizes the output to by "Five-Day" or "Current"
* @parameters No parameters
* @returns A string containing the Weather Widget's type of forecast
* @author Mitch Mancuso
*/
//Getter that sends back the friendly name of the type of forecast the user selected
string WeatherWidget::getTypeOfForecastLong(){
    return this->typeOfForecastLong;
}

/**
* @brief Returns the temperature from the WeatherWidget's API Call results
* @detailed Returns the temperature from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (temperature)
* @parameters No parameters
* @returns A string containing the temperature for use in the weather widget (this is for the current forecast)
* @author Mitch Mancuso
*/
//Retrieves the actual weather temperature through manipulation to the formattedString.
string WeatherWidget::getWeatherTemperature(){
    string weatherDataTemp;
    //Creates a copy of the formattedStringData in order to operate
    weatherDataTemp = this->weatherData;
    //creates int markers for where current temperature is located in the json string (beginning and end)
    int tempCurr, endTempCurr;
    //finds the tempCurr location start based on where ~temp~: appears in the json string, offset by 7 to ignore special characters
    tempCurr = nthWeatherSubstr(1, weatherDataTemp, "~temp~:") + 7;
    //           cout << tempCurr << endl;
    //finds the endTempCurr location start based on where ~pressure~: appears in the json string.
    endTempCurr = nthWeatherSubstr(1, weatherDataTemp, ",~pressure~:");
    //Therefore as the temperature is between the temp: and the pressure: location in the string, use the substr function to subtract the two locations in order to just get the temperature
    string temperature = weatherDataTemp.substr (tempCurr,endTempCurr - tempCurr);

    if(temperature.find(":~404~,") == 0){
        this->weatherWidgetID = -2;
        return "ERROR: City is Incorrect.";
    }

        //        cout << temperature << endl;

        //Return just the temperature to the calling function
    return temperature;
}

/**
* @brief Returns the weather condition from the WeatherWidget's API Call results
* @detailed Returns the weather condition from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (condition)
* as well, this function will produce a glyph or emoji representation of the weather on supported terminals (like macOS)
* @parameters No parameters
* @returns A string containing the weatherCondition with Emoji for use in the weather widget (this is for the current forecast)
* @author Mitch Mancuso
*/
//Retrieves the actual weather conditions through manipulation to the formattedString.
string WeatherWidget::getWeatherConditionEmoji(){
    string weatherDataTemp;
    //Creates a copy of the formattedStringData in order to operate
    weatherDataTemp = this->weatherData;
    //creates int markers for where current desription is located in the json string (beginning and end)
    int descCurr, endDescCurr;
    //finds the descCurr location start based on where ~main~: appears in the json string, offset by 8 to ignore special characters.
    descCurr = nthWeatherSubstr(1, weatherDataTemp, "~main~:~") + 8;
//            cout << descCurr << endl;
    //finds the endDescCurr location start based on where ~description~: appears in the json string.
    endDescCurr = nthWeatherSubstr(1, weatherDataTemp, "~,~description~:~");
    //Therefore as the description is between the main: and the description: location in the string, use the substr function to subtract the two locations in order to just get the description
    string description = weatherDataTemp.substr (descCurr,endDescCurr - descCurr);

//Uses the description categories pulled from the API to display glyphs or emoji on compatible machines.
//On pi, they will show if iocharset=utf8 is enabled, on MacOS and others, they will show using unicode symbols or emoji mapping
    if (description == "Thunderstorm"){
        this->icon = "\U000026C8";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Drizzle"){
        this->icon = "\U0001F4A7";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Rain"){
        this->icon = "\U0001F327";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Snow"){
        this->icon = "\U00002744";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Atmosphere"){
        this->icon = "\U0001F32B";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Clear"){
        this->icon = "\U00002600";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Clouds"){
        this->icon = "\U000026C5";
//                cout << "Icon is: " + icon << endl;
    }
    //stores the glyph and the description in the weather condition string - for returning
    string weatherCondition = this->icon + " - " + description;

    //returns the weatherCondition
    return weatherCondition;
}

/**
* @brief Returns the weather condition from the WeatherWidget's API Call results
* @detailed Returns the weather condition from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (condition)
* @parameters No parameters
* @returns A string containing the weatherCondition with Emoji for use in the weather widget (this is for the current forecast)
* @author Mitch Mancuso
*/
//Retrieves the actual weather conditions through manipulation to the formattedString.
string WeatherWidget::getWeatherCondition(){
    string weatherDataTemp;
    //Creates a copy of the formattedStringData in order to operate
    weatherDataTemp = this->weatherData;
    //creates int markers for where current desription is located in the json string (beginning and end)
    int descCurr, endDescCurr;
    //finds the descCurr location start based on where ~main~: appears in the json string, offset by 8 to ignore special characters.
    descCurr = nthWeatherSubstr(1, weatherDataTemp, "~main~:~") + 8;
//            cout << descCurr << endl;
    //finds the endDescCurr location start based on where ~description~: appears in the json string.
    endDescCurr = nthWeatherSubstr(1, weatherDataTemp, "~,~description~:~");
    //Therefore as the description is between the main: and the description: location in the string, use the substr function to subtract the two locations in order to just get the description
    string description = weatherDataTemp.substr (descCurr,endDescCurr - descCurr);

    //stores the glyph and the description in the weather condition string - for returning
    string weatherCondition = description;

    //returns the weatherCondition
    return weatherCondition;
}

/**
* @brief Returns the temperatures from the WeatherWidget's API Call results for the day input by the user
* @detailed Returns the temperature from the APICallResults based on which day the user would like,
* the day represents the days from now in the five day forecast - for example, if 5 is input
* it will pull the temperature for 5 days from now through the weatherData variable and only picking out what string
* information is relevant (temperature)
* @parameters int day
* @returns A string containing the temperature from a specific day out of the 5 day forecast (this is for the 5 day forecast)
* @author Mitch Mancuso
*/
string WeatherWidget::getWeatherTemperatures(int day){
    //Creates a copy of the formattedStringData in order to operate
    string weatherDataTemp;
    weatherDataTemp = this->weatherData;
    //titleList will be a string keeping track of all of the headlines
    string temperatureList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < 5; i++){
        //Use ints to keep track of the title location and description location in the string - in order to pull out the substring
        int titleloc, descloc;
        //The title will be located 8 after this string, based on this datas format - save this location
        titleloc = nthWeatherSubstr(i+1, weatherData, "~temp~:") + 7;
        //cout << titleloc << endl;
        //The description will be located after this string, based on this datas format - save this location
        descloc = nthWeatherSubstr(i+1, weatherData, ",~temp_min~:");
        //Therefore as the title is between the titlelocation and the descrption location, use the substr function to subtract the two locations in order to just get the headline
        string title = weatherData.substr (titleloc,descloc - titleloc);

        fiveDayTemperatures[i] = title;

        if(title.find(":~404~,") == 0){
            this->weatherWidgetID = -2;
            return "ERROR: City is Incorrect.";
        }

                //cout << title << endl;
        //Keep the title lists in tact throughout each iteration of the loop and keep combining
        temperatureList = temperatureList + title + "\n";
    }
    //return the titleList which is all headlines ,once the for loop completes
    return fiveDayTemperatures[day];
}

/**
* @brief Returns the weather conditions from the WeatherWidget's API Call results for the day input by the user
* @detailed Returns the weather conditions from the APICallResults based on which day the user would like,
* the day represents the days from now in the five day forecast - for example, if 5 is input
* it will pull the weather conditions for 5 days from now through the weatherData variable and only picking out what string
* information is relevant (weather conditions)
* @parameters int day
* @returns A string containing the weather conditions from a specific day out of the 5 day forecast (this is for the 5 day forecast)
* @author Mitch Mancuso
*/
//Similar process to the headlines where the urls are captures from the formattedString
string WeatherWidget::getWeatherConditions(int day){
    //Creates a copy of the formattedStringData in order to operate
    string weatherDataTemp;
   weatherDataTemp = this->weatherData;
    //urlList will be a string keeping track of all of the URLs
    string conditionList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < 5; i++){
        //Use ints to keep track of the url location and description location in the string - in order to pull out the substring
        int startLoc, endLoc;
        //The url will be located 8 after this string, based on this datas format - save this location
        startLoc = nthWeatherSubstr(i+1, weatherData, "~main~:~") + 8;
//        cout << title1loc << endl;
        //The to_image will be located after this string, based on this datas format - save this locati
        endLoc = nthWeatherSubstr(i+1, weatherData, "~,~description~:~");
        //Therefore as the URL is between the url location and the url_image location, use the substr function to subtract the two locations in order to just get the URL
        string desc = weatherData.substr (startLoc,endLoc - startLoc);

//        cout << desc << endl;

        //return the URLlist which is all URLs ,once the for loop completes
        conditionList = conditionList + desc +'\n';
        fiveDayConditions[i] = desc;
    }
    return fiveDayConditions[day];
}

/**
* @brief Prints the weather information pulled from the API for use in testing
* @detailed Returns the weather information like temperature and conditions that are from the API calls
* @parameters none
* @returns void. Prints to the screen the results of getting the temperature and weather conditions using the other functions in the class.
* @author Mitch Mancuso
*/
void WeatherWidget::printWeatherInformation(){
    string weatherDataTemp;
    //Creates a copy of the formattedStringData in order to operate
    weatherDataTemp = this->weatherData;
    //creates int markers for where current temperature is located in the json string (beginning and end)
    int tempCurr, endTempCurr;
    //creates int markers for where current temperature is located in the json string (beginning and end)
    tempCurr = nthWeatherSubstr(1, weatherDataTemp, "~temp~:") + 7;
    //           cout << tempCurr << endl;
    endTempCurr = nthWeatherSubstr(1, weatherDataTemp, ",~pressure~:");
    string temperature = weatherDataTemp.substr (tempCurr,endTempCurr - tempCurr);

    //        cout << temperature << endl;

    int descCurr, endDescCurr;
    //finds the descCurr location start based on where ~main~: appears in the json string, offset by 8 to ignore special characters.
    descCurr = nthWeatherSubstr(1, weatherDataTemp, "~main~:~") + 8;
//            cout << descCurr << endl;
    //finds the endDescCurr location start based on where ~description~: appears in the json string.
    endDescCurr = nthWeatherSubstr(1, weatherDataTemp, "~,~description~:~");
    //Therefore as the description is between the main: and the description: location in the string, use the substr function to subtract the two locations in order to just get the description
    string description = weatherDataTemp.substr (descCurr,endDescCurr - descCurr);

    //Uses the description categories pulled from the API to display glyphs or emoji on compatible machines.
    //On pi, they will show if iocharset=utf8 is enabled, on MacOS and others, they will show using unicode symbols or emoji mapping

    if (description == "Thunderstorm"){
        this->icon = "\U000026C8";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Drizzle"){
        this->icon = "\U0001F4A7";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Rain"){
        this->icon = "\U0001F327";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Snow"){
        this->icon = "\U00002744";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Atmosphere"){
        this->icon = "\U0001F32B";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Clear"){
        this->icon = "\U00002600";
//                cout << "Icon is: " + icon << endl;
    }
    else if (description == "Clouds"){
        this->icon = "\U000026C5";
//                cout << "Icon is: " + icon << endl;
    }
    //stores the glyph and the description in the weather condition string - for returning
    string weatherCondition = this->icon + " - " + description;

    //Prints the output instead of storing as a string in the format the user can see.
    cout <<"The current temperature in "<< weatherWidgetCity <<" is " << temperature << "° C" << endl;
    cout <<"The current weather conditions for "<< weatherWidgetCity <<" are (" <<  icon << " " << description << ")" << endl;

}

