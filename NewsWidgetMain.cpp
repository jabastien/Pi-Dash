#include <iostream>
#include "NewsWidget.h"

//This file through use of the CMakeLists.txt will create an executable file that allows for the News Widget functionality to be tested on the back-end

using namespace std;

/**
* @brief A test function set for the news widget
* @detailed Tests the news widget's ability to test and pull information from the API
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Mitch Mancuso
*/

int main() {
    int userInputNumber;
    cout << "Enter the number of headlines you would like (max 10): " << endl;
    while (true)
    {
        cin >> userInputNumber;
        if (userInputNumber > 10)
            cout << "Please enter a valid input (1-10):\n";
        else
        break;
    }
    //Asks the user for a number of headlines up to a maximum of 10 due to API limitations.
    //Once the user enters a number of headlines, they are returned along with the URL and debugging informaiton for the widget
    cout << "Retrieving " << userInputNumber << " top headlines." << endl;
    cout << "Please wait a moment..." << endl;
    NewsWidget n = NewsWidget("Test Widget", 2, userInputNumber);
    //Printing of widget get statements to test
    cout << "Widget name is: " << n.getNewsWidgetName() << endl;
    cout << "Widget ID is: " << n.getNewsWidgetID() << endl;
    cout << "Widget number of headlines is/are: " << n.getNumberOfHeadlines() << "\n\n";
    cout << "Headlines from the widget are:\n" << n.getNewsHeadlines() << endl;
    cout << "URLs from the widget are:\n" << n.getNewsUrls() << endl;
    cout << "Headlines from the widget are:\n" << n.getNewsHeadlines(1) << endl;
    cout << "URLs from the widget are:\n" << n.getNewsUrls(1) << endl;
    //Prints the void news method
    n.printNewsInformation();
}
