/* 
 * File:   application.cpp
 * Author: santony
 * 
 * Created on March 12, 2015, 9:33 PM
 */

#include "application.h"
#include <mongo/client/dbclient.h>


application::application() 
{
    mongo::client::initialize();
}


application::~application() 
{
}

