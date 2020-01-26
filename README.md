#########################################################
     Pi-Dash
#########################################################

The dashboard allows for user registration and to log in through a sample account as noted below.<br>
Weather, News, Stocks pull from various APIs using CURL to display on the front end.<br>
All UI is built using ncurses.<br>
For the best UI experience, change the resolution of the PI to the max supported and run terminal full screen.<br>
This will prevent scaling issues with ncurses.<br>

#########################################################
       <br><h2>To Install HomeBrew (MacOS)</h2>
#########################################################

<h3>Note:</h3>
-----<br>
HomeBrew will be needed to run the code on MacOS.<br>
<br>
1) Open Terminal<br>
2) Enter the following command:<br>
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"<br>


#########################################################
             <br><h2>To Install CURL</h2>
#########################################################

<h3>MacOS:</h3>
------<br>
Installing cURL<br>
1) Open Terminal<br>
2) Install HomeBrew using the above steps before continuing.<br>
3) Once completed, run the final command:<br>
    &#09;brew install curl<br>
4) That's it, curl is now installed to:<br>
    &#09;/usr/local/Cellar/curl/7.62.0/<br>
5) The CMakeLists.txt file will need to include:<br>

        set(CURL_LIBRARY "-lcurl")
        find_package(CURL REQUIRED)

        TARGET_LINK_LIBRARIES(StockWidget ${CURL_LIBRARIES})
        TARGET_LINK_LIBRARIES(NewsWidget ${CURL_LIBRARIES})
        TARGET_LINK_LIBRARIES(WeatherWidget ${CURL_LIBRARIES})

<h3>Ubuntu / Debian (Pi)</h3>
------<br>
1) Open Terminal<br>
2) Enter the following command:<br>
     &#09;sudo apt-get install curl<br>
3) If you still run into errors, try the following command as well:<br>
    &#09;sudo apt-get install libcurl4-openssl-dev<br>

#########################################################
             <br><h2>To Install NCURSES</h2>
#########################################################

<h3>MacOS:</h3>
------<br>
Installing NCURSES<br>
1) Open Terminal<br>
2) Install HomeBrew using the above steps before continuing.<br>
3) Once completed, run the final command:<br>
    &#09;brew install ncurses<br>
4) That's it, curl is now installed.<br>
5) The CMakeLists.txt file will need to include:<br>

        find_package(CURSES REQUIRED)

<h3>Ubuntu / Debian (Pi)</h3>
------<br>
1) Open Terminal<br>
2) Enter the following command:<br>
     &#09;sudo apt-get install libncurses5-dev libncursesw5-de<br>

#########################################################
             <br><h2>To Install cmake</h2>
#########################################################

<h3>Mac OS:</h3>
------<br>
1) Follow the directions above to install HomeBrew<br>
2) brew install cmake<br>
3) cmake is now installed

<h3>Ubuntu / Debian (Pi)</h3>
------<br>
1) Open Terminal<br>
2) Enter the following commands:<br>
    &#09;sudo apt-get update<br>
    &#09;sudo apt-get upgrade<br>
    &#09;sudo apt-get install cmake<br>


#########################################################
          <br><h2>To Run The Application</h2>
#########################################################

1) Open Terminal and set the window size to be full screen (this will prevent odd UI issues with ncurses)
2) Enter the following commands:
    cd [directory of project files]
    cmake .
    make
3) Run ./MainInterface to run the main program.<br>
    &#09;<b>Note:</b> A sample login is saved as:<br>
    &#09;&#09;<u>Username:</u> testuser<br>
    &#09;&#09;<u>password:</u> password
4) Executables for widgets can also be run to test them on the back end.<br>
    &#09;./StockWidget - runs the stock widget program<br>
    &#09;./NewsWidget - runs the news widget program<br>
    &#09;./WeatherWidget - runs the weather widget program<br>
    &#09;./DateWidget - runs the date widget program<br>
    &#09;./TimeDateWidget - runs the time widget program<br>
    &#09;./UserPreferences - runs the user preference widget program<br>
    &#09;./TestUserSignIn - runs the user sign in program<br>
