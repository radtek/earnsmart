/* 
 * File:   CXbrlJsonOutputs.h
 * Author: santony
 *
 * Created on February 4, 2014, 2:30 PM
 */

#ifndef CXBRLJSONOUTPUTS_H
#define	CXBRLJSONOUTPUTS_H

using namespace se;
using namespace se::xbrl;

class CXbrlJsonOutputs : public CObjectRoot, public IJsonOutputController
{
public:
    CXbrlJsonOutputs(ISession*);
    CXbrlJsonOutputs(const CXbrlJsonOutputs& orig)=delete;
    virtual ~CXbrlJsonOutputs();
    

    virtual long unsigned int get_YearlyData(CIK cik, std::initializer_list<unsigned long> xids, std::ostream &outputstream);

    
private:

};

#endif	/* CXBRLJSONOUTPUTS_H */

