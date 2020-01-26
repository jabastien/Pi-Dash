#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <curl/curl.h>
#include "NewsWidget.h"

using namespace std;

/**
* @brief Default empty constructor for NEws Widget
* @detailed Creates an empty NEws Widget as the default widget configuration
* @parameters Takes no parameters
* @returns An Empty News Widget
* @author Mitch Mancuso
*/
//The constructor for the empty news widget object, sets parameters to be 0 or empty.
NewsWidget::NewsWidget() {
    newsWidgetName = "";
    newsWidgetID = 0;
    numberOfHeadlines = 0;
}

/**
* @brief Creates a NewsWidget object that stores the user's set information in order to return data
* @detailed Standard NewsWidget object - stores the newsWidgetName, its ID for reference on the dashboard, the number of headlines
* @parameters string newsWidgetName , int newsWidgetID, int numberOfHeadlines
* @returns A news widget with properties set by the constructor's parameters
* @author Mitch Mancuso
*/
//Standard NewsWidget object - stores the newsWidgetName, its ID for reference on the dashboard and will also store the number of headlines that the user has requested
NewsWidget::NewsWidget(string newsWidgetName, int newsWidgetID, int numberOfHeadlines) {
    //Set the global variables to the local constructors
    this->newsWidgetName = newsWidgetName;
    this->newsWidgetID = newsWidgetID;
    this->numberOfHeadlines = numberOfHeadlines;
    //newsData calls the API function to retrieve the headline information as a string
    this->newsData = newsAPICallResults();
}

/**
* @brief Default empty destructor for News Widget
* @detailed Destroys News Widget in memory
* @parameters Takes no parameters
* @author Mitch Mancuso
*/
//Deconstructor method
NewsWidget::~NewsWidget(){
}

/**
* @brief Finds multiple instances of data from the API calls, will pick out multiple headlines, URLs, etc
* @detailed nthsubstr is a function that allows for some classes (like news) that have repeated lines in the API call - to pick out instances.
* @parameters int n - number of repititions, string s - address of string to find, string p - string content to replace
* @author Mitch Mancuso
*/
//nthsubstr is a function that allows for some classes (like news) that have repeated lines in the API call - to pick out instances.
//For example, since headline: is repeated multiple times in the json return, nthsubstr keeps track and can pick out the 2nd, 3rd, 4th, nth instance of it.
int nthNewsSubstr(int n, const string& s, const string& p) {
    //uses the find function to find the string
    string::size_type i = s.find(p);
    int j;
    //traverses the string to find the instance designated by the user in variable n, where n is the instance of it - 1 = 1st, 2 = 2nd, etc
    for (j = 1; j < n && i != string::npos; ++j)
        i = s.find(p, i+1);
    if (j == n)
        // returns the location of the string, if it exists
        return(i);
    else
        // returns -1 to let the calling function know that there are no further instances
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
* @brief Calls the newsAPI and stores the results in a string
* @detailed Utilizes CURL to pull weather data from the newsapi API and store in in a string to be returned. Uses the user's number of headlines.
* @parameters none
* @returns A string with the block of text containing the newsAPI results for the
* news headlines and URLs requested. Returns errors depending on the issues that could be faced.
* For example, an error is returned if the network is not connected, and also if the stock symbol information is incorrect.
* @author Mitch Mancuso
*/
//The newsAPICallResults pulls the information from the API and transforms the data into a string.
//As well, this function removes " characters from the pull and replaces them with ~, as " are special characters in C, C++ - just to make things easier
string NewsWidget::newsAPICallResults() {
    //Mandatory calls in order to use cURL, which allows for data retrieval from the server
    string readBuffer;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    //If curl can be launched used the API key and URL
    if(curl) {
        string APIKEY = "71637470226844a399bd114cff9963bf";
        string baseURL = "https://newsapi.org/v2/top-headlines?country=ca&apiKey=";
        //set the curlURL based on the baseURL and API key
        string curlURL = baseURL + APIKEY;
        //set up variables to store information from curl (readBuffer, and curlURL as a source)
        curl_easy_setopt(curl, CURLOPT_URL, curlURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //Start curl process to pull date
        res = curl_easy_perform(curl);
        //Show user an error if there is a network issue
        if(res!= CURLE_OK){
            this->newsWidgetID = -1;
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
* @brief Returns the name of the NewsWidget
* @detailed A getter function to return the name of the NewsWidget
* @parameters No parameters
* @returns A string containing the NewsWidget's name
* @author Mitch Mancuso
*/
//Standard getter for the widgetName, returns the name as a string
string NewsWidget::getNewsWidgetName(){
    return this->newsWidgetName;
}

/**
* @brief Returns the ID of the NewsWidget
* @detailed A getter function to return the ID of the NewsWidget
* @parameters No parameters
* @returns A string containing the News Widget's ID
* @author Mitch Mancuso
*/
//Standard getter for the widgetID
int NewsWidget::getNewsWidgetID(){
    return this->newsWidgetID;
}

/**
* @brief Returns the numberOfHeadlines requested by the NewsWidget
* @detailed A getter function to return the stock symbol of the NewsWidget
* @parameters No parameters
* @returns A int containing the News Widget's number of headlines
* @author Mitch Mancuso
*/
//Gets the number of headlines that the user has requested
int NewsWidget::getNumberOfHeadlines(){
    return this->numberOfHeadlines;
}

/**
* @brief Returns the headlines from the NewsWidget's API Call results
* @detailed Returns the headlines from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (news headline)
* @parameters No parameters
* @returns A string containing the news story headline for use in the news widget
* @author Mitch Mancuso
*/
//Retrieves the actual headlines through manipulation to the formattedString.
string NewsWidget::getNewsHeadlines(){
    //Creates a copy of the formattedStringData in order to operate
    string newsDataTemp;
    newsDataTemp = this->newsData;
    //titleList will be a string keeping track of all of the headlines
    string titleList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < numberOfHeadlines; i++){
        //Use ints to keep track of the title location and description location in the string - in order to pull out the substring
        int titleloc, descloc;
        //The title will be located 8 after this string, based on this datas format - save this location
        titleloc = nthNewsSubstr(i+1, newsData, "title~:~") + 8;
//        cout << title1loc << endl;
        //The description will be located after this string, based on this datas format - save this location
        descloc = nthNewsSubstr(i+1, newsData, "~,~description");
        //Therefore as the title is between the titlelocation and the descrption location, use the substr function to subtract the two locations in order to just get the headline
        string title = newsData.substr (titleloc,descloc - titleloc) + "\n";

        //        cout << title << endl;

        //Keep the title lists in tact throughout each iteration of the loop and keep combining
        titleList = titleList + title;
    }
    //return the titleList which is all headlines ,once the for loop completes
    this->newsHeadlines = titleList;
    return titleList;
}

/**
* @brief Returns the headlines from the NewsWidget's API Call results
* @detailed Returns the headlines from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (news headlines)
* @parameters No parameters
* @returns A string containing the news headlines for use in the news widget
* @author Mitch Mancuso
*/
//Retrieves the actual headlines through manipulation to the formattedString - the number element allows for returning of multiple headlines.
string NewsWidget::getNewsHeadlines(int number){
    //Creates a copy of the formattedStringData in order to operate
    string newsDataTemp;
    newsDataTemp = this->newsData;
    //titleList will be a string keeping track of all of the headlines
    string titleList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < number; i++){
        //Use ints to keep track of the title location and description location in the string - in order to pull out the substring
        int titleloc, descloc;
        //The title will be located 8 after this string, based on this datas format - save this location
        titleloc = nthNewsSubstr(i+1, newsData, "title~:~") + 8;
//        cout << title1loc << endl;
        //The description will be located after this string, based on this datas format - save this location
        descloc = nthNewsSubstr(i+1, newsData, "~,~description");
        //Therefore as the title is between the titlelocation and the descrption location, use the substr function to subtract the two locations in order to just get the headline
        string title = newsData.substr (titleloc,descloc - titleloc) + "\n";

        //        cout << title << endl;

        //Keep the title lists in tact throughout each iteration of the loop and keep combining
        newsHeadline[number] = title;
    }
    //return the titleList which is all headlines ,once the for loop completes
    return newsHeadline[number];
}

/**
* @brief Returns the headlines from the NewsWidget's API Call results
* @detailed Returns the url from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (url)
* @parameters No parameters
* @returns A string containing the news story url for use in the news widget
* @author Mitch Mancuso
*/
//Similar process to the headlines where the urls are captures from the formattedString
string NewsWidget::getNewsUrls(){
    //Creates a copy of the formattedStringData in order to operate
    string newsDataTemp;
    newsDataTemp = this->newsData;
    //urlList will be a string keeping track of all of the URLs
    string urlList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < numberOfHeadlines; i++){
        //Use ints to keep track of the url location and description location in the string - in order to pull out the substring
        int urlloc, imgurlloc;
        //The url will be located 8 after this string, based on this datas format - save this location
        urlloc = nthNewsSubstr(i+1, newsData, ",~url~:~") + 8;
//        cout << title1loc << endl;
        //The to_image will be located after this string, based on this datas format - save this locati
        imgurlloc = nthNewsSubstr(i+1, newsData, "~,~urlToImage~:");
        //Therefore as the URL is between the url location and the url_image location, use the substr function to subtract the two locations in order to just get the URL
        string url = newsData.substr (urlloc,imgurlloc - urlloc) + "\n";

//        cout << url << endl;

        //return the URLlist which is all URLs ,once the for loop completes
        urlList = urlList + url;
    }
    this->newsUrls = urlList;
    return urlList;
}

/**
* @brief Returns the headlines from the NewsWidget's API Call results
* @detailed Returns the multiple urls from the APICallResults string by traversing
* through the weatherData variable and only picking out what string information is relevant (news urls)
* @parameters No parameters
* @returns A string containing the news urls for use in the news widget
* @author Mitch Mancuso
*/
//Retrieves the actual headlines through manipulation to the formattedString - the number element allows for returning of multiple URLs
string NewsWidget::getNewsUrls(int number){
    //Creates a copy of the formattedStringData in order to operate
    string newsDataTemp;
    newsDataTemp = this->newsData;
    //urlList will be a string keeping track of all of the URLs
    string urlList;
    //for each headline up to the user's requested amount
    for (int i = 0; i < number; i++){
        //Use ints to keep track of the url location and description location in the string - in order to pull out the substring
        int urlloc, imgurlloc;
        //The url will be located 8 after this string, based on this datas format - save this location
        urlloc = nthNewsSubstr(i+1, newsData, ",~url~:~") + 8;
//        cout << title1loc << endl;
        //The to_image will be located after this string, based on this datas format - save this locati
        imgurlloc = nthNewsSubstr(i+1, newsData, "~,~urlToImage~:");
        //Therefore as the URL is between the url location and the url_image location, use the substr function to subtract the two locations in order to just get the URL
        string url = newsData.substr (urlloc,imgurlloc - urlloc) + "\n";
        newsURLs[number] = url;
//        cout << url << endl;

        //return the URLlist which is all URLs ,once the for loop completes
    }
    return newsURLs[number];
}

/**
* @brief Prints the news information pulled from the API for use in testing
* @detailed Returns the news information like headlines and urls that are from the API calls
* @parameters none
* @returns void. Prints to the screen the results of getting the news information using the other functions in the class.
* @author Mitch Mancuso
*/
//A print statement that utilizes the same principles as the getTitle and getURL, but prints instead of returning a string
void NewsWidget::printNewsInformation(){
    string newsDataTemp;
    newsDataTemp = this->newsData;
    string infoList;
    for (int i = 0; i < numberOfHeadlines; i++){
        int titleloc, descloc;
        int urlloc, imgurlloc;
        titleloc = nthNewsSubstr(i+1, newsData, "title~:~") + 8;
//        cout << title1loc << endl;
        descloc = nthNewsSubstr(i+1, newsData, "~,~description");
        string title = newsData.substr (titleloc,descloc - titleloc) + "\n";

        urlloc = nthNewsSubstr(i+1, newsData, ",~url~:~") + 8;
//        cout << title1loc << endl;
        imgurlloc = nthNewsSubstr(i+1, newsData, "~,~urlToImage~:");
        string url = newsData.substr (urlloc,imgurlloc - urlloc) + "\n";

        //        cout << title << endl;
        //        cout << url << endl;

        infoList = infoList + title + " | " + url;
    }
    cout << "The headlines along with the URLs are:\n" << infoList << endl;
}
