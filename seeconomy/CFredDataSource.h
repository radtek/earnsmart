/* 
 * File:   CFredDataSource.h
 * Author: santony
 *
 * Created on September 10, 2015, 1:39 PM
 */

#ifndef CFREDDATASOURCE_H
#define	CFREDDATASOURCE_H

#include "seeconomapi.h"
#include "../senet/CHttpClient.h"
#include "CEconomicIndicator.h"


using namespace se;
using namespace se::economy;
using namespace se::net;

////////////////////// Fred Data Source ////////////////////////

class CFredDataSource : public CObjectRoot, public IFredDataSource
{
public:
    CFredDataSource(ISession*);
    CFredDataSource(const CFredDataSource& orig) = delete;
    virtual ~CFredDataSource();


    virtual long unsigned int Init();


    virtual long unsigned int get_Category(unsigned long categoryId, IIndicatorCategory**);

    virtual long unsigned int get_CategoryChildren(unsigned long categoryId, IIndicatorCategoryList**);

    virtual long unsigned int get_RelatedCategories(unsigned long categoryId, IIndicatorCategoryList**);

    virtual long unsigned int get_Indicator(CSTR code, IEconomyIndicator**);


    virtual long unsigned int get_IndicatorObservations(const char* code, se::training::ITrainingTable**, const char* frequency, const char* aggr, const CDate& start, const CDate& end);


    virtual long unsigned int get_IndicatorsByCategory(unsigned long categoryId, IEconomyIndicatorList**);


    virtual long unsigned int get_IndicatorCategories(const char* code, IIndicatorCategoryList**);

    
protected:
    
    
    SE_CODE save_CategoryTags();
    SE_CODE save_RelatedCategoryTags();
    SE_CODE save_Releases();
    SE_CODE save_ReleaseDates();
    SE_CODE save_ReleaseSources();
    SE_CODE save_ReleaseTags();
    SE_CODE save_RelatedReleaseTags();
    // IMP
    SE_CODE save_SeriesUpdates();
    

    virtual long unsigned int find_Indicators(const char* searchText, IIndicatorCategoryList**);

    
    string _fredKey ;
    string _fredRoot;
    CHttpClient _http;
    string _defFreq = "m";
    
private:
    

};




#endif	/* CFREDDATASOURCE_H */

