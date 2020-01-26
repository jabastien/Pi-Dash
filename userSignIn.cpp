#include "userSignIn.h"
#include "Preferences.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

/**
* @brief Handles the sign in functionality for the widgets
* @detailed Creates a userSignIn instance object with no data set
* @parameters Takes no parameters
* @returns An a userSignIn object
* @author Chao Chao
*/
userSignIn::userSignIn() {
    this->username = "";
    this->password = "";
    this->signInStatus = 0;
    this->name = "";
    this->country = "";
    this->city =  "";
    this->finance = 0;
    this->weather = 0;
    this->news = 0;
    this->date = 0;
    this->errorMessage = "";
}

/**
* @brief Handles sign in functionality when given a username and password
* @detailed Signs the user in when the inputs match the username and passwords outlined in userData.txt
* @parameters String username, String password
* @returns An a userSignIn object
* @author Chao Chao
*/
userSignIn::userSignIn(string username, string password){

    if(username == "" || password == ""){
        errorMessage = "ERROR: Username or Password is blank!";
        return;
    }
    else if(username.find(' ') != std::string::npos || password.find(' ') != std::string::npos){
        errorMessage = "ERROR: Username or Password cannot contain spaces!";
        return;
    }
    errorMessage = "";
    //Gets the length of the passed parameters in order to set array sizes
    int usernameLength = username.length();
    int passwordLength = password.length();

    //Creates multiple arrays in order to prepare to pull data from the text file
    char login_password[passwordLength + 1];
    char stored_password[passwordLength + 1];
    char login_username[usernameLength + 1];
    char stored_username[usernameLength + 1];
    char stored_name[25];
    char stored_city[25];
    char stored_country[25];
    char stored_timeZone[25];
    char stored_finance[5];
    char stored_weather[5];
    char stored_news[5];
    char stored_date[5];

    //Turns the username string into a char array
    strcpy(login_username, username.c_str());
    strcpy(login_password, password.c_str());

    //Pull information from the database
    fstream pull("userData.txt",ios::in);
    if (!pull) {
        //If unable to pull, do nothing
        //cout<<"File not loaded!"<<endl;
    }
    while(strcmp(login_username,stored_username)){

        pull>>stored_username;
        if(pull.eof()){
            //If the end of the file is reached, the username is not in the textfile
            //cout<<"Username does not exist. "<<endl;
            this->signInStatus = -2;
            return;
        }
        else{
            //Otherwise, store the username in a variable
            string username = string(stored_username);
        }
    }
    //Now pull the password next to it
    pull>>stored_password;
    //If these do not match...
    if(strcmp(stored_password,login_password) != 0){
        //cout<<"Wrong password. "<<endl;
        //Return and set the sign in status to -1 to indicate an error
        this->signInStatus = -1;
        return;
    }
    //Otherwise, if thet do match, pull the rest of the text file and load it into variables
    else {
        //cout << "Login successful." << endl;
        this->username = username;
        this->password = password;
        this->signInStatus = 0;
        pull>>stored_name;
        string name = string(stored_name);
        this->name = name;
        pull>>stored_city;
        string city = string(stored_city);
        this->city = city;
        pull>>stored_country;
        string country = string(stored_country);
        this->country = country;
        pull>>stored_timeZone;
        string timeZone = string(stored_timeZone);
        this->timeZone = timeZone;
        pull>>stored_finance;
        string finance = string(stored_finance);
        this->finance = stoi(finance);
        pull>>stored_weather;
        string weather = string(stored_weather);
        this->weather = stoi(weather);
        pull>>stored_news;
        string news = string(stored_news);
        this->news = stoi(news);
        pull>>stored_date;
        string date = string(stored_date);
        this->date = stoi(date);
        return;
    }
}

/**
* @brief Handles the sign in functionality when a new user is created
* @detailed Checks if the username and password exist, if not this will create a user object that has its preferences set and written to the userData.txt file
* @parameters String username, String password, String name, String city, String country, string timeZone, int finance, int weather, int news, int date
* @returns An a userSignIn object
* @author Chao Chao
*/
userSignIn::userSignIn(string username, string password, string name, string city, string country, string timeZone, int finance, int weather, int news, int date){

    if(username == "" || password == "" || name =="" || city == "" || country=="" || timeZone == ""){
        errorMessage = "ERROR: One or more of your fields is blank!";
        return;
    }
    else if(username.find(' ') != std::string::npos || password.find(' ') != std::string::npos || name.find(' ') != std::string::npos || city.find(' ') != std::string::npos || country.find(' ') != std::string::npos || timeZone.find(' ') != std::string::npos){
        errorMessage = "ERROR: Fields cannot contain spaces!";
        return;
    }
    else if(country.length() > 2){
        errorMessage = "ERROR: Please enter a two character country code!";
        return;
    }
    errorMessage = "";
    //Create an int and check if the user exists given the username string
   int userCheck = getUserExists(username);
   //If the user exists, set the variable userCheck to -1 indicating that registration cannot continue
    if (userCheck == -1) {
        //cout << "User already exists." << endl;
        this->userExists = 1;
        return;
    }
    //Otherwise, allow the registration to continue and set the variables to what has been passed in the constructor
    else {
        //cout << "User does not already exist." << endl;
        this->userExists = 0;
        this->username = username;
        this->password = password;
        this->signInStatus = 0;
        this->name = name;
        this->city = city;
        this->country = country;
        this->timeZone = timeZone;
        this->finance = finance;
        this->weather = weather;
        this->news = news;
        this->date = date;
        return;
    }
}

/**
* @brief Default empty destructor for userSignIn
* @detailed Destroys userSignIn in memory
* @parameters Takes no parameters
* @author Chao Chao
*/
userSignIn::~userSignIn() {
}

/**
* @brief Handles the functionality to tell if the user is logged in
* @detailed Should return an int if the user is signed in (0) , negative values are reserved for errors
* @parameters Takes no parameters
* @returns An int that is 0 for success, -1 for incorrect password, and -2 for no username exists
* @author Mitch Mancuso
*/
int userSignIn::getSignInStatus(){
    return this->signInStatus;
}

/**
* @brief Handles the getUserExists functionality for the user
* @detailed This is used during the user registratin process to determine whether or not the username exists in the text file
* @parameters Takes a string username
* @returns An int that determines if the user exists, -1 for no, 1 for file read error, and 0 for user does not exist
* @author Chao Chao
*/
int userSignIn::getUserExists(string username) {

    //Determine the length of the passed username in order to set the array sizes
    int usernameLength = username.length();
    //Create arrays that will be used for setting and getting data
    char login_username[usernameLength + 1];
    char stored_username[usernameLength + 1];

    //Copy the string into the char array
    strcpy(login_username, username.c_str());

    //Pull the information from the userData.txt file
    fstream pull("userData.txt", ios::in);
    if (!pull) {
        return 1;
        //cout<<"File not loaded!"<<endl;
    }
    //If the file reaches the end, it means the username does not exist - return -1
    while (strcmp(login_username, stored_username)) {
        pull >> stored_username;
        if (pull.eof()) {
            //cout<<"Username does not exist. "<<endl;
            return -1;
        }
    }
    //Otherwise it does, return 0
    return 0;
}

/**
* @brief Handles the getUserName functionality for the user
* @detailed Gets the userName and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the userName
* @author Chao Chao
*/
string userSignIn::getUserName() {
    return this->username;
}

/**
* @brief Handles the getPassword functionality for the user
* @detailed Gets the password and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the password
* @author Chao Chao
*/
string userSignIn::getPassword() {
    return this->password;
}

/**
* @brief Handles the getName functionality for the user
* @detailed Gets the name and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the name
* @author Chao Chao
*/
string userSignIn::getName() {
    return this->name;
}

/**
* @brief Handles the getCity functionality for the user
* @detailed Gets the city and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the city
* @author Chao Chao
*/
string userSignIn::getCity() {
    return this->city;
}

/**
* @brief Handles the getCountry functionality for the user
* @detailed Gets the country and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the country
* @author Chao Chao
*/
string userSignIn::getCountry() {
    return this->country;
}

/**
* @brief Handles the timeZone functionality for the user
* @detailed Gets the timeZone and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the timeZone
* @author Chao Chao
*/
string userSignIn::getTimeZone() {
    return this->timeZone;
}

/**
* @brief Handles the getFinance functionality for the user
* @detailed Gets the finance widget status and stores it as an int
* @parameters Takes no parameters
* @returns An int that is the finance widget status
* @author Chao Chao
*/
int userSignIn::getFinance() {
    return this->finance;
}

/**
* @brief Handles the getNews functionality for the user
* @detailed Gets the news widget status and stores it as an int
* @parameters Takes no parameters
* @returns An int that is the news widget status
* @author Chao Chao
*/
int userSignIn::getNews() {
    return this->news;
}

/**
* @brief Handles the getWeather functionality for the user
* @detailed Gets the weather widget status and stores it as an int
* @parameters Takes no parameters
* @returns An int that is the weather widget status
* @author Chao Chao
*/
int userSignIn::getWeather() {
    return this->weather;
}

/**
* @brief Handles the getDate functionality for the user
* @detailed Gets the date widget status and stores it as an int
* @parameters Takes no parameters
* @returns An int that is the date widget status
* @author Chao Chao
*/
int userSignIn::getDate() {
    return this->date;
}

/**
* @brief Handles the getErrorMessage functionality for the user
* @detailed Gets the errorMessage and stores it as a string
* @parameters Takes no parameters
* @returns A string that is the error message
* @author Chao Chao
*/
string userSignIn::getErrorMessage() {
    return this->errorMessage;
}

/**
* @brief Handles the setUserName functionality for the user
* @detailed Sets the username based on the user's input
* @parameters Takes no parameters
* @returns A string that is the new set setUserName
* @author Chao Chao
*/
void userSignIn::setUserName(string username){
    this->username = username;
}

/**
* @brief Handles the setPassword functionality for the user
* @detailed Sets the password based on the user's input
* @parameters Takes no parameters
* @returns A string that is the new set setPassword
* @author Chao Chao
*/
void userSignIn::setPassword(string password){
this->password = password;
}

/**
* @brief Sets the user set name depending on the passed parameter
* @detailed Sets the user set name based on the newCity string input
* @parameters int newName
* @author Chao Chao
*/
void userSignIn::changeName(string newName) {
    this->preference.changeName(newName);
}

/**
* @brief Sets the user set country depending on the passed parameter
* @detailed Sets the user set country based on the newCity string input
* @parameters int newCountry
* @author Chao Chao
*/
void userSignIn::changeCountry(string newCountry) {
    this->preference.changeCountry(newCountry);
}

/**
* @brief Sets the user set city depending on the passed parameter
* @detailed Sets the user set city based on the newCity string input
* @parameters int newCity
* @author Chao Chao
*/
void userSignIn::changeCity(string newCity) {
    this->preference.changeCity(newCity);
}

/**
* @brief Sets the Finance Widget to on or off depending on the user's preferences
* @detailed Sets the Finance Widget based on the status input which will be 1 for on and 0 for off
* @parameters int status which is 1 or 0
* @author Chao Chao
*/
void userSignIn::turnFinance(int status) {
    this->preference.turnFinance(status);
}

/**
* @brief Sets the Weather Widget to on or off depending on the user's preferences
* @detailed Sets the Weather Widget based on the status input which will be 1 for on and 0 for off
* @parameters int status which is 1 or 0
* @author Chao Chao
*/
void userSignIn::turnWeather(int status) {
    this->preference.turnWeather(status);
}

/**
* @brief Sets the News Widget to on or off depending on the user's preferences
* @detailed Sets the News Widget based on the status input which will be 1 for on and 0 for off
* @parameters int status which is 1 or 0
* @author Chao Chao
*/
void userSignIn::turnNews(int status) {
    this->preference.turnNews(status);
}

/**
* @brief Sets the TimeAndDate Widget to on or off depending on the user's preferences
* @detailed Sets the TimeAndDate Widget based on the status input which will be 1 for on and 0 for off
* @parameters int status which is 1 or 0
* @author Chao Chao
*/
void userSignIn::turnDate(int status) {
    this->preference.turnDate(status);
}

/**
* @brief Writes items to the file that is updated by the user
* @detailed Gets a preference object that stores all user preferences
* @parameters Takes no parameters
* @returns An integer upon success or failure
* @author Zhaohe Zhang
*/
int userSignIn::writeToFile() {
    fstream dataWriteFile;
    dataWriteFile.open("userData.txt", fstream::in | fstream::out | fstream::app);
    if(!dataWriteFile.is_open()) {
        cout << "Cannot open data file." << endl;
        return -1;
    }
    string textToWrite = username + "|" + password + "|" + preference.toString();
    dataWriteFile << textToWrite << endl;
    dataWriteFile.close();
    return 0;
}

/**
* @brief Gets the user preferences
* @detailed Gets a preference object that stores all user preferences
* @parameters Takes no parameters
* @returns A preference object
* @author Zhaohe Zhang
*/
//IMPORTANT, use this getPreference and do changes WILL NOT change the preference for the user
Preferences userSignIn::getPreference() {   //use above change functions to change
    return this->preference;
}