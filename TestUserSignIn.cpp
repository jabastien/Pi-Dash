/**
* @brief A test function set for the sign in widget
* @detailed Tests the sign in widget's ability to change information and sign in
* @parameters Takes no parameters, creates an executable file as per the README file for testing
* @returns 1, 0, -1 (system codes)
* @author Zhaohe Zhang
*/


#include "userSignIn.h"
using namespace std;

int main() {
    userSignIn user = userSignIn("hello","1234");
    cout << user.getSignInStatus() << endl;
    userSignIn user2 = userSignIn("testuser","1234");
    cout << user2.getSignInStatus() << endl;
    userSignIn user3 = userSignIn("testuser","password");
    cout << user3.getSignInStatus() << endl;
    userSignIn user4 = userSignIn("testuser2","password");
    cout << user4.getSignInStatus() << endl;
    return 0;
}