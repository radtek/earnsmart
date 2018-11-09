/* 
 * File:   CParseCompanyProfile.cpp
 * Author: santony
 * 
 * Created on 5 September, 2012, 6:34 AM
 */

#include <iostream>
#include <string>
//#include<curl/curl.h>
#include <fstream>


#include<boost/tokenizer.hpp>
#include<boost/regex.hpp>
#include <log4cxx/logger.h>

#include "../ReturnCodes.h"

#include "CParseCompanyProfile.h"

#include <libxml/xmlstring.h>
#include <iconv.h>

using namespace std;


CParseCompanyProfile::CParseCompanyProfile(const char* ticker) :
        _sTicker(ticker),
        _pDoc(0),
        _pNodeQuote(0), 
        _pNodeDetails(0),
        _pNodeSummary(0)
{
}

CParseCompanyProfile::CParseCompanyProfile(const CParseCompanyProfile& orig) 
{
    
}

CParseCompanyProfile::~CParseCompanyProfile() 
{
    
}

//void parseStory (xmlDocPtr doc, xmlNodePtr cur) 
//{
//    xmlChar *key;
//    cur = cur->xmlChildrenNode;
//    while (cur != NULL) {
//        if ((!xmlStrcmp(cur->name, (const xmlChar *)"title"))) 
//        {
//            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
//            //printf("title: %s\n", key);
//            cout<<"title:\t"<<key<<endl;
//            xmlFree(key);
//        }
//        if ((!xmlStrcmp(cur->name, (const xmlChar *)"p"))) 
//        {
//            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
//            //printf("title: %s\n", key);
//            cout<<"summary:\t"<<key<<endl;
//            xmlFree(key);
//        }
//        xmlChar *uri;
//        if ((!xmlStrcmp(cur->name, (const xmlChar *)"div"))) {
//            uri = xmlGetProp(cur,(const xmlChar *)"uri");
//            //printf("uri: %s\n", uri);
//            cout<<"atributes\t"<<uri<<endl;
//            xmlFree(uri);
//        }
//        cur = cur->next;
//    
//    }
//    
//    return;
//}

void CParseCompanyProfile::ParseChildren(xmlDocPtr doc, xmlNodePtr node)
{
    /*
     * What we are looking for are these elements with attributes as below:
     * a. For quotes: <div class="yfi_rt_quote_summary">
     * b. For "Details" table: table class='yfnc_mod_table_title1'
     * c. For Business summary: table class='yfnc_modtitle1'
     */
    
    // scan all children and siblings until above links are found. 
    // store them in class members.

    //xmlChar* sXpathForQuotes = (xmlChar*)"//div[@class='yfi_rt_quote_summary']";
    //xmlChar* sXpathForDetails = (xmlChar*)"//table[@class=\"yfnc_mod_table_title1\"]";
    //xmlChar* sXpathForSummary = (xmlChar*)"//table[@class=\"yfnc_modtitle1\"]";
    
    // we are going to parse the children until the above three are found.
    for (auto n = node; n ; n = n->next)
    {
        if (!xmlStrcmp(n->name, (xmlChar*)"div") || 
                !xmlStrcmp(n->name, (xmlChar*)"table"))
        {
            for (auto attr = n->properties; attr; attr = attr->next)
            {
                if (!xmlStrcmp(attr->name, (xmlChar*)"class"))
                {
                    
                }
            }
        }
        ParseChildren(doc, n);
    }
    
    
    
    
    /*
    
    xmlXPathContextPtr xpcontext;
    xmlXPathObjectPtr xpObject;
    xpcontext = xmlXPathNewContext(doc);
    if (xpcontext == 0)
    {
        cout << "error" << endl;
        return;
    }
    
    // obtain Quote summary.
    xpObject = xmlXPathEvalExpression(sXpathForQuotes, xpcontext);
    if (xpObject == 0)
    {
        cout << "error" << endl;
        return;
    }
    if (xmlXPathNodeSetIsEmpty(xpObject->nodesetval))
    {
        xmlXPathFreeObject(xpObject);
        cout << "error" << endl;
        return;
    }
    if (xpObject) 
    {
        auto nodeset = xpObject->nodesetval;
        for (auto i=0; i < nodeset->nodeNr; i++) 
        {
            if (nodeset->nodeTab[i]->type == XML_ELEMENT_NODE)
            {
                _pNodeQuote = nodeset->nodeTab[i];
            }
        }
        xmlXPathFreeObject (xpObject);
    }    
    
    // obtain details
    xpObject = xmlXPathEvalExpression(sXpathForDetails, xpcontext);
    if (xpObject == 0)
    {
        cout << "error" << endl;
        return;
    }
    if (xmlXPathNodeSetIsEmpty(xpObject->nodesetval))
    {
        xmlXPathFreeObject(xpObject);
        cout << "error" << endl;
        return;
    }
    if (xpObject) 
    {
        auto nodeset = xpObject->nodesetval;
        for (auto i=0; i < nodeset->nodeNr; i++) 
        {
            if (nodeset->nodeTab[i]->type == XML_ELEMENT_NODE)
            {
                _pNodeDetails = nodeset->nodeTab[i];
            }
        }
        xmlXPathFreeObject (xpObject);
    }    
    
    // obtain company summary
    xpObject = xmlXPathEvalExpression(sXpathForSummary, xpcontext);
    if (xpObject == 0)
    {
        cout << "error" << endl;
        return;
    }
    if (xmlXPathNodeSetIsEmpty(xpObject->nodesetval))
    {
        xmlXPathFreeObject(xpObject);
        cout << "error" << endl;
        return;
    }
    if (xpObject) 
    {
        auto nodeset = xpObject->nodesetval;
        for (auto i=0; i < nodeset->nodeNr; i++) 
        {
            if (nodeset->nodeTab[i]->type == XML_ELEMENT_NODE)
            {
                _pNodeSummary = nodeset->nodeTab[i];
            }
        }
        xmlXPathFreeObject (xpObject);
    }    

    // free context
    xmlXPathFreeContext(xpcontext);
      
     */
}

int CParseCompanyProfile::Process()
{
    if (_pDoc != 0)
    {
        _pNodeQuote = _pNodeDetails = _pNodeSummary = 0;
        xmlFreeDoc(_pDoc);
    }
    
    string url1="http://finance.yahoo.com/q/pr?s=";
    string Surl=url1+_sTicker;
    char* url;
    xmlDocPtr doc;
    xmlNodePtr cur; 
    url=&Surl[0];
    doc = _pDoc = htmlParseFile (url, NULL);
    cur = xmlDocGetRootElement(doc);
    this->ParseChildren(doc, cur);
    xmlFreeDoc(doc);
    return S_Success;
    
//    cout<<"name\t"<<cur->name<<endl;
//    cur = cur->xmlChildrenNode;
//    cout<<"name\t"<<cur->name<<endl;
//    while (cur != NULL) 
//    {
//        if ((!xmlStrcmp(cur->name, (const xmlChar *)"head")))
//        {
//            parseStory (doc, cur);
//        }
//        if ((!xmlStrcmp(cur->name, (const xmlChar *)"body")))
//        {
//            parseStory (doc, cur);
//        }
//        cout<<cur->name<<endl;
//        cur = cur->next;
//   }

    
 }

const char* CParseCompanyProfile::ReadQuote()
{
    /*
    auto n = _pNodeQuote->children;
    while (n)
    {
        if (n->type == XML_ELEMENT_NODE)
        {
            _sInfoText += "<" ;
            _sInfoText += (const char*) n->name;
        }
        else if (n->type == XML_TEXT_NODE)
        {
            _sInfoText += (const char*) xmlNodeListGetString(_pDoc, n, 0) ;
        }
        n = n->next;
    }
    */
    
    auto buffer = xmlBufferCreate();
    if (xmlNodeDump(buffer, _pDoc, _pNodeQuote, 10, 1))
    {
        const char* inBuff = (const char*) buffer->content;
        
        //char* outBuff;
        //const char* inBuff = (const char*) xmlBufferContent(buffer);
        //if (Convert(inBuff, strlen(inBuff),  &outBuff ))
        {
            _sInfoText = inBuff;
        }
       
        //free((void*)inBuff);
        //delete[] outBuff;
    }
    else
        _sInfoText = "";
    xmlBufferFree(buffer);
    return _sInfoText.c_str();
}

const char* CParseCompanyProfile::ReadDetails()
{
    return _sInfoText.c_str();
}
const char* CParseCompanyProfile::ReadSummary()
{
    return _sInfoText.c_str();
}
  
int CParseCompanyProfile::Convert(const char* inbuff, int length, char** outBuff)
{
    iconv_t convDesc = iconv_open("US-ASCII", "UTF-8");
    if ( convDesc == 0)
        return 0;
    
    size_t length1, length2;
    length1 = (size_t)length;
    length2 = length1 + 1; 
    
    *outBuff = new char[length2+1];
    size_t iconVal = iconv(convDesc, (char**)&inbuff, &length1, (char**)outBuff, &length2);
    if (iconVal <=0 )
    {
        delete[] *outBuff;
        outBuff = 0;
    }    
    iconv_close(convDesc);
}
