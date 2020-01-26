#ifndef NEWS_H
#define NEWS_H

#include <string>

/**
* @brief A class containing the public functions and private variables in the News Widget objects
* @detailed An overview of the structures behind the NewsWidget.cpp file
* @parameters Takes multiple - outlined in the NewsWidget.cpp documentation
* @returns Many (see NewsWidget.cpp for more information)
* @author Mitch Mancuso
*/

class NewsWidget {

    //h file declarations for global variables as well as getter functions

public:
    NewsWidget();
    NewsWidget(std::string, int, int);
    ~NewsWidget();

    std::string getNewsWidgetName();
    int getNewsWidgetID();
    int getNumberOfHeadlines();
    std::string getNewsHeadlines();
    std::string getNewsHeadlines(int);
    std::string getNewsUrls();
    std::string getNewsUrls(int);
    void printNewsInformation();
    std::string newsAPICallResults();

private:
    std::string newsWidgetName;
    std::string newsHeadlines;
    std::string newsUrls;
    int newsWidgetID;
    int numberOfHeadlines;
    std::string newsData;
    std::string newsHeadline[6];
    std::string newsURLs[6];
};


#endif
