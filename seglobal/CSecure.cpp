/* 
 * File:   CSecure.cpp
 * Author: santony
 * 
 * Created on August 10, 2015, 3:56 PM
 */

#include "seglobalinternal.h"
#include "CSecure.h"

CSecure::CSecure() {
}

CSecure::CSecure(const CSecure& orig) {
}

CSecure::~CSecure() {
}

void CSecure::GenerateRandomPassword(char* freeBuffer, size_t len) 
{
    srand(time(NULL));
    const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz$@#^&*";
    for (int i=0; i!=len; ++i)
    {
        freeBuffer[i] = alphanum[rand() % sizeof(alphanum)-1];
    }
    freeBuffer[len] = 0;
}
