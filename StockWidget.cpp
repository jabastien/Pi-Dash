#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <curl/curl.h>
#include "StockWidget.h"

using namespace std;

/**
* @brief Default empty constructor for Stock Widget
* @detailed Creates an empty Stock Widget as the default widget configuration
* @parameters Takes no parameters
* @returns An Empty Stock Widget
* @author Mitch Mancuso
*/
//The constructor for the empty stock widget object, sets parameters to be 0 or empty.
StockWidget::StockWidget() {
    stockWidgetName = "";
    stockWidgetID = 0;
}

/**
* @brief Creates a StockWidget object that stores the user's set information in order to return data
* @detailed Standard StockWidget object - stores the stockWidgetName, its ID for reference on the dashboard, the user's entered stock symbol
* @parameters string stockWidgetName , int stockWidgetID, string stockSymbol
* @returns A stock widget with properties set by the constructor's parameters
* @author Mitch Mancuso
*/
//Standard StockWidget object - stores the stockWidgetName, its ID for reference on the dashboard and will also store the stock symbol that is being requested
StockWidget::StockWidget(string stockWidgetName, int stockWidgetID, string stockSymbol) {
    //Set the global variables to the local constructors
    this->stockWidgetName = stockWidgetName;
    this->stockWidgetID = stockWidgetID;
    this->stockSymbol = stockSymbol;
    //stockData calls the API function to retrieve the headline information as a string
    this->stockData = stockAPICallResults(stockSymbol);
}

/**
* @brief Default empty destructor for Stock Widget
* @detailed Destroys Stock Widget in memory
* @parameters Takes no parameters
* @author Mitch Mancuso
*/
//Deconstructor method
StockWidget::~StockWidget(){
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
* @brief Calls the stocksAPI and stores the results in a string
* @detailed Utilizes CURL to pull weather data from the intrino API and store in in a string to be returned. Uses the user's input stock symbol put in the URL.
* @parameters string stockSymbol
* @returns A string with the block of text containing the stockAPI results for the
* stock symbols requested. Returns errors depending on the issues that could be faced.
* For example, an error is returned if the network is not connected, and also if the stock symbol information is incorrect.
* @author Mitch Mancuso
*/
//The stockAPICallResults pulls the information from the API and transforms the data into a string. It uses the user's chosen stockSymbol
string StockWidget::stockAPICallResults(string stockSymbol) {
    //Mandatory calls in order to use cURL, which allows for data retrieval from the server
    string readBuffer;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    //If curl can be launched used the API key and URL
    if(curl) {
        string API_Key = "OjEyMTJmZDE2ZjYzN2E4OGE2YzI5Y2VjOTI4YzQxZWEw";
        string baseURL = "https://api.intrinio.com/data_point?identifier=";
        //set the curlURL based on the baseURL, stockSymbol, and other parts of the url string - including the API key
        string curlURL = baseURL + stockSymbol + "&item=ask_price" + "&api_key=" + API_Key;
        //set up variables to store information from curl (readBuffer, and curlURL as a source)
        curl_easy_setopt(curl, CURLOPT_URL, curlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //Start curl process to pull date
        res = curl_easy_perform(curl);
        //Show user an error if there is a network issue
        if(res!= CURLE_OK){
            this->stockWidgetID = -1;
            return "ERROR! Network Not Connected!";
        }
        curl_easy_cleanup(curl);

        //create a new string equal to the entire result of the curl
        string formattedString = readBuffer;

//        cout << formattedString << endl;
        //return the formattedString to the function
        return formattedString;
    }
        return "ERROR. CURL NOT INITIALIZED.";
}

/**
* @brief Returns the name of the StockWidget
* @detailed A getter function to return the name of the StockWidget
* @parameters No parameters
* @returns A string containing the Stock Widget's name
* @author Mitch Mancuso
*/
//Standard getter for the widgetName, returns the name as a string
string StockWidget::getStockWidgetName(){
    return this->stockWidgetName;
}

/**
* @brief Returns the ID of the StockWidget
* @detailed A getter function to return the ID of the StockWidget
* @parameters No parameters
* @returns A string containing the Stock Widget's ID
* @author Mitch Mancuso
*/
//Standard getter for the widgetID, returns the id as an int
int StockWidget::getStockWidgetID(){
    return this->stockWidgetID;
}

/**
* @brief Returns the symbol called by the StockWidget
* @detailed A getter function to return the stock symbol of the StockWidget
* @parameters No parameters
* @returns A string containing the Stock Widget's stock symbol
* @author Mitch Mancuso
*/
//Getter for the stock symbol, returns it as a string
string StockWidget::getStockSymbol(){
    return this->stockSymbol;
}



/**
* @brief Returns the stock price from the StockWidget's API Call results
* @detailed Returns the stock price from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (stock price)
* @parameters No parameters
* @returns A string containing the stock price for use in the stock widget
* @author Mitch Mancuso
*/
//A getter for the stock price, runs through the retreived string from the API
string StockWidget::getStockPrice(){
    //Creates a copy of the formattedStringData in order to operate
    string stockDataTemp;
    stockDataTemp = this->stockData;
    //sets the location of the ask_price string as the beginning and the } character at the end.
    unsigned first = stockData.find("ask_price");
    unsigned last = stockData.find("}");
    //as the strings have special characters in them and will need to be shifted to extract only the price from the JSON data, these constant strings offset the result
    unsigned shiftRight = 19;
    unsigned shiftLeft = 1;
    //shift the string locations to only get the asking price of the stock
    unsigned shiftedRight = first + shiftRight;
    unsigned shiftedLeft = last - shiftLeft;
    //get the substring from the stockData, and save it in a string
    string currentPriceString = stockDataTemp.substr (shiftedRight,shiftedLeft-shiftedRight);
    //If na is returned, the company does not exist, tell the user to try again
    if(currentPriceString == "\"na"){
        this->stockWidgetID = -2;
        return "\nERROR. Stock symbol does not exist. Please try again.";
    }
    else if(currentPriceString == "\":\"Must specify 'ticker' or 'identifier'\",\"message\":\"An error occured. Please contact success@intrinio.com with the details."){
        this->stockWidgetID = -3;
        return "ERROR: Stock symbol is blank. Please try again.";
    }
    //return the price as a string to the calling function
    return currentPriceString;
}

/**
* @brief Prints the stock information pulled from the API for use in testing
* @detailed Returns the stock information like temperature and conditions that are from the API calls
* @parameters none
* @returns void. Prints to the screen the results of getting the stock information using the other functions in the class.
* @author Mitch Mancuso
*/
void StockWidget::printStockPrice(){
    //Creates a copy of the formattedStringData in order to operate
    string stockDataTemp;
    stockDataTemp = this->stockData;
    //sets the location of the ask_price string as the beginning and the } character at the end.
    unsigned first = stockData.find("ask_price");
    unsigned last = stockData.find("}");
    //as the strings have special characters in them and will need to be shifted to extract only the price from the JSON data, these constant strings offset the result
    unsigned shiftRight = 19;
    unsigned shiftLeft = 1;
    //shift the string locations to only get the asking price of the stock
    unsigned shiftedRight = first + shiftRight;
    unsigned shiftedLeft = last - shiftLeft;
    //get the substring from the stockData, and save it in a string
    string currentPriceString =stockDataTemp.substr (shiftedRight,shiftedLeft-shiftedRight);
    //convert the current price into a double, in order to print it with precision
    double currentPrice = atof(currentPriceString.c_str());

    //If na is returned, the company does not exist, tell the user to try again
    if(currentPriceString == "\"na"){
        this->stockWidgetID = -2;
        cout << "ERROR. Stock symbol does not exist. Please try again." << endl;
    }
    //Otherwise, the stock should exist, so - print it out to the user with 2 decimal points
    else {
        cout << "The price per stock of " + stockSymbol + " is $ " << setprecision(2) << fixed << currentPrice << " USD" << endl;
    }
}

