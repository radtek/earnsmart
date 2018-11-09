/* 
 * File:   CPredictor.cpp
 * Author: santony
 * 
 * Created on May 9, 2015, 11:53 AM
 */


#include "sepredictinternal.h"
#include "CPredictor.h"
#include "CTrainingSet.h"
#include "CProductDescription.h"
#include "CXXXAlgoProcessBase.h"
#include "../seglobal/globallogger.h"

using namespace se::predict;

CPredictor::CPredictor(ISession* ps) : CObjectRoot(ps) 
{
}

CPredictor::~CPredictor() 
{
}

long unsigned int CPredictor::Init(IProductDescription* pdesc) 
{
    _prodDesc = pdesc;
    return Load();
}


const char* CPredictor::get_Name() 
{
    return _sName.c_str();
}

void CPredictor::set_Name(const char* name) 
{
    _sName = name;
}

const char* CPredictor::get_Description() {
    return _sDesc.c_str();
}

void CPredictor::set_Description(const char* desc) 
{
    _sDesc = desc;
}

IPredictor& CPredictor::SetProduct(IProductDescription* prodDesc)  throw(std::exception&)
{
    _prodDesc = prodDesc;
    THROWIFFAILED(Load(), "Error loading prediction information.");
    return *this;
}

const DATAPERIOD& CPredictor::get_StartPeriod() 
{
    return _dpStart;
}

void CPredictor::set_StartPeriod(const DATAPERIOD& dp) 
{
    _dpStart = dp;
}

const DATAPERIOD& CPredictor::get_EndPeriod() 
{
    return _dpEnd;
}

void CPredictor::set_EndPeriod(const DATAPERIOD& dp) 
{
    _dpEnd = dp;
}


IPredictor& CPredictor::SetTrainingSet(ITrainingSet* tset) throw(std::exception&) 
{
    _tset = tset;
    return *this;
}


IPredictor& CPredictor::BuildEquation() throw(std::exception&)
{
    if (_dpStart.IsEmpty())
        throw seexception(E_IncorrectOperation, "Starting period is not provided.");
    
    if (_dpEnd.IsEmpty())
    {
        _dpEnd = CDate::Now().ToPeriodForm();
        _dpEnd.Day = 1;
    }

    this->Load();
    // RUN THE TRAINING TABLES INTO THE CHOSEN ALGO ANALYSIS
    // SET RESULT INSTANCE.

    // HOW DOES THIS FUNCTION WORK ?
    /*
     * 1.   Training set may be hierarchical and each of them have their
     *      starting period, ending period received from predictor. Besides
     *      they have their own algo settings. If the ending period is set to future
     *      they are considered to be predicted value.
     * 2.   We first need to go to the most lowest level training sets in 
     *      dependency collection. 
     * 3.   Get the training table
     * 4.   Pass it to the respective ALGO processor class. This processor will
     *      continuously calculate and set the future values
     * 5.   After this training table is completed, we go to next dependent and do as
     *      step 4.
     * 6.   Now the result columns from the dependent levels are added to the parent's
     *      training set table, and follow the step 4 to get the result column for 
     *      future periods. 
     * 7.   This is repeated until we reach top level training set.
     */

    
    
    SP<CXXXAlgoProcessBase> algoProcesor = 
            CXXXAlgoProcessBase::Construct(this, _tset); // pass the top training set.
    
    _presult = algoProcesor->Run();
    
    return *this;
}


IResult& CPredictor::Result()  throw(std::exception&)
{
    if (!_presult)
        BuildEquation();
    return *_presult;
}

SE_CODE CPredictor::Load() 
{

    if (_prodDesc && _tset)
        return S_DataExists; // previously loaded.

    if (!_prodDesc)
        throw se::seexception(E_AlgorithmError, "No product desc specified");

    
    
    // store pkey field in set_InternalId(x)
    
    /*
    Using below properties, 
     * _prodDesc->get_Id();
     * _prodDesc->get_Type();
       _prodDesc->get_Context();
     * go to se_trainingset_contexts table and 
     * load stuff
     */    
        // get the top most tset pk-id
        /*
         * `se_tset_context_product` (
            `pkey` int(11) NOT NULL AUTO_INCREMENT,
            `productdesctype_id` int(11) NOT NULL COMMENT 'This field is an enum declared in the library se.predict.ProductDescriptionType',
            `product_id` bigint(20) NOT NULL,
            `tset_id` int(11) NOT NULL,
            `context_id` char(1) NOT NULL,
            PRIMARY KEY (`pkey`),
         */
    
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    try
    {
        Query q = conn->query();
        q << "select pkey, name, `desc`, tset_id from se_tset_context_product where " ;
        q << "productdesctype_id=" << (int)_prodDesc->get_Type();
        q << " and product_id=" << _prodDesc->get_Id();
        q << " and context_id=" << quote << (CHR)_prodDesc->get_Context();
        //BOOSTER_INFO("se::predict::CPredictor::Load") << q.str();
        auto sqr = q.store();
        if (sqr.size())
        {
            set_Id(sqr[0]["pkey"]);
            _sName = (CSTR)sqr[0]["name"];
            _sDesc = (CSTR)sqr[0]["desc"];
            UID tsetId = sqr[0]["tset_id"];
            auto r = CTrainingSet::Get(get_Session(), conn, get_Id(), tsetId, &_tset);
            if (FAILED(r))
                throw seexception(r, "Failed to get trainingset instance using stored information.");
            
            return S_DataExists;
        }
        return S_OK;
    }
    catch(std::exception& ex)
    {
        //BOOSTER_ERROR("se::predict::CPredictor::Load") << ex.what();
        throw seexception(E_IncorrectOperation, ex.what());
    }
}


long unsigned int CPredictor::Store() 
{
    // atleast name field is necessary 
    if (_sName.empty())
    {
        //BOOSTER_ERROR("se::predict::CPredictor::Store") << "A name must be provided for the predictor for future identification.";
        return E_IncorrectOperation;
    }    
    
    if (!this->_tset)
    {
        //BOOSTER_ERROR("se::predict::CPredictor::Store") << "Training set is not defined.";
        return E_IncorrectOperation;
    }
    
    if (_sDesc.empty())
        _sDesc = _sName;

    using namespace se::data;

    CSmartConnection conn(CSEConnections::GetMain());
    // Store
    conn.BeginTransaction();
    try
    {
        /*
         * `se_tset_context_product` (
            `pkey` int(11) NOT NULL AUTO_INCREMENT,
            `productdesctype_id` int(11) NOT NULL COMMENT 'This field is an enum declared in the library se.predict.ProductDescriptionType',
            `product_id` bigint(20) NOT NULL,
            `tset_id` int(11) NOT NULL,
            `context_id` char(1) NOT NULL,
            PRIMARY KEY (`pkey`),
         */
        // save in se_tset_context_product all the predictor fields
        // we also need the new primary key (if an insert) for saving training tree.
        Query q = conn->query();
        if (!get_Id())
        {
            q << "select * from se_tset_context_product where ";
            q << "productdesctype_id=" << (int)_prodDesc->get_Type();
            q << " and product_id=" << _prodDesc->get_Id();
            q << " and context_id=" << quote << (CHR) _prodDesc->get_Context();
        }
        else if (get_Id())
        {
            q << "select * from se_tset_context_product where ";
            q << "pkey=" << get_Id();
        }
        auto sqr = q.store();
        if (!sqr.size())
        {
                
            q = conn->query();
            q << "insert into se_tset_context_product ";
            q << "(name, `desc`, productdesctype_id, product_id, tset_id, context_id) ";
            q << "values (";
            q << quote << _sName << ", ";
            q << quote << _sDesc << ", ";
            q << (int) _prodDesc->get_Type() << ", ";
            q << _prodDesc->get_Id() << ", ";
            q << _tset->get_Id() << ", ";
            q << quote << (CHR)_prodDesc->get_Context();
            q << ")";
            //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
            q.exec();
            set_Id(q.insert_id());
        }
        else
        {
            set_Id(sqr[0]["pkey"]);
            q << "update se_tset_context_product set ";
            q << " name = " << quote << _sName << ", ";
            q << " `desc` = " << quote << _sDesc ;
            q << " where pkey = " << get_Id();
            q.exec();
        }
    
        // then store the tset hierarchy in se_trainingsets_tree table
        /*
            `se_trainingsets_tree` (
             `set_id` int(11) NOT NULL,
             `parent_set_id` int(11) NOT NULL,
             `tcp_pkey` int(11) NOT NULL,
             PRIMARY KEY (`set_id`,`parent_set_id`,`tcp_pkey`)        
        
         * 
         */
        q = conn->query();
        q << "delete from se_trainingsets_tree " ;
        q << " where tcp_pkey = " << get_Id();
        //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
        q.exec();

        // save each training set in se_trainingsets in the tset hierarchy, 
            // attribs in se_trainingsets_attribs
            // result type in se_trainingsets_result
        
        /*
         *  `se_trainingsets` (
            `set_id` int(11) NOT NULL AUTO_INCREMENT,
            `set_name` varchar(1048) NOT NULL,
            `set_desc` varchar(2048) DEFAULT NULL,
            `set_algo` varchar(45) DEFAULT 'mvreg' COMMENT 'mvreg for multivariate regression analysis\nann for artificial neural network',
            `set_default` char(1) DEFAULT '0',
            PRIMARY KEY (`set_id`)
            `se_trainingsets_attribs` (
             `set_id` int(11) NOT NULL,
             `attrib_id` int(11) NOT NULL,
             `source` char(1) NOT NULL COMMENT '''E'' for economic indicator, ''P'' for performance ratios.',
             PRIMARY KEY (`set_id`,`attrib_id`,`source`)         
            `se_trainingsets_result` (
             `set_id` int(11) NOT NULL,
             `result_id` int(11) NOT NULL DEFAULT '0',
             `source` char(1) NOT NULL DEFAULT 'P' COMMENT 'source = ''E'' economic indicator ; ''P'' performance ratio',
             PRIMARY KEY (`set_id`,`result_id`,`source`)         
         */
        
        THROWIFFAILED(((CTrainingSet*)this->_tset._ptr)->_Store(conn), "Failed to store training sets");

        function<void(ITrainingSet*)> rfunc ;
        rfunc = [this, &rfunc, &conn](ITrainingSet* ptset)->void
        {
            if (ptset->get_Id()) // delete and insert again.
            {
                Query q = conn->query();
                q << "delete from se_trainingsets_tree " ;
                q << " where parent_set_id = " << ptset->get_Id();
                //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
                q.exec();
            }
            if (ptset->get_Dependents())
            {
                SP<ITrainingSetList> ptl = ptset->get_Dependents();
                for (ITrainingSet* pdep : *ptl)
                {
                    // save training and its dependents
                    Query q = conn->query();
                    q << "insert into se_trainingsets_tree values (";
                    q << pdep->get_Id() << ", ";
                    q << ptset->get_Id() << ", ";
                    q << get_Id();
                    q << ")";
                    // recurse for the dependent.
                    //BOOSTER_INFO("se::predict::CPredictor::Store") << q.str();
                    q.exec();
                    rfunc(pdep);
                }
            }
        };
        
        rfunc(this->_tset);
        
        // save topmost tset in context table.
        q = conn->query();
        q << "update se_tset_context_product set tset_id=" << _tset->get_Id() ;
        q << " where pkey = " << get_Id();
        q.exec();
        
        conn.CommitTransaction();
        //conn.RollbackTransaction();   // for testing purpose only.
        return S_OK;
    }
    catch(mysqlpp::Exception& ex)
    {
        conn.RollbackTransaction();
        //BOOSTER_ERROR("se::predict::CPredictor::Store") << ex.what();
        return E_DBUpdateError;
    }
}

long unsigned int CPredictor::Remove() 
{
    //NOTE: While removing records, ensure to check if a trainingid is used by another prediction algo
    // as well as int he training tree by another training set parent.
    
    if (!_tset || !_prodDesc)
    {
        //BOOSTER_ERROR("se::predict::CPredictor::Remove") << "Product description or training set not loaded previously or not given.";
        return E_IncorrectOperation;
    }
    
    if (!get_Id())
    {
        //BOOSTER_ERROR("se::predict::CPredictor::Remove") << "Need to store it first before you can remove from database. Otherwise simply discard the predictor instance";
        return E_IncorrectOperation;
    }
    using namespace se::data;
    CSmartConnection conn(CSEConnections::GetMain());
    // Store
    conn.BeginTransaction();
    try
    {
        Query q = conn->query();
        q << "delete from se_tset_context_product where pkey="<<get_Id();
        q.exec();
        
        q = conn->query();
        q << "Delete from se_trainingsets_tree where tcp_pkey=" << get_Id();
        q.exec();
        
        set_Id(0);
        this->_presult.Clear();
        this->_prodDesc.Clear();
        this->_tset.Clear();
        
        conn.CommitTransaction();
        return S_OK;
    }
    catch(mysqlpp::Exception& ex)
    {
        conn.RollbackTransaction();
        //BOOSTER_ERROR("se::predict::CPredictor::Remove") << "Failed to remove prediction setup. Reported error: " << ex.what();
        return E_DBUpdateError;
    }
    
}


void CPredictor::Clear() 
{
    _presult = nullptr;
}

void CPredictor::Serialize(ISerializedData<IString>* pIn) {

}
