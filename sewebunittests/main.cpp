/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 22, 2015, 8:15 AM
 */

#include <cstdlib>
#include "../semain/semain.h"
#include "../sewebmodels/CRegistration.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    archive a;
    CRegistration rg;
    rg.EmailAddress = "sajiantony@hotmail.com";
    rg.Remember = true;
    rg.serialize(a);
    string str = a.str();
    cout << str;
    
    // reload
    CRegistration rg1 ;
    a.str(str);
    cout << rg1.EmailAddress;

    return 0;
}

