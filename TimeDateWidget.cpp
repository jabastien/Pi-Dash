#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>
#include <termios.h>
#include <sys/ioctl.h>
#include "TimeDateWidget.h"

using namespace std;

/**
* @brief Default empty constructor for TimeDateWidget
* @detailed Creates an empty TimeDateWidget Widget as the default widget configuration
* @parameters Takes no parameters
* @returns An Empty TimeDate Widget
* @author Qi Liu
*/
TimeDateWidget::TimeDateWidget() {
}

/**
* @brief Default destructor for TimeDateWidget
* @detailed Removes the widget from within memory
* @parameters Takes no parameters
* @returns null
* @author Qi Liu
*/
TimeDateWidget::~TimeDateWidget() {
}

/**
* @brief Sends a value back to the widget using system sleep
* @detailed Allows the user to set a thread sleep time which is used for updating the time on the clock
* @parameters Takes milliseconds as a parameter
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::sleep(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::ctmcpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%r", td);
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::moncpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%B", td);
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::wkdcpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%A", td);
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Mitch Mancuso
*/
void TimeDateWidget::mndcpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%d", td);
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::yrcpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%Y", td);
}

/**
* @brief Gets the current time information from the system time and converts it to print
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters *tar - pointer where the information is copied | tm *td a set size of max characters to be copied | sz abbreviated options according to the current locale
* @returns none
* @author Qi Liu
*/
void TimeDateWidget::ydcpy(char *tar, struct tm *td, int sz) {
    strftime(tar, sz, "%j", td);
}

/**
* @brief Allows for a kill switch in the while loop to tell the system to exit based on the user's input
* @detailed Passes parameters to copy the current time information to the system using a format string
* @parameters none
* @returns byteswaiting which sends a killswitch if certain items are pressed on the keyboard (shity + left, etc)
* @author Qi Liu
*/
int TimeDateWidget::kbhit()
{
    static const int STDIN = 0;
    static bool init = false;

    if (!init)
    {
        //Uses termios functions to monitor when the user has sent a command to the system
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        init = true;
    }

    //Send the command forward through bytes waiting in order to be able to exit from the program
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

/**
* @brief Sets the time based on the user's preferences
* @detailed Allows the user to set their own time based on the timezone
* @parameters uses the cin to get input from the user
* @returns none
* @author Qi Liu
*/
//set time by user
void TimeDateWidget::setTime() {
    int hour=00;
    int minutes=00;
    int seconds=00;
    //difference between system time and input time
    cout << "The local time now is: " + getTime() << endl;;
    //ask input for hours
    cout << "Please type in the hour in 24hours" << endl;
    cin >> hour;
    //if the input is invalid, keep asking for input
    while(hour<0 or hour>23){
        cout << "Please enter a valid hour:" << endl;
        cin >> hour;
    }
    differenceHour = hour - sysHour;
    //ask for minutes input
    cout << "Please enter the minute:" << endl;
    cin >> minutes;
    //if the inout is not valid, keep asking for input
    while(minutes < 0 or minutes > 59){
        cout<<"Please enter a valid minutes:"<<endl;
        cin>>minutes;
    }
    differenceMin=minutes-sysMin;
    cout<<"You have set time to: " << getTime() <<endl;
}

/**
* @brief Returns the time as a string
* @detailed Returns the time as a string using a generated toString() function
* @parameters none
* @returns A string containing the current time
* @author Qi Liu
*/
//tostring method to return the time
string TimeDateWidget::timeToString() {
    return getTime();
}

/**
* @brief Returns the time
* @detailed Returns the date through the use of a getter
* @parameters none
* @returns A string containing the current time
* @author Qi Liu
*/
string TimeDateWidget::getTime(){
    sysHour=timeinfo->tm_hour;
    sysMin=timeinfo->tm_min;
    sysSec=timeinfo->tm_sec;
    //get local system time information
    string timeString;
    timeString = to_string(sysHour+differenceHour)+":"+to_string(sysMin+differenceMin)+":"+to_string(sysSec);
    return timeString;
}

/**
* @brief Returns the date
* @detailed Returns the date through the use of a getter
* @parameters none
* @returns A string containing the current date
* @author Qi Liu
*/
string TimeDateWidget::getDate() {
    char date[11];
    strftime(date,11,"%Y-%m-%d",localtime(&rawDate));
    return date;
}

/**
* @brief Returns the date as a string
* @detailed Returns the date as a string using a generated toString() function
* @parameters none
* @returns A string containing the current date
* @author Qi Liu
*/
string TimeDateWidget::dateToString() {
    string strdate=getDate();
    return strdate;
}