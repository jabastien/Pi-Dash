#include <iostream>
#include <sstream>
#include <string>
#include <ncurses.h>
#include "TimeDateWidget.h"

using namespace std;

/**
* @brief A test function set testing the TimeDateWidget
* @detailed Tests the TimeDateWidget's ability to print to the screen
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns ncurses display to the screen with the system's current time and date
* @author Qi Liu
*/

int main()
{
    //Creates a TimeDateWidget object called timedate
    TimeDateWidget timeDate;
    //creates variables that will be used to position the elements on screen and get inputs from the user using ncurses
    int c,ch,ctml,maxx,maxy,midx,midy,ydln,ydln2;
    int els = 10;
    int hln = 25;
    //Sets arrays for the currentTime, Weekday, Year, Month Month Name, Hour, Min, Second, AM/PM
    char ctm[hln], mon[els], wkd[els], yr[els], mnd[els], yd[els], ydl[els];
    //Gets the current system time
    time_t t = time(0);
    struct tm *today = localtime(&t);

    //Starts ncurses
    initscr();

    //Gets the max of the stdscreen on x and y
    maxx = getmaxx(stdscr);
    maxy = getmaxy(stdscr);

    //Divides these values by 2
    midy = maxy/2;
    midx = maxx/2;

    //Creates a loop to always run the screen with the time as it needs constant updating
    while(1)
    {
        //
        int sum = 0;
        curs_set(0);
        box(stdscr,0,0);
        move(0,0);
        t = time(0);
        today = localtime(&t);
        //Get the month, day, year, current time, weekday name, etc from the functions in the TimeDateWidget h file
        timeDate.ctmcpy(ctm,today,hln);
        timeDate.moncpy(mon,today,els);
        timeDate.wkdcpy(wkd,today,els);
        timeDate.mndcpy(mnd,today,els);
        timeDate.yrcpy(yr,today,els);
        timeDate.ydcpy(yd,today,els);
        //Get the length of the strings, add 3 and divide by 2 for positioning
        sum = ((strlen(mon) + strlen(wkd) + strlen(yr) + strlen(mnd) + 3)/2);

        //Use ncurses to move across the screen to a specific spot
        move(midy-1,midx-sum);
        //Print the time and date information on the window
        printw(wkd);
        printw(" ");
        printw(mon);
        printw(" ");
        printw(mnd);
        printw(" ");
        printw(yr);

        ctml = (strlen(ctm)/2);

        move(midy,midx-ctml);
        printw(ctm);
        //Refresh and sleep uses the system to refresh each second to display the new time
        refresh();
        timeDate.sleep(150);
        clear();

        refresh();
    }
    clear();
    //Ends the TimeDateWidget
    endwin();
}