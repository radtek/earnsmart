/* 
 * File:   CParseCompanyProfile.h
 * Author: santony
 *
 * Created on 5 September, 2012, 6:34 AM
 */

#ifndef CPARSECOMPANYPROFILE_H
#define	CPARSECOMPANYPROFILE_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/nanohttp.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include "../seglobaltypedefs.h"
#include "../ReturnCodes.h"
#include "sesecuritiesapi.h"
#include "PublicCompanyInfo.h"

// step one for lognet
//#include <log4cxx/logger.h>

using namespace se;
using namespace se::securities;

class CParseCompanyProfile 
{
public:
    CParseCompanyProfile(ISession* ps, ISecurityDescription* secDesc);
    CParseCompanyProfile(const CParseCompanyProfile& orig)=delete;
    virtual ~CParseCompanyProfile();
    ErrorId Process(CPublicCompanyInfo*);
    const char* ReadQuote() ;
    const char* ReadAddress();
    const char* ReadDetails() ;
    const char* ReadSummary() ;
    
    ErrorId GetCompany(CPublicCompanyInfo*);
    
private:
    void ParseChildren(xmlDoc* doc, xmlNode* node);
    int Convert(const char* inbuff, int length, char** outBuff);
    
private:
    std::string _sHtmlText;
    SP<ISecurityDescription> _pSecDesc;
    xmlDoc* _pDoc;
    xmlNode* _pNodeQuote, 
            *_pNodeDetails,
            *_pNodeSummary;
    std::string _sInfoText;     // holds the string that is returned by above ReadXXX methods.
    SP<IPublicCompanyInfo> _pCoInfo;
    // step 2: create a logger instance specifically for logging this class 
    //log4cxx::LoggerPtr _pLogger;
    ISession* _ps;
    
};


#endif	/* CPARSECOMPANYPROFILE_H */

