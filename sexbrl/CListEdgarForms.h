/* 
 * File:   CListEdgarForms.h
 * Author: santony
 *
 * Created on July 11, 2012, 7:58 PM
 */

#ifndef CLISTEDGARFORMS_H
#define	CLISTEDGARFORMS_H

#include <string>

#include "../seglobaltypedefs.h"
#include "sexbrlapi.h"
#include "CAsyncProcessor.h"

////using namespace log4cxx;

/*
 * a.   List the contents of a form.idx file for the 
 *      given cik, year and quarter.
 * 
 * b.   Add each entry to a database table. Ensure that if the class
 *      is called with same year and quarter, new entries are not made
 *      nor original entries are modified.
 * 
 * 
 */

using namespace se;
using namespace se::xbrl;

class CListEdgarForms : 
        public CAsyncProcessor< IXbrlCompletionEvent<int> >
{
private:
    int _year ;
    int _quarter ;
    std::string _formType;
    
public:
    CListEdgarForms(int year, int quarter, CSTR formType);
    CListEdgarForms(const CListEdgarForms& orig);
    virtual ~CListEdgarForms();

    virtual ErrorId Process(IXbrlCompletionEvent<int>* listener);
    
    
    /* internal */
public:
    //bool IsProcessed(CIK cik, STATEMENTTYPES type, const DATAPERIOD& period);    //TODO
    
    
private:
    
    /** parses a record and returns an array of records. 
        The return value is the size of array. */
    int ParseRecord(std::string** records);
    
    void UpdateDatabase(std::string* records, int size);

private:
    ////LoggerPtr _logger;
    
};

#endif	/* CLISTEDGARFORMS_H */

