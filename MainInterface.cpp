#include <iostream>
#include <fstream>
#include <form.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <regex>
#include <cstdlib>
#include <assert.h>
#include <ncurses.h>
#include <locale.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <curl/curl.h>

#include "MainInterface.h"
#include "WeatherWidget.h"
#include "StockWidget.h"
#include "NewsWidget.h"
#include "TimeDateWidget.h"
#include "userSignIn.h"

using namespace std;

//Sets up global variables for use in the main interface, primary NCURSES fields, WINDOWS and FORMS

static FORM *form;
static FORM *regForm;
static FIELD *fields[5];
static FIELD *regFields[13];
static WINDOW *win_body, *win_form, *menuwin, *menuheader, *userNameWin;

/**
* @brief Default empty constructor for Main Interface
* @detailed Creates an empty Main Interface as the default configuration
* @parameters Takes no parameters
* @returns An Empty Main Interface
* @author Mitch Mancuso
*/
MainInterface::MainInterface(){
}

/**
* @brief Default empty destructor for Main Interface
* @detailed Destroys Main Interface in memory
* @parameters Takes no parameters
* @author Mitch Mancuso
*/
MainInterface::~MainInterface(){
}

/**
* @brief Creates the welcomeScreen using ncurses
* @detailed Sets the windows and draws the front-end using ASCII and positioning
* @parameters Takes no parameters
* @returns void. Does call the login function, register function, or exits based on user selection
* @author Mitch Mancuso
*/
void MainInterface::welcomeScreen() {
    refresh();
    wrefresh(menuheader);
    wrefresh(menuwin);
    //Gets the max size of the window and stores it in order to draw things relatively
    getmaxyx(stdscr, yMax, xMax);
    //Creates a menuHeader window and a menuWin window - the header will store the dashboard title and the window will store the menu
    menuheader = newwin(6, 66, ((yMax / 3) - (4 / 2)), ((xMax / 2) - (66 / 2)));
    menuwin = newwin(6, 30, ((yMax / 2) - (6 / 2)), ((xMax / 2) - (30 / 2)));
    //Draw a box around the menu
    box(menuwin, 0, 0);
    //Print the title for dashboard (ASCII art)
    mvwprintw(menuheader, 0, 5, "    ___   _   ___ _  _ ___  ___   _   ___ ___    ___ _ ");
    mvwprintw(menuheader, 1, 5, "   |   \\ /_\\ / __| || | _ )/ _ \\ /_\\ | _ \\   \\  |_  ) |");
    mvwprintw(menuheader, 2, 5, "   | |) / _ \\\\__ \\ __ | _ \\ (_) / _ \\|   / |) |  / /| |");
    mvwprintw(menuheader, 3, 5, "   |___/_/ \\_\\___/_||_|___/\\___/_/ \\_\\_|_\\___/  /___|_|");
    mvwprintw(menuheader, 5, 5, "Select an option with the arrow keys and press [  ENTER  ]");
    //Refresh the screen to update it with all elements
    wrefresh(menuwin);
    wrefresh(menuheader);
    //Get ready to store user inputs
    keypad(menuwin, true);
    //Print 3 choices on the screen, and store the current location of the cursor
    string choices[3] = {"[ Login ]","[ Register ]", "[ Exit ]"};
    int choice;
    int highlight = 0;
    //Infinite loop that depends on user input, depending on the key pressed - choose a different action
    while (1) {
        //Control the highlight function to highlight the choice the user is currently on
        for (int i = 0; i < 3; i++) {
            if (i == highlight) {
                //wattron styling to highlight
                wattron(menuwin, A_REVERSE);
            }
            //Choose how the highlight moves throughout the execution of the program
            mvwprintw(menuwin, i + 1, 1, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        //Store the user's choice
        choice = wgetch(menuwin);

        //Base the output on the up, down keys - and move the highlight accordingly
        switch (choice) {
            //Moves the highlight location depending on the arrow keys
            case KEY_UP:
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 3) {
                    highlight = 2;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            break;
        }
    }
    //If the user decides to exit, then exit the program and close the windows
    if (choices[highlight] == "[ Exit ]") {
        endwin();
        exit(0);
    }
    //If the user chooses to login, go to the login screen function loginScreen()
    else if (choices[highlight] == "[ Login ]") {
        werase(menuheader);
        refresh();
        loginScreen();
    }
    //If the user chooses to login, go to the register screen function registerScreen()
    else if (choices[highlight] == "[ Register ]") {
        werase(menuheader);
        refresh();
        registerScreen();
    }
}

/**
* @brief Creates the loginScreen using ncurses
* @detailed Sets the windows and draws the front-end loginScreen with a form to get the userName and password from the user
* @parameters Takes no parameters
* @returns void. Does call the form window functions to store the userName and password
* Also draws an ASCII style header for the login screen as a UI element
* @author Mitch Mancuso
*/
void MainInterface::loginScreen(){
    refresh();
    wrefresh(menuheader);
    wrefresh(menuwin);
    //Allows for user input to be stored in ch
    int ch;
    keypad(stdscr, TRUE);
    //Gets the max size of the window and stores it in order to draw things relatively
    getmaxyx(stdscr, yMax, xMax);
    //Draws the ASCII art for the header to display login
    mvwprintw(stdscr, 2, ((xMax/2) - 15), "    _    ___   ___ ___ _  _ ");
    mvwprintw(stdscr, 3, ((xMax/2) - 15), "   | |  / _ \\ / __|_ _| \\| |");
    mvwprintw(stdscr, 4, ((xMax/2) - 15), "   | |_| (_) | (_ || || .` |");
    mvwprintw(stdscr, 5, ((xMax/2) - 15), "   |____\\___/ \\___|___|_|\\_|");

    //Create a body that will hold the form for the user login
    win_body = newwin(24, 80, ((yMax/2)-(24/2)), ((xMax/2)-(80/2)));
    assert(win_body != NULL);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 12, 70, 6, 1);
    getmaxyx(win_body, yMaxWin, xMaxWin);
    //Prints the commands to go back, exit, confirm - for the user to see
    mvwprintw(win_body, 3, ((xMaxWin/2)-(32/2)),"[  SHIFT + LEFT ] : Main Menu");
    mvwprintw(win_body, 4, ((xMaxWin/2)-(21/2)),"[  ENTER  ] : Confirm");
    mvwprintw(win_body, 5, ((xMaxWin/2)-(16/2)),"[  F12  ] : Exit");
    mvwprintw(win_body, 18, ((xMaxWin/2)-(13/2)),"[   LOGIN   ]");

    //Sets and stores fields for username and password
    fields[0] = new_field(1, 10, 4, 10, 0, 0);
    fields[1] = new_field(1, 40, 4, 25, 0, 0);
    fields[2] = new_field(1, 10, 6, 10, 0, 0);
    fields[3] = new_field(1, 40, 6, 25, 0, 0);
    fields[4] = NULL;
    assert(fields[0] != NULL && fields[1] != NULL && fields[2] != NULL && fields[3] != NULL);

    //Displays the text friendly username, password areas and stores the results in an array
    set_field_buffer(fields[0], 0, "Username:");
    set_field_buffer(fields[1], 0, "");
    set_field_buffer(fields[2], 0, "Password:");
    set_field_buffer(fields[3], 0, "");

    //Chooses the visibility of text fields, the password field will not be visible and has autoskip enabled for security
    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[3], O_VISIBLE | O_EDIT | O_ACTIVE);

    //Underlines the fields to make it look more like a form
    set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[3], A_UNDERLINE);

    //Creates the form and positions it on the screen
    form = new_form(fields);
    assert(form != NULL);
    set_form_win(form, win_form);
    set_form_sub(form, derwin(win_form, 18, 76, 2, 2));
    post_form(form);

    //Refreshes the form to show it on screen
    refresh();
    wrefresh(win_body);
    wrefresh(win_form);

    //While the user has not decided to exit, run the loginHelper function which will choose what to do based on input
    while ((ch = getch()) != KEY_SLEFT) {
        loginHelper(ch);
    }
    //Once the while loop has completed, clear, refresh and destroy the elements on screen - go back to the welcome screen
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);
    free_field(fields[3]);
    clear();
    refresh();
    welcomeScreen();
}

/**
* @brief Allows for a login helper that determines if the user's information is valid and pushes the function through the login stage
* @detailed Allows the user to login and based on the user's inputs may exit, enter, or quit the program entirely
* @parameters char ch is the user's keystroke input which is monitored against the system options
* @returns void. Calls other functions and displays things like error messages if the user password is incorrect, etc
* @author Mitch Mancuso
*/
void MainInterface::loginHelper(int ch){
    //Define variables to use in the function
    int i, y, x;
    string errorDescription;
    user = userSignIn();

    //Choose the option to take based off of the user's input
    switch (ch) {
        //If the user inputs F8, this is used for general debugging - it will print the username and password on screen
        case KEY_F(8):
            // Uses the form_driver to sync actions to the screen
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_PREV_FIELD);
            //Chooses user move options
            move(LINES-3, 2);

            //Prints the fields as entered by the user
            for (i = 0; fields[i]; i++) {
                printw("%s", trim_whitespaces(field_buffer(fields[i], 0)));

                if (field_opts(fields[i]) & O_ACTIVE)
                    printw("\"\t");
                else
                    printw(": \"");
            }
            refresh();
            //positions the form's cursor
            pos_form_cursor(form);
            break;

        case 10:
            //If the user enters, check the username and password - these should load from a text file to see if they exist or are correct
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_PREV_FIELD);
            move(LINES-3, 2);
            //The following two functions compare the username and password against the text file to see if they exist
            //NOTE: admin and password are used as test functions
            user = userSignIn(trim_whitespaces(field_buffer(fields[1], 0)),trim_whitespaces(field_buffer(fields[3],0)));
            //If the userName exists and password exists, allow the user to login
            if(user.getSignInStatus() == 0){
                    getyx(stdscr, y, x); // save current pos
                    move(y, 0);          // move to beginning of line
                    clrtoeol();          // clear line
                    move(y, x);          // move back to where you were
                    //print a success message for the user
                    printw("SUCCESS: YOU WILL NOW BE LOGGED IN!");
                    //Store the userName and password in the global variable
                    userName = user.getUserName();
                    password = user.getPassword();
                    //Get the user attributes from the text file and set them to the global variables
                    name = user.getName();
                    city = user.getCity();
                    country = user.getCountry();
                    timeZone = user.getTimeZone();
                    weather = user.getWeather();
                    stocks = user.getFinance();
                    news = user.getNews();
                    timeanddate = user.getDate();
                    wclear(win_form);
                    wrefresh(win_form);
                    clear();
                    refresh();
                    //Go to the main dashboard
                    dashboardScreen();
            }
            //Otherwise, the username does not exist
            else{
                getyx(stdscr, y, x); // save current pos
                move(y, 0);          // move to begining of line
                clrtoeol();          // clear line
                move(y, x);          // move back to where you were
                if(user.getSignInStatus() == -2) {
                    printw("ERROR: NO USER FOUND!");
                }
                else if(user.getSignInStatus() == -1){
                    printw("ERROR: PASSWORD IS INCORRECT!");
                }
                else if(user.getErrorMessage() != ""){
                    errorDescription = user.getErrorMessage();
                    printw("%s",errorDescription.c_str());
                }
            }
            refresh();
            pos_form_cursor(form);
            break;
            //If the user enters F12, then exit the program completely
        case KEY_F(12):
            endwin();
            exit(0);
            //Down arrow will go to the next field
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
            //The up arrow will go to the previous field
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        //The left key will go to the previous character
        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;
        //The right key will go to the next character
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;

            // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;

            // Delete the char under the cursor
        case KEY_DC:
            form_driver(form, REQ_DEL_CHAR);
            break;

        default:
            form_driver(form, ch);
            break;
    }
//Refresh the form on the screen
    wrefresh(win_form);
}

/**
* @brief Creates the registrationScreen using ncurses
* @detailed Sets the windows and draws the front-end registrationScreen with a form to get the userName, Password, City, Country Code, Etc from the user
* @parameters Takes no parameters
* @returns void. Does call the form window functions to store the userName, password, name, country character code
* Also draws an ASCII style header for the login screen as a UI element
* @author Mitch Mancuso
*/
void MainInterface::registerScreen() {
    //Allows for user input through the keypad
    int ch;
    keypad(stdscr, TRUE);
    //Determines the size of the window for placement of elemenets
    getmaxyx(stdscr, yMax, xMax);

    //Draws an ASCII style header for the registration screen
    mvwprintw(stdscr, 2, ((xMax/2) - 20), "    ___ ___ ___ ___ ___ _____ ___ ___ ");
    mvwprintw(stdscr, 3, ((xMax/2) - 20), "   | _ \\ __/ __|_ _/ __|_   _| __| _ \\");
    mvwprintw(stdscr, 4, ((xMax/2) - 20), "   |   / _| (_ || |\\__ \\ | | | _||   /");
    mvwprintw(stdscr, 5, ((xMax/2) - 20), "   |_|_\\___\\___|___|___/ |_| |___|_|_\\");

    win_body = newwin(30, 80, 8, ((xMax / 2) - (80/2)));

    //Starts the creation of a form to be able to enter the user registration information
    assert(win_body != NULL);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 15, 70, 6, 3);
    getmaxyx(win_body, yMaxWin, xMaxWin);
    //Prints prompts for the user with information how to exit, enter, etc
    mvwprintw(win_body, 3, ((xMaxWin/2)-(32/2)),"[  SHIFT + LEFT ] : Main Menu");
    mvwprintw(win_body, 4, ((xMaxWin/2)-(21/2)),"[  ENTER  ] : Confirm");
    mvwprintw(win_body, 5, ((xMaxWin/2)-(16/2)),"[  F12  ] : Exit");
    mvwprintw(win_body, 24, ((xMaxWin / 2) - (16 / 2)), "[   REGISTER   ]");

    //Creates sets of fields that will be used to store the various data points
    regFields[0] = new_field(1, 10, 4, 10, 0, 0);
    regFields[1] = new_field(1, 40, 4, 25, 0, 0);
    regFields[2] = new_field(1, 10, 6, 10, 0, 0);
    regFields[3] = new_field(1, 40, 6, 25, 0, 0);
    regFields[4] = new_field(1, 10, 8, 10, 0, 0);
    regFields[5] = new_field(1, 40, 8, 25, 0, 0);
    regFields[6] = new_field(1, 10, 10, 10, 0, 0);
    regFields[7] = new_field(1, 40, 10, 25, 0, 0);
    regFields[8] = new_field(1, 12, 12, 10, 0, 0);
    regFields[9] = new_field(1, 40, 12, 25, 0, 0);
    regFields[10] = new_field(1, 10, 14, 10, 0, 0);
    regFields[11] = new_field(1, 40, 14, 25, 0, 0);
    regFields[12] = NULL;
    assert(regFields[0] != NULL && regFields[1] != NULL && regFields[2] != NULL && regFields[3] != NULL && regFields[4] != NULL && regFields[5] != NULL && regFields[6] != NULL && regFields[7] != NULL && regFields[8] != NULL && regFields[9] != NULL && regFields[10] != NULL && regFields[11] != NULL);

    //Sets the fields and data to be empty in the storage array fields and the information type in the array spot before it
    set_field_buffer(regFields[0], 0, "Username:");
    set_field_buffer(regFields[1], 0, "");
    set_field_buffer(regFields[2], 0, "Password:");
    set_field_buffer(regFields[3], 0, "");
    set_field_buffer(regFields[4], 0, "Name:");
    set_field_buffer(regFields[5], 0, "");
    set_field_buffer(regFields[6], 0, "City:");
    set_field_buffer(regFields[7], 0, "");
    set_field_buffer(regFields[8], 0, "Country Code:");
    set_field_buffer(regFields[9], 0, "");
    set_field_buffer(regFields[10], 0, "Time Zone:");
    set_field_buffer(regFields[11], 0, "");

    //Sets the visibility attributes of the form, the password field will be hidden for security upon input
    set_field_opts(regFields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[3], O_VISIBLE | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[4], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[5], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[6], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[7], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[8], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[9], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[10], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[11], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    //The fields where entry will be expected will have the ability to be underlined
    set_field_back(regFields[1], A_UNDERLINE);
    set_field_back(regFields[3], A_UNDERLINE);
    set_field_back(regFields[5], A_UNDERLINE);
    set_field_back(regFields[7], A_UNDERLINE);
    set_field_back(regFields[9], A_UNDERLINE);
    set_field_back(regFields[11], A_UNDERLINE);

    //Create and set the form information - post the form
    regForm = new_form(regFields);
    assert(regForm != NULL);
    set_form_win(regForm, win_form);
    set_form_sub(regForm, derwin(win_form, 145, 54, 2, 2));
    post_form(regForm);

    //Refresh the form on screen
    refresh();
    wrefresh(win_body);
    wrefresh(win_form);

    while ((ch = getch()) != KEY_SLEFT)
        registerHelper(ch);
    unpost_form(regForm);
    free_form(regForm);
    free_field(regFields[0]);
    free_field(regFields[1]);
    free_field(regFields[2]);
    free_field(regFields[3]);
    free_field(regFields[4]);
    free_field(regFields[5]);
    free_field(regFields[6]);
    free_field(regFields[7]);
    free_field(regFields[8]);
    free_field(regFields[9]);
    free_field(regFields[10]);
    free_field(regFields[11]);
    clear();
    refresh();
    //Send the user back to the welcome screen if they hit shift + left
    welcomeScreen();
}

/**
* @brief Allows for a register helper that determines if the user's information is valid and pushes the function through the register stage
* @detailed Allows the user to register and based on the user's inputs may exit, enter, or quit the program entirely
* @parameters char ch is the user's keystroke input which is monitored against the system options
* @returns void. Calls other functions and displays things like error messages if the information is blank, userName already exists, etc
* @author Mitch Mancuso
*/
void MainInterface::registerHelper(int ch){
    //Declares variables to hold values of window size, the password the user wishes to implement and their selected username
    int i, y, x, userNameExists;
    string tempUserName, tempPass, tempName, tempCity, tempCountry, tempTimeZone, errorDescription;
    user = userSignIn();
    //Monitor's the user input to determine next steps to take
    switch (ch) {
        //F8 is used to debug text input and is not mentioned on the main screen
        case KEY_F(8):
            // Or the current field buffer won't be sync with what is displayed
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_PREV_FIELD);
            move(LINES-3, 2);
        //Prints fields removing whitespaces on the screen for easy viewing
            for (i = 0; regFields[i]; i++) {
                printw("%s", trim_whitespaces(field_buffer(regFields[i], 0)));

                if (field_opts(regFields[i]) & O_ACTIVE)
                    printw("\"\t");
                else
                    printw(": \"");
            }
            //Refreshes the screen so that updated information can be seen
            refresh();
            pos_form_cursor(form);
            break;
        //If the user presses enter, this is to confirm they have completed the form. Store their data.
        case 10:
            // Or the current field buffer won't be sync with what is displayed
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_PREV_FIELD);
            move(LINES-3, 2);
            tempUserName = trim_whitespaces(field_buffer(regFields[1], 0));
            tempPass = trim_whitespaces(field_buffer(regFields[3], 0));
            tempName = trim_whitespaces(field_buffer(regFields[5], 0));
            tempCity = trim_whitespaces(field_buffer(regFields[7], 0));
            tempCountry = trim_whitespaces(field_buffer(regFields[9], 0));
            tempTimeZone = trim_whitespaces(field_buffer(regFields[11], 0));
            user = userSignIn(tempUserName, tempPass, tempName, tempCity, tempCountry, tempTimeZone,1,1,1,1);
            if(user.getErrorMessage() != ""){
                refresh();
                errorDescription = user.getErrorMessage();
                printw("%s",errorDescription.c_str());
            }
            else if(user.getUserExists(tempUserName) < 0){
                    getyx(stdscr, y, x); // save current pos
                    move(y, 0);          // move to begining of line
                    clrtoeol();          // clear line
                    move(y, x);          // move back to where you were
                    printw("SUCCESS: YOUR ACCOUNT HAS BEEN CREATED!");
                    //Store the user's entered information in fields
                    userName = tempUserName;
                    password = tempPass;
                    name = tempName;
                    city = tempCity;
                    country = tempCountry;
                    timeZone = tempTimeZone;
                    weather = 1;
                    stocks = 1;
                    news = 1;
                    timeanddate = 1;
                    clear();
                    refresh();
                    //Go to the dasboard
                    dashboardScreen();
            }
                //Otherwise, the username does not exist
            else{
                getyx(stdscr, y, x); // save current pos
                move(y, 0);          // move to begining of line
                clrtoeol();          // clear line
                move(y, x);          // move back to where you were
                if(user.getUserExists(tempUserName) == 0) {
                    printw("ERROR: USER ALREADY EXISTS!");
                }
            }
            refresh();
            pos_form_cursor(regForm);
            break;
            //If the user selects F12, then exit
        case KEY_F(12):
            endwin();
            exit(0);
            //Key Down will change the location of the cursor to go down
        case KEY_DOWN:
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_END_LINE);
            break;

            //Key Up will change the location of the cursor to go down
        case KEY_UP:
            form_driver(regForm, REQ_PREV_FIELD);
            form_driver(regForm, REQ_END_LINE);
            break;

            //Key Left will change the location of the cursor to go down
        case KEY_LEFT:
            form_driver(regForm, REQ_PREV_CHAR);
            break;

            //Key Right will change the location of the cursor to go down
        case KEY_RIGHT:
            form_driver(regForm, REQ_NEXT_CHAR);
            break;

            // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(regForm, REQ_DEL_PREV);
            break;

            // Delete the char under the cursor
        case KEY_DC:
            form_driver(regForm, REQ_DEL_CHAR);
            break;
            //Default, enter the text on the form
        default:
            form_driver(regForm, ch);
            break;
    }
    wrefresh(win_form);
}

/**
* @brief Trims the whitespaces before and after the user entry
* @detailed Deletes whitespaces from before and after the form entries in order to not store things that are too large
* @parameters char str, takes a string in order to trim the whitespaces
* @returns The string that has been trimmed of whitespaces
* @author Mitch Mancuso
*/
char* MainInterface::trim_whitespaces(char *str){
    //If there are spaces, skip them
    char *end;
    while(isspace(*str))
        str++;

    if(*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end+1) = '\0';

    return str;
}

/**
* @brief Draws the main dashboard window that will contain the widgets
* @detailed Shows the main dashboard which will be the central location in which users can access widgets
* @parameters none
* @returns void. Calls other functions and displays widgets through their own window functions
* @author Mitch Mancuso
*/
void MainInterface::dashboardScreen() {
    //Gets the size of the screen in order to position the options 4 x 4
    getmaxyx(stdscr, yMax, xMax);
    menuheader = newwin(4, 60, 2, ((xMax / 2) - (42 / 2)));
    menuwin = newwin((yMax-15), (xMax - 10), 12,((xMax-(xMax-10))/2));
    getmaxyx(menuwin, yMaxWin, xMaxWin);
    //Draws a box around the window
    box(menuwin, 0, 0);
    //Prints the ASCII art for the welcome area
    mvwprintw(menuheader, 0, 0, "   __      _____ _    ___ ___  __  __ ___ ");
    mvwprintw(menuheader, 1, 0, "   \\ \\    / / __| |  / __/ _ \\|  \\/  | __|");
    mvwprintw(menuheader, 2, 0, "    \\ \\/\\/ /| _|| |_| (_| (_) | |\\/| | _| ");
    mvwprintw(menuheader, 3, 0, "     \\_/\\_/ |___|____\\___\\___/|_|  |_|___|");
    int sizeOfUserName = strlen(userName.c_str());
    //Tells the user what username they are currently logged in as
    userNameWin = newwin(2, 60, 6, ((xMax / 2) - (22+sizeOfUserName)/2));
    mvwprintw(userNameWin, 1, 0, "You are logged in as: ");
    wattron(userNameWin, A_UNDERLINE);
        mvwprintw(userNameWin,1, 23, userName.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    wrefresh(userNameWin);
    wrefresh(menuwin);
    wrefresh(menuheader);

    keypad(menuwin, true);
    //Outlines the options to access the widgets, the user's choice will be the widget that opens
    string widgets[8] = {"Time and Date", "Calendar", "News", "Stocks", "Weather","Preferences","Log Out","Exit"};
    int choice;
    int highlight = 0;
    //Cycle through the 8 options to get the user's choice
    while (1) {
        for (int i = 0; i < 8; i++) {
            //Highlight the option the user is currently on
            if (i == highlight) {
                wattron(menuwin, A_REVERSE);
            }
            //Print all options on screen
            mvwprintw(menuwin, (10+ ((i/4) * (yMaxWin/2))), (10+ ((i % 4) * (xMaxWin/4))), widgets[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        //Get and store the user's choice
        choice = wgetch(menuwin);

        //Use the left arrow to cycle left through the options, if it hits end end of the screen - stop
        switch (choice) {
            case KEY_LEFT:
            highlight--;
            if (highlight == -1) {
                highlight = 0;
            }
            break;
            //Use the right arrow to cycle right through the options, if it hits end end of the screen - stop
            case KEY_RIGHT:
                highlight++;
                if (highlight == 8) {
                    highlight = 7;
                }
                break;
                //Use the up arrow to cycle up through the options, if it hits end end of the screen - stop
            case KEY_UP:
                highlight = highlight - 4;
                if (highlight < 0) {
                    highlight = 0;
                }
                break;
                //Use the down arrow to cycle down through the options, if it hits end end of the screen - stop
            case KEY_DOWN:
                highlight = highlight + 4;
                if (highlight > 7) {
                    highlight = 7;
                }
                break;
                //If the user enters shift + left, then exit back to the welcome screen
            case KEY_SLEFT:
                clear();
                refresh();
                welcomeScreen();
                break;
            default:
                break;
        }
        //The enter key will select the option for the user
        if (choice == 10) {
            break;
        }
    }
    //These commands determine which widget is launched

    //If the user decides to exit, exit the entire program
    if (widgets[highlight] == "Exit") {
        endwin();
        exit(0);
    }
    //If the user logs out, take them to the welcome screen and delete the stored variables for userName, password, etc
    else if (widgets[highlight] == "Log Out") {
        clear();
        refresh();
        userName = "";
        password = "";
        name = "";
        city = "";
        country = "";
        timeZone = "";
        weather = 0;
        stocks = 0;
        news = 0;
        timeanddate = 0;
        welcomeScreen();
    }
    //If the user selects weather, open the weather widget
    else if (widgets[highlight] == "Weather") {
        if(user.getWeather() == 0){
            refresh();
            move(LINES-3, 2);
            printw("The weather widget has been disabled. Please try another widget.");
            refresh();
            timeout(900);
            dashboardScreen();
        }
        else {
            wclear(win_form);
            wclear(win_body);
            wrefresh(win_form);
            wrefresh(win_body);
            clear();
            refresh();
            weatherWidgetScreen();
        }
    }
    //If the user selects stocks, open the stock widget
    else if (widgets[highlight] == "Stocks") {
        if(user.getFinance() == 0){
            refresh();
            move(LINES-3, 2);
            printw("The finance widget has been disabled. Please try another widget.");
            refresh();
            timeout(900);
            dashboardScreen();
        }
        else{
        wclear(win_form);
        wclear(win_body);
        wrefresh(win_form);
        wrefresh(win_body);
        clear();
        refresh();
        stockWidgetScreen();
        }
    }
    //If the user selects news, open the news widget
    else if (widgets[highlight] == "News") {
        if(user.getNews() == 0){
            refresh();
            move(LINES-3, 2);
            printw("The news widget has been disabled. Please try another widget.");
            refresh();
            timeout(900);
            dashboardScreen();
        }
        else {
            wclear(win_form);
            wclear(win_body);
            wrefresh(win_form);
            wrefresh(win_body);
            clear();
            refresh();
            newsWidgetScreen();
        }
    }
    //If the user selects time and date, open the time and date function
    else if (widgets[highlight] == "Time and Date") {
        if(user.getDate() == 0){
            refresh();
            move(LINES-3, 2);
            printw("The time and date widget has been disabled. Please try another widget.");
            refresh();
            timeout(900);
            dashboardScreen();
        }
        else {
            wclear(win_form);
            wclear(win_body);
            wrefresh(win_form);
            wrefresh(win_body);
            clear();
            refresh();
            timeAndDateWidgetScreen();
        }
    }
        //If the user selects preferences option, open the preferences screen
    else if (widgets[highlight] == "Preferences") {
        wclear(win_form);
        wclear(win_body);
        wrefresh(win_form);
        wrefresh(win_body);
        clear();
        refresh();
        preferencesScreen();
        }
    //If the user selects calendar option, display a warning - as it has not been built
    else if (widgets[highlight] == "Calendar") {
        refresh();
        move(LINES-3, 2);
        printw("The calendar widget is currently not available. Please try another widget.");
        refresh();
        timeout(900);
        dashboardScreen();
    }
}

/**
* @brief Draws the weather widget screen that wull show the weather information
* @detailed Shows the weather widget screen that will give the users options within the widget for current and five day forecasts
* @parameters none
* @returns void. Calls other functions and displays widgets through their own window functions
* @author Mitch Mancuso
*/
void MainInterface::weatherWidgetScreen() {
    //Creates WeatherWidgets, one for the current forecast and one for the five day forecast
    WeatherWidget weatherWidgetCurrent;
    WeatherWidget weatherWidgetFiveDay;
    //Stores the returned values of the temperature and description for use in priting to the screen
    string tempReturn;
    string descReturn;
    //Stores strings for the entire temp and description print statements
    string tempReturnPrintStatement;
    string descReturnPrintStatement;
    //Stores the values of conditions and temperatures for each of the 5 days
    string dayOneTemp, dayTwoTemp, dayThreeTemp, dayFourTemp, dayFiveTemp;
    string dayOneCond, dayTwoCond, dayThreeCond, dayFourCond, dayFiveCond;
    //Stores the print statement for the 5 day forecasts
    string tempFiveReturnPrintStatement;
    string descFiveReturnPrintStatement;

    //Gets the size of the window in order to create the header and menus
    getmaxyx(stdscr, yMax, xMax);
    menuheader = newwin(4, 60, 2, ((xMax / 2) - (42 / 2)));
    menuwin = newwin((yMax - 15), (xMax - 10), 13, ((xMax - (xMax - 10)) / 2));
    getmaxyx(menuwin, yMaxWin, xMaxWin);
    box(menuwin, 0, 0);
    //Prints the ASCII art for the weather title
    mvwprintw(menuheader, 0, 0, "   __      _____   _ _____ _  _ ___ ___ ");
    mvwprintw(menuheader, 1, 0, "   \\ \\    / / __| /_\\_   _| || | __| _ \\");
    mvwprintw(menuheader, 2, 0, "    \\ \\/\\/ /| _| / _ \\| | | __ | _||   /");
    mvwprintw(menuheader, 3, 0, "     \\_/\\_/ |___/_/ \\_\\_| |_||_|___|_|_\\");
    //Prints the user's username, country code and city that the widget is using
    int sizeOfUserName = strlen(userName.c_str());
    userNameWin = newwin(6, 60, 6, ((xMax / 2) - (22 + sizeOfUserName) / 2));
    mvwprintw(userNameWin, 1, 0, "You are logged in as: ");
    wattron(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 1, 23, userName.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 2, 0, "Your country ISO code is: ");
    wattron(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 2, 27, country.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 3, 0, "Your city is set to: ");
    wattron(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 3, 21, city.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    //Print out the options for the user to exit, and what commands are used to invoke options for the weather
    mvwprintw(userNameWin, 4, 0, "[  SHIFT + LEFT ] : Dashboard");
    mvwprintw(userNameWin, 5, 0, "[  F12  ] : Exit");
    mvwprintw(stdscr, 13, 10, "Press (c) to view a current forecast. Press (f) to view a five-day forecast.");
    //Refresh the screen items
    wrefresh(userNameWin);
    wrefresh(menuwin);
    wrefresh(menuheader);
    refresh();
    //Track user key input
    keypad(menuwin, true);

    int choice;

    while (1) {
        //Set choice to be the user's key input
        choice = wgetch(menuwin);

        switch (choice) {
            //If the user enters a lowercase c, pull information from the current forecast
            case 99:
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 2, 10, "Current Forecast");
                wattroff(menuwin, A_UNDERLINE);
                //Set the Widget object's parameters based on the user's set country and city
                weatherWidgetCurrent = WeatherWidget("CurrentWidget", 1, country, city, 'c');
                //Get the temperature and weather conditions and store in a string
                tempReturn = weatherWidgetCurrent.getWeatherTemperature();
                descReturn = weatherWidgetCurrent.getWeatherCondition();
                //Complete error handling functions if the user is not connected to the internet or their city is wrong
                if(weatherWidgetCurrent.getWeatherWidgetID() == -1){
                    mvwprintw(menuwin, 3, 15, "ERROR! Connection to API failed. Try checking your network?");
                }
                else if(weatherWidgetCurrent.getWeatherWidgetID() == -2){
                    mvwprintw(menuwin, 3, 15, "ERROR! City does not exist. Please set a new location in preferences.");
                }
                else {
                    //Otherwise, use the stored strings and print them to the screen
                    tempReturnPrintStatement = tempReturn + " °C is the current forecast in [" + city + "]";
                    descReturnPrintStatement = descReturn + " is the current weather condition in [" + city + "]";
                    mvwprintw(menuwin, 4, 15, (tempReturnPrintStatement).c_str());
                    mvwprintw(menuwin, 5, 15, (descReturnPrintStatement).c_str());

                    //The following ASCII art is printed to the screen depending on the description returned from the weather API

                    if(descReturn == "Thunderstorm") {
                        mvwprintw(menuwin, 7, 15, "      .-.      ");
                        mvwprintw(menuwin, 8, 15, "     (   ).    ");
                        mvwprintw(menuwin, 9, 15, "    (___(__)   ");
                        mvwprintw(menuwin, 10, 15, "     \\ ' /     ");
                        mvwprintw(menuwin, 11, 15,"     / '  \\ '   ");
                    }
                    else if(descReturn == "Drizzle") {
                        mvwprintw(menuwin, 6, 15, "      .-.      ");
                        mvwprintw(menuwin, 7, 15, "     (   ).    ");
                        mvwprintw(menuwin, 8, 15, "    (___(__)   ");
                        mvwprintw(menuwin, 9, 15, "     ' ' '     ");
                        mvwprintw(menuwin, 10, 15,"        '      ");
                    }

                    else if(descReturn == "Rain") {
                        mvwprintw(menuwin, 6, 15, "      .-.      ");
                        mvwprintw(menuwin, 7, 15, "     (   ).    ");
                        mvwprintw(menuwin, 8, 15, "    (___(__)   ");
                        mvwprintw(menuwin, 9, 15, "     : : :     ");
                        mvwprintw(menuwin, 10, 15,"     : : :     ");
                    }

                    else if(descReturn == "Snow") {
                        mvwprintw(menuwin, 6, 15, "      .-.      ");
                        mvwprintw(menuwin, 7, 15, "     (   ).    ");
                        mvwprintw(menuwin, 8, 15, "    (___(__)   ");
                        mvwprintw(menuwin, 9, 15, "     * * *     ");
                        mvwprintw(menuwin, 10, 15,"      * *      ");
                    }
                    else if(descReturn == "Atmosphere") {
                        mvwprintw(menuwin, 6, 15, "   ~~~~~~~~    ");
                        mvwprintw(menuwin, 7, 15, "   ~~~~~~~~    ");
                        mvwprintw(menuwin, 8, 15, "   ~~~~~~~~   ");
                        mvwprintw(menuwin, 9, 15, "   ~~~~~~~~    ");
                        mvwprintw(menuwin, 10, 15,"   ~~~~~~~~     ");
                    }
                    else if(descReturn == "Clear") {
                        mvwprintw(menuwin, 6, 15, "     \\   /     ");
                        mvwprintw(menuwin, 7, 15, "      .-.      ");
                        mvwprintw(menuwin, 8, 15, "   ~ (   ) ~   ");
                        mvwprintw(menuwin, 9, 15, "      `-'      ");
                        mvwprintw(menuwin, 10, 15,"     /   \\     ");
                    }
                    else if (descReturn == "Clouds"){
                        mvwprintw(menuwin, 6, 15, "      .-.      ");
                        mvwprintw(menuwin, 7, 15, "     (   ).    ");
                        mvwprintw(menuwin, 8, 15, "    (___(__)   ");
                        mvwprintw(menuwin, 9, 15, "               ");
                        mvwprintw(menuwin, 10, 15,"               ");
                    }
                        //If the condition is not found, produce an ERROR grid
                    else {
                        mvwprintw(menuwin, 6, 15, " E  R  R  O  R ");
                        mvwprintw(menuwin, 7, 15, " E  R  R  O  R ");
                        mvwprintw(menuwin, 8, 15, " E  R  R  O  R ");
                        mvwprintw(menuwin, 9, 15, " E  R  R  O  R ");
                        mvwprintw(menuwin, 10, 15," E  R  R  O  R ");
                    }
                }
                //Tell the user to go back after the widget's function has completed
                mvwprintw(stdscr, 45, 10, "Please press [  SHIFT + LEFT  ] to go back.");
                refresh();
                break;
                //If the user enters an f, pull the 5 day forecast
            case 102:
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 2, 10, "Five Day Forecast");
                wattroff(menuwin, A_UNDERLINE);
                //Create a widget for the five day forecast
                weatherWidgetFiveDay = WeatherWidget("FiveDayWidget", 1, country, city, 'f');
                //Using the alternate getter function, pull information from the temperatures field based on the day from now
                //This will pull and store the next 5 days (including the current)
                dayOneTemp = weatherWidgetFiveDay.getWeatherTemperatures(0);
                dayTwoTemp = weatherWidgetFiveDay.getWeatherTemperatures(1);
                dayThreeTemp = weatherWidgetFiveDay.getWeatherTemperatures(2);
                dayFourTemp = weatherWidgetFiveDay.getWeatherTemperatures(3);
                dayFiveTemp = weatherWidgetFiveDay.getWeatherTemperatures(4);
                dayOneCond = weatherWidgetFiveDay.getWeatherConditions(0);
                dayTwoCond = weatherWidgetFiveDay.getWeatherConditions(1);
                dayThreeCond = weatherWidgetFiveDay.getWeatherConditions(2);
                dayFourCond = weatherWidgetFiveDay.getWeatherConditions(3);
                dayFiveCond = weatherWidgetFiveDay.getWeatherConditions(4);

                //If there are errors accessing the API due to network, tell the user
                if(weatherWidgetFiveDay.getWeatherWidgetID() == -1){
                    mvwprintw(menuwin, 3, 15, "ERROR! Connection to API failed. Try checking your network?");
                }
                    //If the city does not exist, tell the user
                else if(weatherWidgetFiveDay.getWeatherWidgetID() == -2){
                    mvwprintw(menuwin, 3, 15, "ERROR! City does not exist. Please set a new location in preferences.");
                }
                else {
                    //For all 5 days, save information relating to the temp and conditions
                    for(int i = 0; i < 5; i++) {
                        //Save the temperature description in a string
                        tempFiveReturnPrintStatement =
                                weatherWidgetFiveDay.getWeatherTemperatures(i) + " °C is the forecasted temperature " +
                                to_string(i) + " day(s) from now in [" + city + "]";
                        //Save the condition description in a string
                        descFiveReturnPrintStatement =
                                weatherWidgetFiveDay.getWeatherConditions(i) + " is the forecasted condition " +
                                to_string(i) + " day(s) from now in [" + city + "]";
                        //Print the conditions on the screen one after another
                        mvwprintw(menuwin, 4 + (i * 5), 20, (tempFiveReturnPrintStatement).c_str());
                        mvwprintw(menuwin, 4 + ((i * 5) + 1), 20, (descFiveReturnPrintStatement).c_str());
                        mvwprintw(menuwin, 4 + ((i * 5) + 2), 20, " ");
                        mvwprintw(menuwin, 4 + ((i * 5) + 3), 20, " ");
                        mvwprintw(menuwin, 4 + ((i * 5) + 4), 20, " ");

                        //The following ASCII art is printed to the screen depending on the description returned from the weather API

                        if (weatherWidgetFiveDay.getWeatherConditions(i) == "Thunderstorm") {
                            mvwprintw(menuwin, 4 + (i * 5), 2,       "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2, "     (   ).    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2, "    (___(__)   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2, "     \\ ' /     ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2, "     / '  \\    ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Drizzle") {
                            mvwprintw(menuwin, 4 + (i * 5), 2,       "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2, "     (   ).    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2, "    (___(__)   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2, "     ' ' '     ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2, "        '      ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Rain") {
                            mvwprintw(menuwin, 4 + (i * 5), 2,       "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2, "     (   ).    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2, "    (___(__)   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2, "     : : :     ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2, "     : : :     ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Snow") {
                            mvwprintw(menuwin, 4 + (i * 5), 2 ,      "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2, "     (   ).    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2, "    (___(__)   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2, "     * * *     ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2, "      * *      ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Atmosphere") {
                            mvwprintw(menuwin, 4 + (i * 5), 2 ,      "   ~~~~~~~~    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2, "   ~~~~~~~~    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2, "   ~~~~~~~~    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2, "   ~~~~~~~~    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2, "   ~~~~~~~~    ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Clear") {
                            mvwprintw(menuwin, 4 + (i * 5), 2 ,       "     \\   /     ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2 , "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2 , "   ~ (   ) ~   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2 , "      `-'      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2 , "     /   \\     ");
                        } else if (weatherWidgetFiveDay.getWeatherConditions(i) == "Clouds") {
                            mvwprintw(menuwin, 4 + (i * 5), 2 ,       "      .-.      ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2 , "     (   ).    ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2 , "    (___(__)   ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2 , "               ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2 , "               ");
                        }
                        else {
                            mvwprintw(menuwin, 4 + (i * 5), 2 ,       " E  R  R  O  R ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 1), 2 , " E  R  R  O  R ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 2), 2 , " E  R  R  O  R ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 3), 2 , " E  R  R  O  R ");
                            mvwprintw(menuwin, 4 + ((i * 5) + 4), 2 , " E  R  R  O  R ");
                        }
                    }
                }
                //After the widget has been run, tell the user the command to exit
                mvwprintw(stdscr, 45, 10, "Please press [  SHIFT + LEFT  ] to go back.");
                refresh();
                break;
                //If the user enters shift + left, go back to the dashboard
            case KEY_SLEFT:
                clear();
                refresh();
                dashboardScreen();
                break;
                //If the user enters F12, then exit entirely
            case KEY_F(12):
                endwin();
                exit(0);
            default:
                break;
        }
    }
}

/**
* @brief Draws the stock widget screen that wull show the stock information
* @detailed Shows the stock widget screen that will give the users options to get a list of top stocks and user entries
* @parameters none
* @returns void. Calls the stockWidget functions within the StockWidget.cpp class to pull information
* @author Mitch Mancuso
*/
void MainInterface::stockWidgetScreen() {
    //Create StockWidgets for each instance of the top5 stocks and the individual user choice stocks
    StockWidget stockWidget;
    StockWidget stockWidgetTop1;
    StockWidget stockWidgetTop2;
    StockWidget stockWidgetTop3;
    StockWidget stockWidgetTop4;
    StockWidget stockWidgetTop5;
    //Store strings relating to the print statements of the top 5 stocks
    string stockPricePrintTop1;
    string stockPricePrintTop2;
    string stockPricePrintTop3;
    string stockPricePrintTop4;
    string stockPricePrintTop5;
    string stockPricePrint;
    //Store the user entered stock symbol in an array
    char userSymbol[10];

    //Get the size of the screen and use it for positioning
    getmaxyx(stdscr, yMax, xMax);
    menuheader = newwin(4, 60, 2, ((xMax / 2) - (33 / 2)));
    menuwin = newwin((yMax - 15), (xMax - 10), 12, ((xMax - (xMax - 10)) / 2));
    getmaxyx(menuwin, yMaxWin, xMaxWin);
    //Draw a box around the window for the header
    box(menuwin, 0, 0);
    //Prints the ASCII art for the weather title
    mvwprintw(menuheader, 0, 0, "    ___ _____ ___   ___ _  _____ ");
    mvwprintw(menuheader, 1, 0, "   / __|_   _/ _ \\ / __| |/ / __|");
    mvwprintw(menuheader, 2, 0, "   \\__ \\ | || (_) | (__| ' <\\__ \\");
    mvwprintw(menuheader, 3, 0, "   |___/ |_| \\___/ \\___|_|\\_\\___/");
    //Print the username that the user is logged in as
    int sizeOfUserName = strlen(userName.c_str());
    userNameWin = newwin(6, 60, 6, ((xMax / 2) - (18 + sizeOfUserName) / 2));
    mvwprintw(userNameWin, 1, 0, "You are logged in as: ");
    wattron(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 1, 23, userName.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    //Print the commands that the user can enter
    mvwprintw(userNameWin, 2, 0, "[  SHIFT + LEFT ] : Dashboard");
    mvwprintw(userNameWin, 3, 0, "[  F12  ] : Exit");
    //Display the user choices to view top 5 stocks or enter their own
    mvwprintw(stdscr, 12, 10, "Press [ t ] to view prices of the top 5 stocks.  |  Press [ e ] to enter a stock symbol to look up");
    wrefresh(userNameWin);
    wrefresh(menuwin);
    wrefresh(menuheader);
    refresh();
    //Get ready to take user input
    keypad(menuwin, true);

    int choice;
    //Monitor the user's input and make decisions accordingly
    while (1) {
        choice = wgetch(menuwin);

        switch (choice) {
            //If the user enters t, then get the top 5 stock prices by creating multiple stock widgets
            case 116:
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 2, 10, "Top 5 Stock Prices");
                wattroff(menuwin, A_UNDERLINE);
                stockWidgetTop1 = StockWidget("Microsoft Stock Price:", 2, "MSFT");
                stockWidgetTop2 = StockWidget("Apple Stock Price", 3, "AAPL");
                stockWidgetTop3 = StockWidget("Amazon Stock Price", 4, "AMZN");
                stockWidgetTop4 = StockWidget("Google Stock Price", 5, "GOOGL");
                stockWidgetTop5 = StockWidget("Facebook Stock Price", 6, "FB");
                //Handle error messages that have to do with APIs and if -1, then the network is not connected
                if (stockWidgetTop1.getStockWidgetID() == -1) {
                    mvwprintw(menuwin, 3, 15, "ERROR! Connection to API failed. Try checking your network?");
                } else {
                    //Print out the price and stock symbols for each of the top 5 stocks and display them on screen

                    //From 1 - 5

                    stockPricePrintTop1 = "$ " + stockWidgetTop1.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 4, 20, ((stockWidgetTop1.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 5, 20, stockPricePrintTop1.c_str());
                    wattroff(menuwin, A_REVERSE);

                    stockPricePrintTop2 = "$ " + stockWidgetTop2.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 7, 20, ((stockWidgetTop2.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 8, 20, stockPricePrintTop2.c_str());
                    wattroff(menuwin, A_REVERSE);

                    stockPricePrintTop3 = "$ " + stockWidgetTop3.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 10, 20, ((stockWidgetTop3.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 11, 20, stockPricePrintTop3.c_str());
                    wattroff(menuwin, A_REVERSE);

                    stockPricePrintTop4 = "$ " + stockWidgetTop4.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 13, 20, ((stockWidgetTop4.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 14, 20, stockPricePrintTop4.c_str());
                    wattroff(menuwin, A_REVERSE);

                    stockPricePrintTop5 = "$ " + stockWidgetTop5.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 16, 20, ((stockWidgetTop5.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 17, 20, stockPricePrintTop5.c_str());
                    wattroff(menuwin, A_REVERSE);

                    //Tell the user what command to enter in order to exit

                    mvwprintw(stdscr, 45, 10, "Please press [  SHIFT + LEFT  ] to go back.");
                    refresh();
                }
                break;

                //If the user enters e, they will enter their own symbol
                case 101:
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 2, 10, "User Selected Stock Price");
                wattroff(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 3, 10, "Enter a stock symbol and then press [  ENTER  ]");
                wrefresh(menuwin);
                //Get the symbol from the user and return it
                getstr(userSymbol);
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 6, 10, userSymbol);
                    wattron(menuwin, A_REVERSE);
                    wrefresh(menuwin);
                    //Put the userSymbol in a widget function and get the price
                stockWidget = StockWidget("Your selected stock price:", 7, userSymbol);
                stockWidget.getStockPrice();
                //If there are errors connecting to the API, the stock symbol doesn't exit, or the symbol is blank - tell the user
                if (stockWidget.getStockWidgetID() == -1) {
                    mvwprintw(menuwin, 4, 15, "ERROR! Connection to API failed. Try checking your network?");
                } else if (stockWidget.getStockWidgetID() == -2) {
                    mvwprintw(menuwin, 4, 15, "ERROR! Stock symbol does not exist. Please try again.");
                } else if (stockWidget.getStockWidgetID() == -3) {
                    mvwprintw(menuwin, 4, 15, "ERROR! You entered a blank stock symbol. Please try again.");
                } else {
                    //Otherwise, call the API and print the price on screen
                    wrefresh(menuwin);
                    stockPricePrint = "$ " + stockWidget.getStockPrice() + "0 USD";
                    mvwprintw(menuwin, 8, 20, ((stockWidget.getStockWidgetName()).c_str()));
                    wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, 9, 20, stockPricePrint.c_str());
                    wattroff(menuwin, A_REVERSE);
                }
                //Tell the user how to exit the widget option
                    mvwprintw(stdscr, 45, 10, "Please press [  SHIFT + LEFT  ] to go back.");
                    refresh();
                    break;
                //If the user enters shift + left, go back to the main dashboard screen
                    case KEY_SLEFT:
                        clear();
                        refresh();
                        dashboardScreen();
                    break;
                    //If the user enters F12, then exit entirely
                    case KEY_F(12):
                        endwin();
                        exit(0);
                    default:
                        break;
        }
    }
}
/**
* @brief Draws the news widget screen that will show the news information
* @detailed Shows the news widget screen that will give the users options to get a list of top stocks and user entries
* @parameters none
* @returns void. Calls the newsWidget functions within the NewsWidget.cpp class to pull information
* @author Mitch Mancuso
*/
void MainInterface::newsWidgetScreen() {
    //Create a news widget for the top 5 stories
    NewsWidget newsWidgetTop5;
    //Store the top 5 headlines and URLs in respective strings
    string top1Headline, top2Headline, top3Headline, top4Headline, top5Headline;
    string top1URL, top2URL, top3URL, top4URL, top5URL;

    //Get the screen size information in order to position things appropriately
    getmaxyx(stdscr, yMax, xMax);
    menuheader = newwin(4, 80, 2, ((xMax / 2) - (26 / 2)));
    menuwin = newwin((yMax - 15), (xMax - 10), 12, ((xMax - (xMax - 10)) / 2));
    getmaxyx(menuwin, yMaxWin, xMaxWin);
    box(menuwin, 0, 0);
    //Use ASCII art to print the news header on screen
    mvwprintw(menuheader, 0, 0, "    _  _ _____      _____ ");
    mvwprintw(menuheader, 1, 0, "   | \\| | __\\ \\    / / __|");
    mvwprintw(menuheader, 2, 0, "   | .` | _| \\ \\/\\/ /\\__ \\");
    mvwprintw(menuheader, 3, 0, "   |_|\\_|___| \\_/\\_/ |___/");
    //Display the user's current name on screen that they are currently logged in as
    int sizeOfUserName = strlen(userName.c_str());
    userNameWin = newwin(6, 60, 6, ((xMax / 2) - (18 + sizeOfUserName) / 2));
    mvwprintw(userNameWin, 1, 0, "You are logged in as: ");
    wattron(userNameWin, A_UNDERLINE);
    mvwprintw(userNameWin, 1, 23, userName.c_str());
    wattroff(userNameWin, A_UNDERLINE);
    //Display instructions on the screen on how to exit and go through other commands
    mvwprintw(userNameWin, 2, 0, "[  SHIFT + LEFT ] : Dashboard");
    mvwprintw(userNameWin, 3, 0, "[  F12  ] : Exit");
    mvwprintw(stdscr, 12, 10, "Press [ v ] to view the top news stories.");
    wrefresh(userNameWin);
    wrefresh(menuwin);
    wrefresh(menuheader);
    refresh();

    //Get and store the user input in order to decide the next steps
    keypad(menuwin, true);

    int choice;

    while (1) {
        choice = wgetch(menuwin);
        //Choose what to do based on the user's input
        switch (choice) {
            //If the user inputs v, pull the top 5 news stories
            case 118:
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 2, 10, "Top 5 News Stories");
                wattroff(menuwin, A_UNDERLINE);
                newsWidgetTop5 = NewsWidget("News Headlines", 8, 10);
                //If the API call has an error, tell the user
                if (newsWidgetTop5.getNewsWidgetID() == -1) {
                    mvwprintw(menuwin, 3, 15, "ERROR! Connection to API failed. Try checking your network?");
                }
                //If there are no errors, get the headlines and URLS
                else {

                    //Get the top5 headlines and URLs and return them to the screen

                top1Headline = newsWidgetTop5.getNewsHeadlines(1);
                top1URL = newsWidgetTop5.getNewsUrls(1);
                mvwprintw(menuwin, 4, 5, (top1Headline.c_str()));
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 5, 5, top1URL.c_str());
                wattroff(menuwin, A_UNDERLINE);

                top2Headline = newsWidgetTop5.getNewsHeadlines(2);
                top2URL = newsWidgetTop5.getNewsUrls(2);
                mvwprintw(menuwin, 7, 5, (top2Headline.c_str()));
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 8, 5, top2URL.c_str());
                wattroff(menuwin, A_UNDERLINE);

                top3Headline = newsWidgetTop5.getNewsHeadlines(3);
                top3URL = newsWidgetTop5.getNewsUrls(3);
                mvwprintw(menuwin, 10, 5, (top3Headline.c_str()));
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 11, 5, top3URL.c_str());
                wattroff(menuwin, A_UNDERLINE);

                top4Headline = newsWidgetTop5.getNewsHeadlines(4);
                top4URL = newsWidgetTop5.getNewsUrls(4);
                mvwprintw(menuwin, 13, 5, (top4Headline.c_str()));
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 14, 5, top4URL.c_str());
                wattroff(menuwin, A_UNDERLINE);

                top5Headline = newsWidgetTop5.getNewsHeadlines(5);
                top5URL = newsWidgetTop5.getNewsUrls(5);
                mvwprintw(menuwin, 16, 5, (top5Headline.c_str()));
                wattron(menuwin, A_UNDERLINE);
                mvwprintw(menuwin, 17, 5, top5URL.c_str());
                wattroff(menuwin, A_UNDERLINE);

                //Tell the user the instructions of how to exit

                mvwprintw(stdscr, 45, 10, "Please press [  SHIFT + LEFT  ] to go back.");
                refresh();

        }
                break;

                //If the user presses the shift + left key, go back to the dashboard
            case KEY_SLEFT:
                clear();
                refresh();
                dashboardScreen();
                break;

                //If the user presses the F12 key, exit entirely
            case KEY_F(12):
                endwin();
                exit(0);
            default:
                break;
        }
    }
}

/**
* @brief Draws the time and date widget screen that will show the time and date information
* @detailed Shows the time and date widget screen that will give show the time and date based on the user's current set location
* @parameters none
* @returns void. Creates TimeAndDateWidget object and performs operations on it to display time and date based on the user's current location
* @author Qi Liu
*/
void MainInterface::timeAndDateWidgetScreen(){
    //Create an instance of the time and date widget object
    TimeDateWidget timeDate;
    int c,ch,ctml,maxx,maxy,midx,midy,ydln,ydln2;
    int els = 10;
    int hln = 25;
    //Store local variables for the week days, months, current minutes, hours, seconds
    char ctm[hln], mon[els], wkd[els], yr[els], mnd[els], yd[els], ydl[els];
    //Get the set time and store
    time_t t = time(0);
    struct tm *today = localtime(&t);

    //Get the size of the screen in terms of position for the overlay window
    maxx = getmaxx(stdscr);
    maxy = getmaxy(stdscr);

    midy = maxy/2;
    midx = maxx/2;
    int sum = 0;

    while(1) {
        //Set the ASCII art for the title
        mvwprintw(stdscr, 10, midx - (53/2), "    _____ ___ __  __ ___     _     ___   _ _____ ___ ");
        mvwprintw(stdscr, 11, midx - (53/2), "   |_   _|_ _|  \\/  | __|  _| |_  |   \\ /_\\_   _| __|");
        mvwprintw(stdscr, 12, midx - (53/2), "     | |  | || |\\/| | _|  |_   _| | |) / _ \\| | | _| ");
        mvwprintw(stdscr, 13, midx - (53/2), "     |_| |___|_|  |_|___|   |_|   |___/_/ \\_\\_| |___|");
        wattron(menuwin, A_UNDERLINE);
        mvwprintw(stdscr, 20, midx - (53/2), "Based on your current set location, the time and date is:");
        //Print and display the time and date information
        wattroff(menuwin, A_UNDERLINE);
                curs_set(0);
                box(stdscr, 0, 0);
                move(0, 0);
                t = time(0);
                //Grab the local time and set
                today = localtime(&t);
                //Grab the days, months, year, day of the week, hours, minutes, seconds
                timeDate.ctmcpy(ctm, today, hln);
                timeDate.moncpy(mon, today, els);
                timeDate.wkdcpy(wkd, today, els);
                timeDate.mndcpy(mnd, today, els);
                timeDate.yrcpy(yr, today, els);
                timeDate.ydcpy(yd, today, els);
                sum = ((strlen(mon) + strlen(wkd) + strlen(yr) + strlen(mnd) + 3) / 2);
                //Move the cursor and print out the returned values
                move(midy - 1, midx - sum);
                printw(wkd);
                printw(" ");
                printw(mon);
                printw(" ");
                printw(mnd);
                printw(" ");
                printw(yr);
                ctml = (strlen(ctm) / 2);
                //After printing move the cursor to the middle of the screen
                move(midy, midx - ctml);
                printw(ctm);
                refresh();
                //Set the sleep function in order to schedule the pulling of the seconds and other information to update
                timeDate.sleep(150);
                clear();
                //Wait for the kill switch from the user in order to exit the terminal
        if(timeDate.kbhit())
        {
            //Get the character from the user's key input
            ch = getch();
            //If the user inputs shift + left, go back to the main dashboard
            if(ch == KEY_SLEFT) {
                clear();
                refresh();
                dashboardScreen();
            }
            //If the user enters F12, exit entirely
            else if (ch == KEY_F(12)){
                endwin();
                exit(0);
        }
        refresh();
    }
        //Print instructions at the bottom to tell the user how to go back and exit
        mvwprintw(stdscr, 50, midx - (31/2), "[  SHIFT + LEFT  ]  : Dashboard");
        mvwprintw(stdscr, 51, midx - (16/2), "[  F12  ] : Exit");
        }
    }

/**
* @brief Creates the preferencesScreen using ncurses
* @detailed Sets the windows and draws the front-end preferencesScreen with a form to get the userName, Password, City, Country Code, Etc from the user
* @parameters Takes no parameters
* @returns void. Does call the form window functions to store the userName, password, name, country character code
* Also draws an ASCII style header for the login screen as a UI element
* @author Mitch Mancuso
*/
void MainInterface::preferencesScreen() {
    //Allows for user input through the keypad
    int ch;
    keypad(stdscr, TRUE);
    //Determines the size of the window for placement of elements
    getmaxyx(stdscr, yMax, xMax);

    //Draws an ASCII style header for the preferences screen
    mvwprintw(stdscr, 2, ((xMax/2) - 25), "    ___ ___ ___ ___ ___ ___ ___ _  _  ___ ___ ___ ");
    mvwprintw(stdscr, 3, ((xMax/2) - 25), "   | _ \\ _ \\ __| __| __| _ \\ __| \\| |/ __| __/ __|");
    mvwprintw(stdscr, 4, ((xMax/2) - 25), "   |  _/   / _|| _|| _||   / _|| .` | (__| _|\\__ \\");
    mvwprintw(stdscr, 5, ((xMax/2) - 25), "   |_| |_|_\\___|_| |___|_|_\\___|_|\\_|\\___|___|___/");

    win_body = newwin(30, 80, 8, ((xMax / 2) - (80/2)));

    //Starts the creation of a form to be able to enter the user registration information
    assert(win_body != NULL);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 15, 70, 6, 3);
    getmaxyx(win_body, yMaxWin, xMaxWin);
    //Prints prompts for the user with information how to exit, enter, etc
    mvwprintw(win_body, 3, ((xMaxWin/2)-(32/2)),"[  SHIFT + LEFT ] : Main Menu");
    mvwprintw(win_body, 4, ((xMaxWin/2)-(21/2)),"[  ENTER  ] : Confirm");
    mvwprintw(win_body, 5, ((xMaxWin/2)-(16/2)),"[  F12  ] : Exit");
    mvwprintw(win_body, 24, ((xMaxWin / 2) - (13 / 2)), "[   APPLY   ]");

    //Creates sets of fields that will be used to store the various data points
    regFields[0] = new_field(1, 10, 4, 10, 0, 0);
    regFields[1] = new_field(1, 40, 4, 25, 0, 0);
    regFields[2] = new_field(1, 10, 6, 10, 0, 0);
    regFields[3] = new_field(1, 40, 6, 25, 0, 0);
    regFields[4] = new_field(1, 10, 8, 10, 0, 0);
    regFields[5] = new_field(1, 40, 8, 25, 0, 0);
    regFields[6] = new_field(1, 10, 10, 10, 0, 0);
    regFields[7] = new_field(1, 40, 10, 25, 0, 0);
    regFields[8] = new_field(1, 12, 12, 10, 0, 0);
    regFields[9] = new_field(1, 40, 12, 25, 0, 0);
    regFields[10] = new_field(1, 10, 14, 10, 0, 0);
    regFields[11] = new_field(1, 40, 14, 25, 0, 0);
    regFields[12] = NULL;
    assert(regFields[0] != NULL && regFields[1] != NULL && regFields[2] != NULL && regFields[3] != NULL && regFields[4] != NULL && regFields[5] != NULL && regFields[6] != NULL && regFields[7] != NULL && regFields[8] != NULL && regFields[9] != NULL && regFields[10] != NULL && regFields[11] != NULL);

    //Sets the fields and data to be empty in the storage array fields and the information type in the array spot before it
    set_field_buffer(regFields[0], 0, "Username:");
    set_field_buffer(regFields[1], 0, "");
    set_field_buffer(regFields[2], 0, "Password:");
    set_field_buffer(regFields[3], 0, "");
    set_field_buffer(regFields[4], 0, "Name:");
    set_field_buffer(regFields[5], 0, "");
    set_field_buffer(regFields[6], 0, "City:");
    set_field_buffer(regFields[7], 0, "");
    set_field_buffer(regFields[8], 0, "Country Code:");
    set_field_buffer(regFields[9], 0, "");
    set_field_buffer(regFields[10], 0, "Time Zone:");
    set_field_buffer(regFields[11], 0, "");

    //Sets the visibility attributes of the form, the password field will be hidden for security upon input
    set_field_opts(regFields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[3], O_VISIBLE | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[4], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[5], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[6], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[7], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[8], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[9], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(regFields[10], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(regFields[11], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    //The fields where entry will be expected will have the ability to be underlined
    set_field_back(regFields[1], A_UNDERLINE);
    set_field_back(regFields[3], A_UNDERLINE);
    set_field_back(regFields[5], A_UNDERLINE);
    set_field_back(regFields[7], A_UNDERLINE);
    set_field_back(regFields[9], A_UNDERLINE);
    set_field_back(regFields[11], A_UNDERLINE);

    //Create and set the form information - post the form
    regForm = new_form(regFields);
    assert(regForm != NULL);
    set_form_win(regForm, win_form);
    set_form_sub(regForm, derwin(win_form, 145, 54, 2, 2));
    post_form(regForm);

    //Refresh the form on screen
    refresh();
    wrefresh(win_body);
    wrefresh(win_form);

    while ((ch = getch()) != KEY_SLEFT)
        registerHelper(ch);
    unpost_form(regForm);
    free_form(regForm);
    free_field(regFields[0]);
    free_field(regFields[1]);
    free_field(regFields[2]);
    free_field(regFields[3]);
    free_field(regFields[4]);
    free_field(regFields[5]);
    free_field(regFields[6]);
    free_field(regFields[7]);
    free_field(regFields[8]);
    free_field(regFields[9]);
    free_field(regFields[10]);
    free_field(regFields[11]);
    clear();
    refresh();
    //Send the user back to the dashboard screen if they hit shift + left
    dashboardScreen();
}

/**
* @brief Allows for a preferences helper that determines if the user's information is valid and pushes the function through the preferences update stage
* @detailed Allows the user to set preferences and based on the user's inputs may exit, enter, or quit the program entirely
* @parameters char ch is the user's keystroke input which is monitored against the system options
* @returns void. Calls other functions and displays things like error messages if the information is blank, userName already exists, etc
* @author Mitch Mancuso
*/
void MainInterface::preferencesHelper(int ch){
    //Declares variables to hold values of window size, the password the user wishes to implement and their selected username
    int i, y, x, userNameExists;
    string tempUserName, tempPass, tempName, tempCity, tempCountry, tempTimeZone, errorDescription;
    user = userSignIn();
    //Monitor's the user input to determine next steps to take
    switch (ch) {
        //F8 is used to debug text input and is not mentioned on the main screen
        case KEY_F(8):
            // Or the current field buffer won't be sync with what is displayed
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_PREV_FIELD);
            move(LINES-3, 2);
            //Prints fields removing whitespaces on the screen for easy viewing
            for (i = 0; regFields[i]; i++) {
                printw("%s", trim_whitespaces(field_buffer(regFields[i], 0)));

                if (field_opts(regFields[i]) & O_ACTIVE)
                    printw("\"\t");
                else
                    printw(": \"");
            }
            //Refreshes the screen so that updated information can be seen
            refresh();
            pos_form_cursor(form);
            break;
            //If the user presses enter, this is to confirm they have completed the form. Store their data.
        case 10:
            // Or the current field buffer won't be sync with what is displayed
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_PREV_FIELD);
            move(LINES-3, 2);
            tempUserName = trim_whitespaces(field_buffer(regFields[1], 0));
            tempPass = trim_whitespaces(field_buffer(regFields[3], 0));
            tempName = trim_whitespaces(field_buffer(regFields[5], 0));
            tempCity = trim_whitespaces(field_buffer(regFields[7], 0));
            tempCountry = trim_whitespaces(field_buffer(regFields[9], 0));
            tempTimeZone = trim_whitespaces(field_buffer(regFields[11], 0));
            user = userSignIn(tempUserName, tempPass, tempName, tempCity, tempCountry, tempTimeZone,1,1,1,1);
            if(user.getErrorMessage() != ""){
                refresh();
                errorDescription = user.getErrorMessage();
                printw("%s",errorDescription.c_str());
            }
            else if(user.getUserExists(tempUserName) < 0){
                getyx(stdscr, y, x); // save current pos
                move(y, 0);          // move to begining of line
                clrtoeol();          // clear line
                move(y, x);          // move back to where you were
                printw("SUCCESS: YOUR ACCOUNT HAS BEEN CREATED!");
                //Store the user's entered information in fields
                userName = tempUserName;
                password = tempPass;
                name = tempName;
                city = tempCity;
                country = tempCountry;
                timeZone = tempTimeZone;
                weather = 1;
                stocks = 1;
                news = 1;
                timeanddate = 1;
                clear();
                refresh();
                //Go to the dasboard
                dashboardScreen();
            }
                //Otherwise, the username does not exist
            else{
                getyx(stdscr, y, x); // save current pos
                move(y, 0);          // move to begining of line
                clrtoeol();          // clear line
                move(y, x);          // move back to where you were
                if(user.getUserExists(tempUserName) == 0) {
                    printw("ERROR: USER ALREADY EXISTS!");
                }
            }
            refresh();
            pos_form_cursor(regForm);
            break;
            //If the user selects F12, then exit
        case KEY_F(12):
            endwin();
            exit(0);
            //Key Down will change the location of the cursor to go down
        case KEY_DOWN:
            form_driver(regForm, REQ_NEXT_FIELD);
            form_driver(regForm, REQ_END_LINE);
            break;

            //Key Up will change the location of the cursor to go down
        case KEY_UP:
            form_driver(regForm, REQ_PREV_FIELD);
            form_driver(regForm, REQ_END_LINE);
            break;

            //Key Left will change the location of the cursor to go down
        case KEY_LEFT:
            form_driver(regForm, REQ_PREV_CHAR);
            break;

            //Key Right will change the location of the cursor to go down
        case KEY_RIGHT:
            form_driver(regForm, REQ_NEXT_CHAR);
            break;

            // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(regForm, REQ_DEL_PREV);
            break;

            // Delete the char under the cursor
        case KEY_DC:
            form_driver(regForm, REQ_DEL_CHAR);
            break;
            //Default, enter the text on the form
        default:
            form_driver(regForm, ch);
            break;
    }
    wrefresh(win_form);
}
