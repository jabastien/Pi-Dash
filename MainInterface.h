#include <string>
#include "userSignIn.h"

/**
* @brief A header containing the public functions and private variables in the Main Interface instance
* @detailed An overview of the structures behind the MainInterface.cpp file
* @parameters Takes multiple - outlined in the MainInterface.cpp documentation
* @returns Many (see MainInterface.cpp for more information)
* @author Mitch Mancuso
*/

class MainInterface {

public:
    MainInterface();
    ~MainInterface();
    void welcomeScreen();
    void loginScreen();
    void loginHelper(int);
    void registerScreen();
    void registerHelper(int);
    char* trim_whitespaces(char *);
    void dashboardScreen();
    void weatherWidgetScreen();
    void stockWidgetScreen();
    void newsWidgetScreen();
    void timeAndDateWidgetScreen();
    void preferencesScreen();
    void preferencesHelper(int);

private:
    std::string userName, password, name, city, country, timeZone;
    int weather, stocks, news, timeanddate;
    int yMax, xMax;
    int yMaxWin, xMaxWin;
    userSignIn user;

};

