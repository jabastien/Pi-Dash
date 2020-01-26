#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

/**
* @brief A class containing the public functions and private variables in the TimeDate Widget objects
* @detailed An overview of the structures behind the TimeDateWidget.cpp file
* @parameters Takes multiple - outlined in the TimeDateWidget.cpp documentation
* @returns Many (see TimeDateWidget.cpp for more information)
* @author Qi Liu
*/

class TimeDateWidget {


public:

    TimeDateWidget();
    ~TimeDateWidget();
    void sleep(int);
    void ctmcpy(char *, struct tm *, int);
    void moncpy(char *, struct tm *, int);
    void wkdcpy(char *, struct tm *, int);
    void mndcpy(char *, struct tm *, int);
    void yrcpy(char *, struct tm *, int);
    void ydcpy(char *, struct tm *, int);
    int kbhit();
    void setTime();
    string getTime();
    string timeToString();
    string getDate();
    string dateToString();

private:
    time_t rawTime;
    struct tm*timeinfo;
    int differenceHour;
    int differenceMin;
    int sysHour;
    int sysMin;
    int sysSec;
    time_t rawDate;
};