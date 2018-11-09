/* 
 * File:   main.cpp
 * Author: Arun
 *
 * Created on 10 September, 2012, 10:21 AM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/nanohttp.h>
#include <libxml/xmlreader.h>

#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>

using namespace std;

/*
 * 
 */


int main(int argc, char** argv) 
{
    const string pattern1="(.*)(</tr></table><p>)(.*)(</p><p><a href=)(.*)";
    boost::regex regexPattern1(pattern1, boost::regex::extended);
    boost::smatch what1;
    
    const string pattern3="(.*)(>Index Membership:</td><td class=\")(.*)(>)(.*)(</td></tr><tr><td)(.*)(>Sector:</td><td)(.*)";
    boost::regex regexPattern3(pattern3, boost::regex::extended);
    boost::smatch what3;
    
    const string pattern="(.*)(>Index Membership:</td><td class=)(.*)(>Sector:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>Industry:</td><td)(.*)(>)(.*)(</a></td></tr><tr><td)(.*)(>)(.*)(</td></tr></table></td></tr></table><br/><table)(.*)(>Business Summary</span></th><th)(.*)";
   
    boost::regex regexPattern(pattern, boost::regex::extended);
    boost::smatch what;
    
    const string pattern2="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(><b>)(.*)(</b><br/>)(.*)(<br/>)(.*)(<br/>)(.*)(<a href=)(.*)(>Map</a><br/>)(.*)(>)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern2(pattern2, boost::regex::extended);
    boost::smatch what2;
    
    const string pattern4="(.*)(>Index Membership:</td><td class=\")(yfnc_tabledata1\")(><a href=\")(http)(://)(finance.yahoo.com/)(q)(\\?)(s=%)(5EDJA)(&)(amp)(;)(d=t)(\")(>)(.*)(</a><br/)(><a href=\")(http)(://)(finance.yahoo.com/)(q)(\\?)(s=%)(5EDJI)(&)(amp)(;)(d=t)(\")(>)(.*)(</a>)(.*)(>Sector:</td><td)(.*)";
    boost::regex regexPattern4(pattern4, boost::regex::extended);
    boost::smatch what4;
    
    const string pattern5="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(Fax:)(.*)(<br/>Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern5(pattern5, boost::regex::extended);
    boost::smatch what5;
    
    const string pattern6="(.*)(<tr><td><big><b>Profile</b></big></td><td)(.*)(>Map</a><br/>)(Phone:)(.*)(<br/>)(.*)(Website:)(.*)(</a><br/><br/><table width=)(.*)(>Details</span></th><th align=)(.*)";
    boost::regex regexPattern6(pattern6, boost::regex::extended);
    boost::smatch what6;
    
    string ticker="ibm";
    string url1="http://finance.yahoo.com/q/pr?s=";
    string Surl=url1+ticker;
    char* url;
    xmlChar *key;
    xmlDocPtr doc;
    xmlNodePtr cur; 
    url=&Surl[0];
    xmlBufferPtr nodeBuffer = xmlBufferCreate();
    doc = htmlParseFile (url, NULL);
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    cur = cur->next;
    xmlNodeDump(nodeBuffer, doc, cur, 0, 1);
    key=nodeBuffer->content;
    string answer = (const char *)key;
    cout<<"entering....."<<endl; 
    int flag=0,flag1=0;
  
    bool isMatchFound2 = boost::regex_match(answer, what2, regexPattern2);   
        if (isMatchFound2)
            {
                  for (unsigned int k=0; k < what2.size(); k++)
                  {
                    if(k==5){
                        cout<<"PROFILE"<<endl;
                        cout<<what2[5]<<endl;
                        cout<<what2[7]<<endl;
                        cout<<what2[9]<<endl;
                         cout<<what2[11]<<endl;
                        cout<<what2[17]<<endl;
                        cout<<endl;
                     }
                  }
            }
   
    
    bool isMatchFound5 = boost::regex_match(answer, what5, regexPattern5);   
        if (isMatchFound5)
            {
                  flag1=1;
                  for (unsigned int k=0; k < what5.size(); k++)
                  {
                    if(k==2){                      
                        cout<<"Phone:"<<what5[6]<<endl;
                        cout<<"Fax:"<<what5[9]<<endl;
                        cout<<endl;
                     }
                  }
            }
    
   
    if(flag1==0)
    {
        bool isMatchFound6 = boost::regex_match(answer, what6, regexPattern6);   
            if (isMatchFound6)
                {
                      for (unsigned int k=0; k < what6.size(); k++)
                      {
                        if(k==2){                      
                            cout<<"Phone:"<<what6[6]<<endl;

                            cout<<endl;
                         }
                      }
                }
    }
 
   bool isMatchFound4 = boost::regex_match(answer, what4, regexPattern4);  
    
        if (isMatchFound4)
            {
            flag=1;
                  for (unsigned int i=0; i < what4.size(); i++)
                  {
                    if(i==1){
                        cout<<endl;
                        cout<<"Details"<<endl;
                        cout<<endl;
                        cout<<"Index Membership:\t"<<endl;
                        cout<<"\t"<<what4[18]<<endl;
                        cout<<"\t"<<what4[34]<<endl;
                        cout<<endl;
                     }
                  }
            }
    
  
    if(flag==0)
    {
        bool isMatchFound3 = boost::regex_match(answer, what3, regexPattern3);   
            if (isMatchFound3)
                {
                      for (unsigned int i=0; i < what3.size(); i++)
                      {
                        if(i==1){
                            cout<<endl;
                            cout<<"Details"<<endl;
                            cout<<endl;
                            cout<<"Index Membership:\t"<<what3[5]<<endl;
                            cout<<endl;
                         }
                      }
                }
        cout<<"end"<<endl;
    }
     bool isMatchFound = boost::regex_match(answer, what, regexPattern);  
     
        if (isMatchFound)
            {
                  for (unsigned int i=0; i < what.size(); i++)
                  {
                    if(i==1){
                        cout<<endl;
                       
                        cout<<endl;
                        //cout<<"Index Membership:\t"<<what[5]<<endl;
                        //cout<<"\t"<<what[9]<<endl;
                        cout<<"Sector\t"<<what[7]<<endl;
                        cout<<"Industry\t"<<what[13]<<endl;
                        cout<<"Full Time Employees:\t"<<what[17]<<endl;
                        
                        cout<<endl;
                     }
                  }
            }
     
    bool isMatchFound1 = boost::regex_match(answer, what1, regexPattern1);   
        if (isMatchFound1)
            {
                  for (unsigned int j=0; j < what1.size(); j++)
                  {
                    if(j==3){
                        cout<<endl;
                        cout<<"Business Summary"<<endl;
                        cout<<endl;
                        cout<<what1[3]<<endl;
                        cout<<endl;
                     }
                  }
            }

   
}

