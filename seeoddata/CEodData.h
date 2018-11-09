/* 
 * File:   CEodData.h
 * Author: santony
 *
 * Created on February 1, 2015, 8:17 PM
 */

#ifndef CEODDATA_H
#define CEODDATA_H

#include "seeoddata.h"


namespace se
{
namespace eoddata
{

class CEodData : public CObjectRoot, public IEODData
{
public:
    CEodData(ISession*);
    virtual ~CEodData();

    NamedObjectNamingType get_TypeId() override;


    virtual long unsigned int Init();


    virtual long unsigned int LoginToWSClient(const char* userName, const char* password, IWSClientSession** ppOut);
    virtual SE_CODE OpenDefault(IWSClientSession** ppOut);

    long unsigned int ReadEODFile(const string& fileName, const EODFileTraits& traits) override;


private:
    CEodData(const CEodData& orig);

};
}
}


#endif /* CEODDATA_H */

