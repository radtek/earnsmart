/* 
 * File:   CEconAPI.cpp
 * Author: santony
 * 
 * Created on November 18, 2013, 10:05 AM
 */

#include "internal.h"
#include "CTrainingAPI.h"
#include "../seglobal/CROList.h"
#include "../semysql/CSEConnections.h"
#include "../seglobal/CFunctionCache.h"
#include "../sedata/CDataTable.h"
#include "../seann/fann.h"
#include "../seann/fann_cascade.h"
#include "setrainingapi.h"
#include <unordered_map>
#include <boost/tuple/detail/tuple_basic.hpp>
#include <fstream>
#include <string>
#include "../seglobal/exceptions.h"
#include "IndicatorTrainingClasses.h"
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>
#include "../seglobal/globallogger.h"

#define SEDBCONN        CSEConnections::GetMain()

using namespace se::data;

namespace se
{
    namespace training
    {
        
        Poco::Mutex CTrainingAPI::_trainingLock;

        extern "C"
        {
            
            
            SP<ITrainingAPI> pApi;
            
            ErrorId ECONEXP TrainingOpen(ISession* ps, ITrainingAPI** ppOut)
            {
                if (!pApi)
                {
                    Poco::ScopedLock<Poco::Mutex> g(CTrainingAPI::_trainingLock);
                    if (!pApi)
                    {
                        RETURNIFFAILED(CObject<CTrainingAPI>::Create( ps, &pApi));
                    }
                }
                pApi.CopyTo(ppOut);
                return S_OK;
            }
        }
        
//        
//        /*****************InflationCPIData********************/
//        bool InflationCPIData::operator !=(const InflationCPIData& r)
//        {
//            return !this->operator ==(r);
//        }
//        
//        bool InflationCPIData::operator ==(const InflationCPIData& r)
//        {
//            return this->CountryId == r.CountryId &&
//                    this->StateId == r.StateId &&
//                    this->Year == r.Year &&
//                    this->Month == r.Month;
//        }
        
        /*************************Trainingapi**********************/
        
        typedef std::vector<R4> InputNeurons;
        typedef std::vector<tuple<InputNeurons, R4>> TrainingRecords;
        typedef std::unordered_map<UID, TrainingRecords> TrainingRecordsCache;
        
        TrainingRecordsCache _trainingRecordsCache;
        
//#define MASKTRAININGID(trainingId, numrecords) trainingId << 16 | numrecords
//#define UNMASKTRAININGID(maskedvalue) (maskedvalue >> 16)
//#define UNMASKNUMRECORDS(maskedvalue) (maskedvalue & 0xFFFF)

        // function ptr for ann callback
        extern "C" void FANN_API CreateTrainDataCallback( 
                unsigned int,
                unsigned int,
                unsigned int,
                fann_type * ,
                fann_type * );

        CTrainingAPI::CTrainingAPI(ISession* ps):CObjectRoot(ps)
        {

        }

        CTrainingAPI::CTrainingAPI(const CTrainingAPI& orig):CObjectRoot(orig) 
        {

        }

        CTrainingAPI::~CTrainingAPI() 
        {

        }
        
        
        ErrorId CTrainingAPI::get_TrainingRecords(unsigned long trainingId, se::data::IStoreQueryResult** result)
        {
            try
            {
               // LOGIP("get_TrainingRecords: Query training records.");
                CSmartConnection conn(SEDBCONN);
               // LOGIP("Running query to get training data details");
                Query qTrainingData = conn->query();
                qTrainingData << "select traindata_query from se_ann_traintree where traindata_id = " << trainingId;
                StoreQueryResult rTrainingData = qTrainingData.store();
                if (!rTrainingData.size())
                {
                    stringstream str;
                    str << "No data available for training id " << trainingId;
                    throw se::seexception(str.str().c_str());
                }
                Row& rwTrainingData = rTrainingData[0];
               // LOGI(_logger, "get_TrainingRecords: Quering ... " <<  (string)rwTrainingData[0]);
                Query qTrainingQuery = conn->query((string)rwTrainingData[0]);
                //auto result1f = qTrainingQuery.store();
                ErrorId retCode = IStoreQueryResult::Create(get_Session(), qTrainingQuery.store(), result);
                if (FAILED(retCode))
                    throw se::seexception("Unable to create IStoreQueryResult.");
                
                //NOTE: BELOW IS REQUIRED WHEN EXECUTING A STORED PROCEDURE SINCE
                // PROCEDURE EXECUTE RESULTS IN A 2ND EMPTY RESULT SET.
                for (int i = 1; qTrainingQuery.more_results(); ++i)
                {
                    auto res = qTrainingQuery.store_next();
                        //int num_results = res.size();
                }                
               // LOGI(_logger, "get_TrainingRecords: Returning query results for query " <<  (string)rwTrainingData[0]);
                return S_Success;
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "get_TrainingRecords: Exception thrown: " << ex.what());
                return E_DBQueryException;
            }
            catch(...)
            {
               // LOGEP("get_TrainingRecords: Unknown exception thrown.");
                return E_UnknownError;
            }
        }
        
        
//        ErrorId CTrainingAPI::get_InflationCPI(UID countryId, UID stateId, I16 year, I16 month, IROList<InflationCPIData> **ppOut)
//        {
//           // LOGIP("Entering get_InflationCPI.");
//            try
//            {
//                const string funcName = "se.economics.get_InflationCPI";
//                if (!FC.TrySPGetFValue(funcName, countryId, stateId, year, month, *ppOut))
//                {
//                    // build the sql based on the criteria
//                    CSmartConnection conn(SEDBCONN);
//                    Query qInflation = conn->query();
//                    qInflation << "select * from se_eco_inflation_cpi ";
//                    if (countryId > 0 || stateId > 0 || year > 0 or month > 0)
//                        qInflation << " where ";
//                    if (countryId > 0)
//                        qInflation << " country_id = " << countryId;
//                    if (stateId > 0)
//                    {
//                        if (countryId > 0)
//                            qInflation << " and ";
//                        qInflation << " state_id = " << stateId;
//                    }
//                    if (year > 0)
//                    {
//                        if (countryId > 0 || stateId > 0)
//                            qInflation << " and ";
//                        qInflation << " year = " << year;
//
//                    }
//                    if (month > 0)
//                    {
//                        if (countryId > 0 || stateId > 0 || year > 0)
//                            qInflation << " and ";
//                        qInflation << " month = " << month;
//
//                    }
//                    // using default order.
//                    SP<IROList<InflationCPIData>> pList;
//                    LOGRETURNIFFAILED(CObject<CROListNonSerialized<InflationCPIData>>::Create(&pList), "Out of memory. Cannot create the list");
//                    StoreQueryResult rInflation = qInflation.store();
//                    if (!rInflation.size())
//                    {
//                       // LOGWP("No data available.");
//                       // LOGIP("Exiting get_InflationCPI successfuly.");
//                        return S_False;
//                    }
//                    for (Row& r : rInflation)
//                    {
//                        InflationCPIData data;
//                        data.CountryId = r[0];
//                        data.StateId = r[1];
//                        data.Year = r[2];
//                        data.Month = r[3];
//                        data.Value = r[4];
//                        data.Delta = r[5];
//                        ((CROListNonSerialized<InflationCPIData>*)pList._ptr)->Add(data);
//                    }
//                    FC.SPSetFValue(funcName, countryId, stateId, year, month, pList._ptr);
//                    *ppOut = pList._ptr;
//                }
//                (*ppOut)->AddRef();
//               // LOGIP("Exiting get_InflationCPI successfuly.");
//                return S_Success;
//            }
//            catch(std::exception& ex)
//            {
//               // LOGE(_logger, "Failed to run get_InflationCPI method. Reported error: " << ex.what());
//                return E_UnknownError;
//            }
//        }
        
        ErrorId CTrainingAPI::AnnTrainData(unsigned long trainingId)
        {
            try
            {
               // LOGI(_logger, "AnnTrainData: Starting ANN Traing for " << trainingId);
                // get the records from trainingtree for training input
                CSmartConnection conn(SEDBCONN);

               // LOGIP("Running query to get training data details");
                StoreQueryResult rTrainingData;
                {
                Query qTrainingData = conn->query();
                qTrainingData << "select * from se_ann_traintree where traindata_id = " << trainingId;
                rTrainingData = qTrainingData.store();
                if (!rTrainingData.size())
                {
                   // LOGI(_logger, "No data available for training id " << trainingId);
                    return E_NotFound;
                }
                }
                
                Row& rwTrainingData = rTrainingData[0];
                I16 nNumInputs = rwTrainingData["traindata_numinputs"];
                I16 nNumOutputs = rwTrainingData["traindata_numoutputs"];
                auto tinyvalue = sql_tinyint_null(rwTrainingData["traindata_iscascaded"]);
                bool bIsCascadeTraining = (tinyvalue.is_null || 0 == (I32)tinyvalue.data) ? false : true;
                
                StoreQueryResult rTrainingQuery;
                {
                Query qTrainingQuery = conn->query();
                qTrainingQuery << (string)rwTrainingData["traindata_query"];
                StoreQueryResult rTrainingQuery = qTrainingQuery.store();
                if (!rTrainingQuery.size())
                {
                   // LOGI(_logger, "No data available in training output neuron table for trainingId " << trainingId);
                    return E_NotFound;
                }
                //NOTE: BELOW IS REQUIRED WHEN EXECUTING A STORED PROCEDURE SINCE
                // PROCEDURE EXECUTE RESULTS IN A 2ND EMPTY RESULT SET.
                for (int i = 1; qTrainingQuery.more_results(); ++i)
                {
                        auto res = qTrainingQuery.store_next();
                        int num_results = res.size();
                }                
                }
                U32 totalTrainingRecords = rTrainingQuery.size();
               // LOGI(_logger, "There are " << totalTrainingRecords << " training records");
                
               // LOGIP("Creating a traingrecords.");
                
                TrainingRecords trainRecords;
                // get schema from rTrainingQuery
                auto& fields = rTrainingQuery.fields();
                for (int i = 0; i != totalTrainingRecords; ++i)
                {
                    Row& rwOutput = rTrainingQuery[i];
                    InputNeurons inputneurons;
                    for (int j=0; j != fields.size(); ++j)
                    {
                        string fieldName = fields[j].name();
                        if (fieldName != "output")
                            inputneurons.push_back(rwOutput[j]);
                    }
                    trainRecords.push_back(make_tuple(inputneurons, (R4)rwOutput["output"]));
                }
                
                _trainingRecordsCache[trainingId] = trainRecords;
                
               // LOGIP("Creating ANN training data structure using callback");
                struct fann_train_data* anntrainingData = fann_create_train_from_callback(totalTrainingRecords, nNumInputs, nNumOutputs, CreateTrainDataCallback);
                auto inputCount = fann_num_input_train_data(anntrainingData);
                auto outputCount = fann_num_output_train_data(anntrainingData);
                
               // LOGIP("Completed populating trainingg records with input and output neurons");
                struct fann* ann ;
                if (bIsCascadeTraining)
                {
                   // LOGIP("Creating ANN cascasded trainer using shortcut");
                    ann = fann_create_shortcut(2, nNumInputs, nNumOutputs);
                }
                else
                {
                   // LOGIP("Creating ANN standard trainer");
                    U32 numLayers = rwTrainingData["traindata_numlayers"];
                    U32 numHidLayers = rwTrainingData["traindata_numhiddenlayers"];
                    U32 arrayparam[3];
                    arrayparam[0] = inputCount;
                    arrayparam[1] = numHidLayers;
                    arrayparam[2] = outputCount;
                    ann = fann_create_standard_array(numLayers, arrayparam);
                }                
                fann_set_train_stop_function(ann, FANN_STOPFUNC_MSE);
                fann_set_activation_function_hidden(ann, FANN_ELLIOT_SYMMETRIC);
                fann_set_activation_function_output(ann, FANN_GAUSSIAN_SYMMETRIC);
                fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);
                fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);

                fann_set_quickprop_decay(ann, -0.0001);
                fann_set_quickprop_mu(ann, 1.75);
                fann_set_rprop_increase_factor(ann, 1.2);
                fann_set_rprop_decrease_factor(ann, 0.5);
                fann_set_rprop_delta_min(ann, 0.0);
                fann_set_rprop_delta_max(ann, 50);
                fann_set_learning_rate(ann, 0.7);
                
                //if (bIsCascadeTraining)
                {
                    fann_set_cascade_output_change_fraction(ann, 0.01);
                    fann_set_cascade_candidate_stagnation_epochs(ann, 12);
                    fann_set_cascade_output_change_fraction(ann, 0.01);
                    fann_set_cascade_candidate_change_fraction(ann, 0.01);

                    fann_set_cascade_weight_multiplier(ann, 0.4);
                    fann_set_cascade_candidate_limit(ann, 1000);
                    fann_set_cascade_max_out_epochs(ann, 150);
                    fann_set_cascade_max_cand_epochs(ann, 150);
                    fann_set_cascade_num_candidate_groups(ann, 2);
                }
                
               // LOGIP("Attempting to ANN train data.");
                SP<IString> pstrDate;
                DATETIME::Now().ToString(&pstrDate);
               // LOGI(_logger,  "Traing started at : " << pstrDate->get_Buffer())
                if (bIsCascadeTraining)
                {
                    fann_cascadetrain_on_data(ann, anntrainingData, 50000, 100, 0.00010);
                }
                else
                {
                    fann_train_on_data(ann, anntrainingData, 500000, 1000, 0.00010);
                }
                
                DATETIME::Now().ToString(&pstrDate);
               // LOGI(_logger,  "Training ended at : " << pstrDate->get_Buffer());

                stringstream strFileName ;
                strFileName << "trainingdata_" << trainingId;
               // LOGI(_logger, "Save cascasded training result in a file " << strFileName.str());
                fann_save(ann, strFileName.str().c_str());
                
               // LOGIP("Destroying ANN training data structure and ann data structure");
                fann_destroy_train(anntrainingData);
                fann_destroy(ann);
                
               // LOGIP("finally remove from cache");
                _trainingRecordsCache.erase(trainingId);
                
               // LOGIP("Persist trained data in database from file.");
                std::ifstream trainingDataFile(strFileName.str());
                std::string fileContent(
                        (std::istreambuf_iterator<std::ifstream::char_type>(trainingDataFile)), 
                        (std::istreambuf_iterator<std::ifstream::char_type>() ));
                
                trainingDataFile.close();

                
                {
                Query qUpdateTraingTree = conn->query();
                qUpdateTraingTree << "update se_ann_traintree set traindata_result = " 
                        << quote << fileContent << " where traindata_id = " << trainingId;
                qUpdateTraingTree.execute();
               // LOGIP("Saved training data in the database.");
                }
                
               // LOGI(_logger, "AnnTrainData: Completed ANN Traing for " << trainingId);
                return S_Success;
            }
            catch(std::exception& ex)
            {
               // LOGE(_logger, "AnnTrainData: Failed for unknown reason. " << ex.what());
                return E_UnknownError;
            }
        }

        ErrorId CTrainingAPI::AnnPredictResult(UID trainingId, ANNINPUTCALLBACK inputCallback, ANNOUTPUTCALLBACK outputCallback)
        {
            try
            {
               // LOGIP("AnnPredictResult : Starting function.");
                // invoke input callback
                CSmartConnection conn(SEDBCONN);
                
               // LOGIP("AnnPredictResult: Running query to get training data details");
                StoreQueryResult rTrainingData ;
                {
                Query qTrainingData = conn->query();
                qTrainingData << "select * from se_ann_traintree where traindata_id = " << trainingId;
                rTrainingData = qTrainingData.store();
                if (!rTrainingData.size())
                {
                   // LOGI(_logger, "No data available for training id " << trainingId);
                    return E_NotFound;
                }
                }
                
                Row& rwTrainingData = rTrainingData[0];
                I16 nNumInputs = rwTrainingData["traindata_numinputs"];
                I16 nNumOutputs = rwTrainingData["traindata_numoutputs"];
                string strTrainedData = (string)rwTrainingData["traindata_result"];
                
               // LOGIP("AnnPredictResult: Saving trained data to a temporary file.");
                stringstream fileName ;
                fileName << "trainingdata_" << trainingId;
                std::ofstream file(fileName.str());
                file.write(strTrainedData.c_str(), strTrainedData.size());
                file.flush();
                file.close();
                
               // LOGIP("AnnPredictResult: Creating an ANN from the file.");
                struct fann* ann = fann_create_from_file(fileName.str().c_str());
                
               // LOGIP("AnnPredictResult: Requesting input values");
                R4 inputs[nNumInputs];
                R4* outputs;
                for (int i=0; i != nNumInputs; ++i)
                {
                    inputs[i] = inputCallback(i);
                }
                
               // LOGIP("AnnPredictResult: Running FANN training to get predicted results.");
                outputs = fann_run(ann, inputs);
                
               // LOGI(_logger, "Predicted values for inputs [");
                for (int i=0; i != nNumInputs; ++i)
                {
                   // LOGI(_logger, " " << inputs[i] << " ");
                }
               // LOGI(_logger, "] is " << outputs[0]);
                
               // LOGIP("AnnPredictResult: Calling the callback to return output.");
                outputCallback(nNumOutputs, outputs);
                
                fann_destroy(ann);
               // LOGIP("AnnPredictResult: Successfully completed function.");
                return S_Success;
            }
            catch(std::exception& ex)
            {
               // LOGE(_logger, "AnnPredictResult: Failed for unknown reason. " << ex.what());
                return E_UnknownError;
            }
        }
        
        
        /*****************************native c callback for fann***********************/
        
        extern "C" void FANN_API CreateTrainDataCallback(unsigned int recordIndex, unsigned int numOfInputs, unsigned int numOfoutputs, fann_type* inputs, fann_type* output)
        {
            auto& record = _trainingRecordsCache[100][recordIndex];
            inputs = get<0>(record).data();
            output = &get<1>(record);
//            R4 test1, test2;
//            test1 = inputs[3];
//            test2 = *output;
//            cout << test1 << test2;
        }
        
        
        //////////////////////////////////Static Create/////////////////////////////
        

        extern "C"
        {
            SE_CODE ECONEXP CreateTrainingTable(ISession* ps, PERIOD datafreq, PERIOD requiredDataFreq, ITrainingTable** ppOut)
            {
                SP<CTrainingTable> ptable;
                RETURNIFFAILED(CObject<CTrainingTable>::Create(ps, &ptable));
                ptable->set_DataFrequency(datafreq);
                ptable->set_Property("RequiredFreq", (I32)requiredDataFreq);
                ptable.CopyTo(ppOut);
                return S_OK;
            }

            SE_CODE ECONEXP CreateExTrainingTable(ISession* ps, IExTrainingTable** ppOut)
            {
                return CObject<CExTrainingTable>::Create(ps, ppOut);
            }
            
            SE_CODE ECONEXP CreateTrainingTableRecord(ISession* ps, ITrainingTable* table, DATAPERIOD dp, OmniType value, ITrainingRecord** ppOut)
            {
                return CObject<CTrainingRecord>::Create(ps, (CTrainingTable*)table, dp, value, ppOut);
            }

            SE_CODE ECONEXP CreateExTrainingTableRecord(ISession* ps, IExTrainingTable* table, IExTrainingRecord** ppOut)
            {
                return CObject<CExTrainingRecord>::Create(ps, (CExTrainingTable*)table, ppOut);
            }
            
            
        }        
    }
}