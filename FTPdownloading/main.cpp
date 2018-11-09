/* 
 * File:   main.cpp
 * Author: arun
 *
 * Created on 26 October, 2012, 2:24 PM
 */

//#include <libxml/tree.h>
//#include <libxml/parser.h>
//#include <libxml/HTMLparser.h>
//#include <libxml/nanohttp.h>
//#include <libxml/xmlreader.h>
//#include<libxml/nanoftp.h>
#include <sstream>
#include <stdexcept>
#include<stdio.h>

#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <cstring>


#include <cstdlib>
#include<string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <vector>
#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>

#include<ftplib.h>


//#include "../semysql/CSEConnections.h"
//#include<mysql++/mysql++.h>
//#include <mysql++/ssqls.h>
#include <sstream>

//#define SEDBCONN        CSEConnections::GetMain()



using namespace std;


/*
 * 
 */

void FtpDownload(string filename,string location,string file){
   
    string URL = location+file;
    string output = filename+file;
    cout<<URL<<endl;
    cout<<output<<endl;
    int con,res;
    char *url;
    char *Output;
    url = &URL[0];
    Output = &output[0];
    
    netbuf *pnetBuf;
    netbuf *data;
    cout<<pnetBuf<<endl;
    
    con=FtpConnect("ftp.sec.gov",&pnetBuf);
    
    cout<<"open  "<<con<<endl;
    cout<<pnetBuf<<endl;   
   
    
    int i = FtpGet(Output,url,FTPLIB_ASCII,pnetBuf);
    
    
     //FtpClose(data);
}


int main(int argc, char** argv) {
    //ofstream ftp;
    //ftp.open("/home/arun/Documents/ftpData/sndk-20120401_pre.xml",ios::app);
    
   
    string output = "/home/arun/Documents/ftpData/";
    //const char *path = "ftp://ftp.sec.gov/edgar/data/1000180/000100018012000029/sndk-20120401_cal.xml"; 
    string path = "/edgar/data/1000180/000100018012000029/";
    string file1 = "sndk-20120401_pre.xml";
    //FtpDownload(output,path,file1);
    string file2= "sndk-20120401.xml";
    FtpDownload(output,path,file2);
    //ftp.close();

     return 0;
    
}

