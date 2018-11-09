/* 
 * File:   CTimeSeriesBuilder.cpp
 * Author: santony
 * 
 * Created on June 20, 2015, 11:42 AM
 */

#include "sepredictinternal.h"
#include "CTimeSeriesBuilder.h"
#include "../seeconomy/seeconomapi.h"
#include "../seglobal/globallogger.h"

using namespace se::predict;

CTimeSeriesBuilderBase::CTimeSeriesBuilderBase(ISession* ps) : _ps(ps)
{
    
}
    
CTimeSeriesBuilderBase::CTimeSeriesBuilderBase(ISession* ps, IAttributeDescriptionBase* attrib) : 
    _ps(ps),
    _pattrib(attrib)
{
    
}


CTimeSeriesBuilderBase::~CTimeSeriesBuilderBase() 
{
    
}

IAttributeDescriptionBase* CTimeSeriesBuilderBase::get_Attrib() 
{
    return _pattrib;
}

void CTimeSeriesBuilderBase::set_Attrib(IAttributeDescriptionBase* attrib) 
{
    _pattrib = attrib;
}


std::shared_ptr<CTimeSeriesBuilderBase> CTimeSeriesBuilderBase::Build(ISession* ps, IAttributeDescriptionBase* attrib) 
{
    CTimeSeriesBuilderBase* pBuilder = nullptr;
    switch (attrib->get_Context())
    {
        case TrainingDataContext::Custom:
        {
            CustomTimeSeriesBuilders::iterator it;
            if ((it = _customBuilders.find(attrib->get_Id()))!=_customBuilders.end())
            {
                pBuilder = it->second;
                pBuilder->set_Attrib(attrib);
            }
            break;
        }
        case TrainingDataContext::EconomicIndicator:
            pBuilder = new CEconomicDataTimeSeriesBuilder(ps , attrib);
            break;
        case TrainingDataContext::FirmPerformance:
            pBuilder = new CFirmDataTimeSeriesBuilder(ps, attrib);
            break;
        case TrainingDataContext::SectorPerformance:
            pBuilder = new CSectorDataTimeSeriesBuilder(ps, attrib);
            break;
        case TrainingDataContext::IndustryPerformance:
            pBuilder = new CIndustryDataTimeSeriesBuilder(ps, attrib);
            break;
        case TrainingDataContext::SecurityPerformance:
            pBuilder = new CSecurityDataTimeSeriesBuilder(ps, attrib);
            break;
    }
    return std::shared_ptr<CTimeSeriesBuilderBase>(pBuilder);
}

long unsigned int CTimeSeriesBuilderBase::GetTimeSeriesTable(ISession* ps, IAttributeDescriptionBase* pIn, se::training::ITrainingTable** ppOut) 
{
    std::shared_ptr<CTimeSeriesBuilderBase> builder = Build(ps, pIn);
    if (!builder)
        throw seexception(E_NoDataToReturn, "Time series builder not found.");
    *ppOut = builder->get_Table();
    return S_OK;
}


////////////////////// Invididual Timeseries Builders /////////////////////

CEconomicDataTimeSeriesBuilder::CEconomicDataTimeSeriesBuilder(ISession* ps,  IAttributeDescriptionBase* attr) : 
            CTimeSeriesBuilderBase(ps, attr)
{
    
}

se::training::ITrainingTable* CEconomicDataTimeSeriesBuilder::get_Table() 
{
    using namespace se::economy;
    using namespace se::training;
    DATAPERIOD dpstart, dpend;
    dpstart = _pattrib->get_StartPeriod();
    dpend = _pattrib->get_EndPeriod();
    UID ecoid = _pattrib->get_Id();
    
    SP<IEconomyApi> peapi ;
    THROWIFFAILED(EconOpen(_ps, &peapi), "Failed to open the training api.");
    
    SP<se::training::ITrainingTable> ptable;
    THROWIFFAILED(peapi->get_EconomicIndicatorTrainingTable(ecoid, 0, 0, dpstart, dpend, &ptable), "Failed to load the economic training table.");
    
    return ptable;
    
}
