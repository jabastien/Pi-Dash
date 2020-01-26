/**
* @brief A test function set for the preferences functionality
* @detailed Tests the preferences  ability to test and pull and set information
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Zhaohe Zhang
*/

#include "Preferences.h"
using namespace std;

int main() {
    bool on = 1;
    bool off = 0;
    Preferences p = Preferences("Testing", "Canada", "Toronto", 1, 1, 1, 1);
    cout << "Name: " << p.getName() << endl;
    cout << "Country: " << p.getCountry() << endl;
    cout << "City: " << p.getCity() << endl;
    if(p.changeName("Test") == -1) {
        cout << "Error changing user name." << endl;
    }
    else {
        cout << "Name changed to " << p.getName() << endl;
    }

    if(p.changeCountry("England") == -1) {
        cout << "Error changing country." << endl;
    }
    else {
        cout << "Location changed to " << p.getCountry() << endl;
    }

    if(p.changeCity("London") == -1) {
        cout << "Error changing location." << endl;
    }
    else {
        cout << "City changed to " << p.getCity() << endl;
    }

    if(p.turnFinance(off) == -1) {
        cout << "Error turning finance panel off." << endl;
    }

    if(p.turnWeather(off) == -1) {
        cout << "Error turning weather panel off." << endl;
    }

    if(p.turnNews(off) == -1) {
        cout << "Error turning news panel off." << endl;
    }

    if(p.turnDate(off) == -1) {
        cout << "Error turning time and date panel off." << endl;
    }

    string preferences = p.getPreferences();
    bool status = 0;
    for(int i=0; i<preferences.size(); i++) {
        if(preferences[i] == '1') {
            status = 1;
        }
    }
    cout << "All panels are turned " << (status?"ON":"OFF") << endl;

    if(p.turnFinance(on) == -1) {
        cout << "Error turning finance panel on." << endl;
    }

    if(p.turnWeather(on) == -1) {
        cout << "Error turning weather panel on." << endl;
    }

    if(p.turnNews(on) == -1) {
        cout << "Error turning news panel on." << endl;
    }

    if(p.turnDate(on) == -1) {
        cout << "Error turning date panel on." << endl;
    }

    cout << "All panels are turned " << (status?"ON":"OFF") << endl;
}

