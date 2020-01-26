#ifndef STOCKS_H
#define STOCKS_H

#include <string>

/**
* @brief A class containing the public functions and private variables in the Stock Widget objects
* @detailed An overview of the structures behind the StockWidget.cpp file
* @parameters Takes multiple - outlined in the StockWidget.cpp documentation
* @returns Many (see StockWidget.cpp for more information)
* @author Mitch Mancuso
*/

class StockWidget {

    //h file declarations for global variables as well as getter functions

public:
    StockWidget();
    StockWidget(std::string, int, std::string);
    ~StockWidget();

    std::string getStockWidgetName();
    int getStockWidgetID();
    std::string getStockSymbol();
    std::string getStockPrice();
    void printStockPrice();
    std::string stockAPICallResults(std::string);

private:
    std::string stockWidgetName;
    int stockWidgetID;
    int stockWidgetMode;
    std::string stockSymbol;
    std::string stockPrice;
    std::string stockData;

};


#endif
