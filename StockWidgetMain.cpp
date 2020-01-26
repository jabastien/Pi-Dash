#include <iostream>
#include "StockWidget.h"

using namespace std;

/**
* @brief A test function set for the stock widgets
* @detailed Tests the stock widget's ability to test and pull information from the API
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Mitch Mancuso
*/
int main() {

    //A test function set that allows for the user to run the stock widget on the back-end
    int userInputNumber;
    string userInputSymbol;
    cout << "Enter the number of stocks you would like a price for (max 10): " << endl;
    while (true) {
        cin >> userInputNumber;
        if (userInputNumber > 10)
            cout << "Please enter a valid input (1-10):\n";
        else {
            break;
        }
    }
            for (int i = 0; i < userInputNumber; i++) {
                //Asks the user to input a stock symbol in order to retreive it through system calls
                cout << "Enter the stock symbol you would like the price for (like AMZN), or type EXIT to exit." << endl;
                cin >> userInputSymbol;
                //If the user wants to exit, print goodbye and exit
                if (userInputSymbol == "EXIT" || userInputSymbol == "exit" || userInputSymbol == "Exit") {
                    cout << "Goodbye!\n";
                    return 0;
                }
                else {
                    //Retrieve lines for multiple stocks - if a user would like to get 2 or 3, etc
                    cout << "Retrieving current price for " + userInputSymbol << endl;
                    cout << "Please wait a moment..." << endl;
                    if(i==0){
                        StockWidget s = StockWidget("Test Widget", 1, userInputSymbol);
                        cout << "Widget name is: " << s.getStockWidgetName() << endl;
                        cout << "Widget ID is: " << s.getStockWidgetID() << endl;
                    }
                    StockWidget s = StockWidget("Test Widget", 1, userInputSymbol);
                    cout << "Widget stock symbol is: " << s.getStockSymbol() << endl;
                    cout << "Stock price of widget's stock symbol is: " << s.getStockPrice() << endl;
                    //Standard print statement from Stock class
                    s.printStockPrice();
                }
            }
    return 0;
    }