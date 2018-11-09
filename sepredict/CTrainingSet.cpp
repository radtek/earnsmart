/* 
 * File:   CTrainingSet.cpp
 * Author: santony
 * 
 * Created on May 14, 2015, 6:55 PM
 */

#include "sepredictinternal.h"
#include "CTrainingSet.h"
#include "../seglobal/globallogger.h"

using namespace se::predict;

CTrainingSet::CTrainingSet(ISession* ps) : CObjectRoot(ps) 
{
    
}


long unsigned int CTrainingSet::Init() 
{
    auto ret = CObject<CTrainingSetList>::Create(get_Session(), &_dependents);
    
    return ret;
}


CTrainingSet::~CTrainingSet() 
{
    
}

long unsigned int CTrainingSet::Init(signed int tsetId) 
{
    set_Id(tsetId);
    return CObject<CTrainingSetList>::Create(get_Session(), &_dependents);
}

const char* CTrainingSet::get_Name() 
{
    return _sName.c_str();
}

void CTrainingSet::set_Name(const char* val) {
    _sName = val;
    if (_sDesc.empty())
        _sDesc = _sName;
}

const char* CTrainingSet::get_Desc() {
    return _sDesc.c_str();
}

void CTrainingSet::set_Desc(const char* val) {
    _sDesc = val;
}



PredictionAlgos CTrainingSet::get_Algo() {
    return _algo;
}

void CTrainingSet::set_Algo(PredictionAlgos algo) {
    _algo = algo;
}


IResultAttribDescription* CTrainingSet::get_ResultContext() {
    return _resultContext;
}

void CTrainingSet::set_ResultContext(IResultAttribDescription* value) 
{
    _resultContext = dynamic_cast<CResultAttributeDescription*>(value);
}


void CTrainingSet::AddDependent(ITrainingSet* tsetChild) 
{
    _dependents->Add(tsetChild);
}

void CTrainingSet::RemoveDependent(signed int tsetId) 
{
    _dependents->RemoveById(tsetId);
}

void CTrainingSet::Serialize(ISerializedData<IString>* pIn) {

}


ITrainingSetList* CTrainingSet::get_Dependents() throw(std::exception&)
{
    return _dependents;
}

SE_CODE PREDICTEXP ITrainingSet::CreateorOpen(ISession* ps, CSTR name, ITrainingSet** ppOut)
{
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    return CTrainingSet::_CreateorOpen(ps, conn, name, ppOut);
}

SE_CODE PREDICTEXP ITrainingSet::Get(ISession* ps, UID predictorId, signed int tsetid, ITrainingSet** ppOut) throw(std::exception&)
{
    // create training set hierarchies and return the top object...
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    return CTrainingSet::Get(ps, conn, predictorId, tsetid, ppOut);
}

long unsigned int CTrainingSet::_CreateorOpen(ISession* ps, se::data::CSmartConnection& conn, const char* name, ITrainingSet** ppOut) 
{
    
    Query q = conn->query();
    q << "select set_id from se_trainingsets where set_name=" << quote << name;
    auto sqr = q.store();
    if (sqr.size())
        return _Get(ps, conn, sqr[0][0], ppOut);
    else
    {
        // training set can be opened either by id or name since both are primary keys
        auto r= CObject<CTrainingSet>::Create(ps, ppOut);
        (*ppOut)->set_Name(name);
        return r;
    }
}

long unsigned int CTrainingSet::_Get(ISession* ps, se::data::CSmartConnection& conn, signed int tsetId, ITrainingSet** ppOut) throw(std::exception&)
{

    // populate training set with given algo
     /*
  *  `se_trainingsets` (
     `set_id` int(11) NOT NULL AUTO_INCREMENT,
     `set_name` varchar(1048) NOT NULL,
     `set_desc` varchar(2048) DEFAULT NULL,
     `set_algo` varchar(45) DEFAULT 'mvreg' COMMENT 'mvreg for multivariate regression analysis\nann for artificial neural network',
     `set_default` char(1) DEFAULT '0',
     PRIMARY KEY (`set_id`)
      */
     Query q = conn->query();
     q << "select * from se_trainingsets where set_id=" << tsetId;
     // populate matching trainingset, its attribs and result.
     auto sqr = q.store();
     if (!sqr.size())
         throw seexception(E_NoDataToReturn, "Training set record not found.");
     
     SP<CTrainingSet> _tset;
     auto r = CObject<CTrainingSet>::Create(ps, tsetId, &_tset);
     if (FAILED(r))
         throw seexception(r, "Failed to create trainingset instance");
     
     const Row& rw = sqr[0];
     _tset->set_Algo((PredictionAlgos)(int)rw["set_algo"]);
     _tset->set_Id(tsetId);
     _tset->set_Desc((CSTR)rw["set_desc"]);
     _tset->set_Name((CSTR)rw["set_name"]);

     // populate attribs and results.
     /*
     `se_trainingsets_attribs` (
      `set_id` int(11) NOT NULL,
      `attrib_id` int(11) NOT NULL,
      `source` char(1) NOT NULL COMMENT '''E'' for economic indicator, ''P'' for performance ratios.',
      PRIMARY KEY (`set_id`,`attrib_id`,`source`)         
     */
     q = conn->query();
     q << "select * from se_trainingsets_attribs where set_id = " << tsetId;
     for (const Row& r : q.store())
     {
         SP<CAttributeDescription> pdesc;
         CObject<CAttributeDescription>::Create(ps, r["attrib_id"], (TrainingDataContext)((CSTR)r["source"])[0], &pdesc);
         _tset->add_Attribute(pdesc);
     }
     /* 
      * `se_trainingsets_result` (
      `set_id` int(11) NOT NULL,
      `result_id` int(11) NOT NULL DEFAULT '0',
      `source` char(1) NOT NULL DEFAULT 'P' COMMENT 'source = ''E'' economic indicator ; ''P'' performance ratio',
      PRIMARY KEY (`set_id`,`result_id`,`source`)         
      */

     q = conn->query();
     q << "select * from se_trainingsets_result where set_id = " << tsetId;
     sqr = q.store();
     if (!sqr.size())
         throw seexception(E_NoDataToReturn, "Training set result attribute not found.");

     const Row& rw1 = sqr[0];
     SP<CResultAttributeDescription> presult;
     CObject<CResultAttributeDescription>::Create(ps, rw1["result_id"], (TrainingDataContext)((CSTR)rw1["source"])[0], &presult);
     _tset->set_ResultContext(presult);

     
     _tset.CopyTo(ppOut);
     
     return S_DataExists;
}


long unsigned int CTrainingSet::Get(ISession* ps, se::data::CSmartConnection& conn, UID predictorId, signed int tsetId, ITrainingSet** tset) throw(std::exception&) 
{
    
     
    // RECURSE for dependents
    /*
     `se_trainingsets_tree` (
      `set_id` int(11) NOT NULL,
      `parent_set_id` int(11) NOT NULL,
      `tcp_pkey` int(11) NOT NULL,
      PRIMARY KEY (`set_id`,`parent_set_id`,`tcp_pkey`)        
    */
    
    SP<ITrainingSet> _tset ;
    
    RETURNIFFAILED(_Get(ps, conn, tsetId, &_tset));
    
    Query q  = conn->query();
    q << "select * from se_trainingsets_tree where ";
    q << " parent_set_id = " << tsetId;
    q << " and tcp_pkey=" << predictorId;
    for (const auto& r : q.store())
    {
        SP<ITrainingSet> pdep;
        CTrainingSet::Get(ps, conn, predictorId, r["set_id"], &pdep);
        _tset->AddDependent(pdep);
    }

    _tset.CopyTo(tset);
    
    return S_OK;
}


se::training::IExTrainingTable* CTrainingSet::get_Table(bool refresh) 
{
    if (_dpStart.IsEmpty())
        throw seexception(E_IncorrectOperation, "Starting period is not provided.");
    
    if (_dpEnd.IsEmpty())
    {
        _dpEnd = CDate::Now().ToPeriodForm();
        _dpEnd.Day = 1;
    }
    return nullptr;
}

void CTrainingSet::add_Attribute(IAttributeDescription* attrib) 
{
    if (!_pAttribs)
    {
        SE_CODE c = CObject<CAttributeDescriptionList>::Create(get_Session(), &_pAttribs);
        if (FAILED(c))
            throw seexception(c, "Error creating attribute collection");
    }
    _pAttribs->Add(attrib);
}

void CTrainingSet::rem_Attribute(unsigned long attribId) 
{
    if (!_pAttribs)
        return ;
    _pAttribs->RemoveById(attribId);
}

IAttributeDescriptionList* CTrainingSet::get_Attributes() {
    return _pAttribs;
}

IAttributeDescription* CTrainingSet::get_CaseWeightAttribute()
{
    return _pCaseWeight;
}

void CTrainingSet::set_CaseWeightAttribute(IAttributeDescription* val)
{
    _pCaseWeight = dynamic_cast<CAttributeDescription*>(val);
}

const DATAPERIOD& CTrainingSet::get_StartPeriod() {
    return _dpStart;
}

void CTrainingSet::set_StartPeriod(const DATAPERIOD& dp) {
    _dpStart = dp;
}

const DATAPERIOD& CTrainingSet::get_EndPeriod() {
    return _dpEnd;
}

void CTrainingSet::set_EndPeriod(const DATAPERIOD& dp) {
    _dpEnd = dp;
}


long unsigned int CTrainingSet::_Store(se::data::CSmartConnection& conn) throw(const std::exception&)
{
    Query q = conn->query();
    if (get_Id()) // upate
    {
        q << "update se_trainingsets ";
        q << " set ";
        q << " set_name = " << quote << _sName << ", ";
        q << " set_desc = " << quote << _sDesc << ", ";
        q << " set_algo = " << quote << (int)_algo ;
        q << " where set_id = " << get_Id();
        //BOOSTER_INFO("se::predict::CTrainingSet::_Store") << q.str();
        q.exec();

        q = conn->query();
        q << "delete from se_trainingsets_attribs where set_id = " << get_Id();
        //BOOSTER_INFO("se::predict::CTrainingSet::_Store") << q.str();
        q.exec();

        q = conn->query();
        q << "delete from se_trainingsets_result where set_id=" << get_Id();
        //BOOSTER_INFO("se::predict::CTrainingSet::_Store") << q.str();
        q.exec();
    }
    else
    {
        q << "insert into se_trainingsets ";
        q << " ( set_name, set_desc, set_algo )";
        q << " values (" ;
        q << quote << _sName << ", ";
        q << quote << _sDesc << ", ";
        q << quote << (int)_algo ;
        q << ")";
        //BOOSTER_INFO("se::predict::CTrainingSet::_Store") << q.str();
        q.exec();
        set_Id(q.insert_id());
    }
    
    IAttributeDescriptionList* pAttrl = get_Attributes();
    if (!pAttrl)
        throw seexception(E_IncorrectOperation, "Not attributes available for the training set.");

    for (IAttributeDescriptionBase* pAttr : *pAttrl)
    {
        
        q = conn->query();
        q << "insert into se_trainingsets_attribs values (";
        q << get_Id() << ", ";
        q << pAttr->get_Id() << ", ";
        q << quote << (CHR)pAttr->get_Context();
        q << ")";
        //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
        q.exec();
        pAttr->set_Id(q.insert_id());
    }

    IResultAttribDescription* prattr = get_ResultContext();
    if (!prattr)
        throw seexception(E_IncorrectOperation, "Not attributes available for the training set.");
    q << "insert into se_trainingsets_result values (";
    q << get_Id() << ", ";
    q << prattr->get_Id() << ", ";
    q << quote << (CHR)prattr->get_Context();
    q << ")";
    //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
    q.exec();
    prattr->set_Id(q.insert_id());
    
    if (get_Dependents())
    {
        SP<ITrainingSetList> ptl = get_Dependents();
        for (ITrainingSet* pdep : *ptl)
        {
            THROWIFFAILED(((CTrainingSet*)pdep)->_Store(conn), "Failed to write dependent training set.");
        }
    }
    return S_OK;
}

long unsigned int CTrainingSet::_Delete(se::data::CSmartConnection& conn) throw(const std::exception&)
{
    Query q = conn->query();
    q << "select count(*) from se_trainingsets_tree where set_id = " << get_Id();
    q << " or parent_set_id = " << get_Id();
    auto sqr = q.store();
    if (0 == (I32)sqr[0][0])
    {
        q << "delete from se_trainingsets_result where set_id = " << get_Id();
        q.exec();

        q << "delete from se_trainingsets_attribs where set_id = " << get_Id();
        q.exec();
    }
    return S_OK;
}


long unsigned int CTrainingSet::Store() 
{
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    // Store
    conn.BeginTransaction();
    try
    {
        _Store(conn);
        
        conn.CommitTransaction();
        return S_OK;
    }
    catch(mysqlpp::Exception& ex)
    {
        conn.RollbackTransaction();
        //BOOSTER_ERROR("se::predict::CTrainingSet::Store") << "Failed to store. Reported error: " << ex.what();
        return E_DBUpdateError;
    }
    
}

long unsigned int CTrainingSet::Delete() 
{
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    // Store
    conn.BeginTransaction();
    try
    {
        _Delete(conn);
        conn.CommitTransaction();
        return S_OK;
    }
    catch(mysqlpp::Exception& ex)
    {
        conn.RollbackTransaction();
        //BOOSTER_ERROR("se::predict::CTrainingSet::Delete") << "Failed to delete. Reported error: " << ex.what();
        return E_DBUpdateError;
    }

}


