#include <iostream>
#include <fstream>
#include <assert.h>
#include <ncurses.h>
#include <form.h>
#include <string>
#include <algorithm>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <locale.h>
#include <curl/curl.h>
#include "MainInterface.h"


/**
* @brief The main function's wrapper - starts the interface and controls ncurses.
* While the majority of the UI code exists in MainInterface, this starts the functions.
* @detailed Starts up the ncurses UI requirements, creates an interface object and starts with the welcome screen
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Mitch Mancuso
*/

int main(){
    initscr();
    noecho();
    cbreak();
    MainInterface m;
    m.welcomeScreen();

    getch();
    endwin();

    return 0;

}