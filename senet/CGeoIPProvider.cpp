/* 
 * File:   CGeoIPProvider.cpp
 * Author: santony
 * 
 * Created on August 16, 2015, 3:15 PM
 */

#include <sstream>

#include "senetinternal.h"
#include "CGeoIPProvider.h"
#include "CHttpClient.h"


using namespace se;
using namespace se::net;

CGeoIPProvider::CGeoIPProvider() 
{
}

CGeoIPProvider::CGeoIPProvider(const CGeoIPProvider& orig) {
}

CGeoIPProvider::~CGeoIPProvider() 
{
}

long unsigned int CGeoIPProvider::get_GeoIpCountry(const char* ipAddress, IString** ppOut) 
{
    //ipinfo.io/74.205.59.143/json
    // output example

/*
 {
  "ip": "8.8.8.8",
  "hostname": "google-public-dns-a.google.com",
  "city": "Mountain View",
  "region": "California",
  "country": "US",
  "loc": "37.3860,-122.0838",
  "org": "AS15169 Google Inc.",
  "postal": "94040"
 }
*/    
    
    if (!ipAddress)
    {
        BERR << "Invalid ipaddress";
        return E_InvalidArg;
    }
    
    CHttpClient client;
    client.EnableFollowLocation();
    client.Timeout(5);
    std::ostringstream ostr ;
    ostr << "http://ipinfo.io/" << ipAddress << "/json";
    return client.GetBuffer(ostr.str().c_str(), ppOut);
}
