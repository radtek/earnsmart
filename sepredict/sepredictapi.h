/* 
 * File:   sepredictapi.h
 * Author: santony
 *
 * Created on May 9, 2015, 3:30 PM
 */

#ifndef SEPREDICTAPI_H
#define	SEPREDICTAPI_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../seglobalenums.h"
#include "../setraining/setrainingapi.h"

#define PREDICTEXP __attribute__ ((visibility ("default")))

namespace se
{
    namespace predict
    {
        enum class PredictionAlgos
        {
            MultiVarLinearRegression = 1,
            ArtificialNeuralNetwork = 2
        };
        
        enum class TrainingDataContext : char
        {
            Custom = 'C',           // training set is built using custom rules
            EconomicIndicator = 'E',    // training is done against an economic indicator.
            SectorPerformance = 'T', 
            IndustryPerformance = 'I',
            FirmPerformance = 'P',  // training done for firm's performance
            SecurityPerformance = 'S'   // training done for security performance
        };
        
        
        struct IResult : public IObject
        {
            virtual OmniType& Value() = 0;
        };

        // Describes the product used to analyse a future value
        // Eg. when a publicly listed company's netcome is 
        // analysed, the predictor will first see if a record
        // exists for its firm id/firm combination. If not
        // firm's industry SIC/Industry combination is checked.
        // if not, its SECTOR GICS-SIC/Sector is checked.
        // if not, General is used.
        // Economic indicator and security are exclusively used
        // for an economic indicator like gdp and 
        // for a stock, option, future, currency, etc value respectively.
        enum class ProductDescriptionType
        {
            General = 0,  // prediction is done irrespective of a specific target.
            EconomicIndicator = 1,
            Sector = 11,
            Industry = 12,
            Firm = 13,
            Security = 21
        };
        
        /* Define a product including its type.
         * get_Id and set_Id will be used to set
         * the product pkey identifier
         */
        struct IProductDescription : public IObject
        {
            virtual void set_Type(ProductDescriptionType type) = 0;
            virtual ProductDescriptionType get_Type() = 0;
            virtual TrainingDataContext get_Context() = 0;
            virtual void set_Context(TrainingDataContext) = 0;
            static SE_CODE PREDICTEXP Create(ISession*, UID id, ProductDescriptionType type, TrainingDataContext ctxt, IProductDescription** ppOut);
        };
        
        struct IPeriodRange 
        {
            virtual const DATAPERIOD& get_StartPeriod() = 0;
            virtual void set_StartPeriod(const DATAPERIOD& dp) = 0;
            virtual const DATAPERIOD& get_EndPeriod() = 0;
            virtual void set_EndPeriod(const DATAPERIOD& dp) = 0;
        };
        
        // a set of econometrics used in modeling data.
        enum class TimeSeriesMetricType 
        {
            Standard,
            /*
            ARCH,
            GARCH,
            NGARCH,
            IGARCH,
            EGARCH,
            GARCH_M,
            OGARCH,
            GJR_GARCH,
            TGARCH,
            fGARCH,
            COGARCH
            */
        };
        
        struct IAttributeDescriptionBase : public IObject, public IPeriodRange, public ISerialization
        {
            enum QUERYFLDS
            {
                TRContext,  // Training data context type.
                NameLike,
                Metric,
            };
            
            virtual const TimeSeriesMetricType& get_Metric() = 0;
            virtual void set_Metric(const TimeSeriesMetricType& metric) = 0;
            virtual TrainingDataContext get_Context() = 0;
            static SE_CODE PREDICTEXP BuildTimeSeries(ISession* ps, IAttributeDescriptionBase* attribDesc, se::training::ITrainingTable** ppOut);
        };
        
        struct IAttributeDescription : public IAttributeDescriptionBase
        {
            static SE_CODE PREDICTEXP Create(ISession*, U32 uid, TrainingDataContext ctxt, IAttributeDescription**);
        };
        
        struct IResultAttribDescription : public IAttributeDescriptionBase
        {
            static SE_CODE PREDICTEXP Create(ISession*, U32 uid, TrainingDataContext ctxt, IResultAttribDescription**);
        };
        
        
        struct ITrainingSet ;
        struct ITrainingSetList : ISPList<ITrainingSet>
        {
            
        };
        
        struct IAttributeDescriptionList : ISPList<IAttributeDescriptionBase>
        {
            virtual SE_CODE Filter(TrainingDataContext context, IAttributeDescriptionList* ppOut) = 0;
        };
        
        struct ITrainingSet : public ISDObject, public IObject, public ISerialization, public IPeriodRange
        {
            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR) = 0;
            virtual CSTR get_Desc() = 0;
            virtual void set_Desc(CSTR) = 0;
            virtual void set_Algo(PredictionAlgos algo) = 0;
            virtual PredictionAlgos get_Algo() = 0;
            virtual void add_Attribute(IAttributeDescription*) = 0;
            virtual void rem_Attribute(UID attribId) = 0;
            virtual IAttributeDescriptionList* get_Attributes() = 0;
            virtual void set_CaseWeightAttribute(IAttributeDescription*)=0;
            virtual IAttributeDescription* get_CaseWeightAttribute()=0;
            virtual void set_ResultContext(IResultAttribDescription*) = 0;
            virtual IResultAttribDescription* get_ResultContext() = 0;
            
            virtual void AddDependent(ITrainingSet* tsetChild) = 0;
            virtual void RemoveDependent(I32 tsetId) = 0;
            virtual ITrainingSetList* get_Dependents() throw(std::exception&)= 0;
            
            /* Load training table from attribute and result sources. set refresh to
             true if cached table is cleared and fresh one needs to be loaded.
             * Note: set_StartPeriod set_EndPeriod should be used to 
             * set start and end periods before making this call.
             */
            virtual se::training::IExTrainingTable* get_Table(bool refresh = false) = 0;
        
            
            static SE_CODE PREDICTEXP CreateorOpen(ISession*, CSTR name, ITrainingSet** ppOut);
            static SE_CODE PREDICTEXP CreateorOpen(ISession*, UID tsetid, ITrainingSet** ppOut);
            static SE_CODE PREDICTEXP Get(ISession*, UID predictorId, I32 tsetid, ITrainingSet**) throw(std::exception&);
        };
        
        /*
         * Setting up a predictor:
         * A predictor may be set up for an economic data indicator,
         * firm, sector or industry or for a security price. 
         * It is called a context. A context specifies what result you're expecting such
         * as revenue or security price. 
         * A product is a primary key of a record we have
         * in our system such as a firm id, economi indicator id,
         * or security id (stock, future, currency, etc.). The trainset
         * table (which may be optionally hierarchical is constructed
         * from various input time series data points. In a hierarchical
         * trainingset, one training set may depend on the result of
         * one or more trainingsets' results. We also specify
         * what algorithm to use before running the predicting
         * process against the training set.
         * 
         */
        struct IPredictor : public IIdName, public IPeriodRange
        {
            // Get or Set a description
            virtual CSTR get_Description() = 0;
            virtual void set_Description(CSTR desc) = 0;
            // set training set. This is used only when a new prediction rulesset is built.
            // for existing training, BuildEquation can be called by utilizing
            // algo, product and context provided the info is in the database.
            virtual IPredictor& SetTrainingSet(ITrainingSet* tset) throw(std::exception&) = 0;
            /// Build the equation using the algorithm and data. Throws std exception
            /// Note: set_StartPeriod set_EndPeriod should be used to 
            /// set start and end periods before making this call.
            virtual IPredictor& BuildEquation() throw(std::exception&) = 0 ;
            virtual IResult& Result() throw(std::exception&) = 0;
            // Save predictor information.
            virtual SE_CODE Store() = 0;
            // Remove this prediction
            virtual SE_CODE Remove() = 0;
            // Retry by clearing all internal variables
            virtual void Clear() = 0;
        };
        
        
        extern "C"
        {
            
            /* Open and obtain a reference to the api gateway */
            SE_CODE PREDICTEXP Create(ISession*, IProductDescription*, IPredictor** ppGateway) ;
            
            // Obtain a list of all attributes in smartearn system.
            SE_CODE PREDICTEXP ListAttributes(ISession*, IAttributeDescriptionList** ppOut, QUERYFUNC(IAttributeDescriptionBase) filter = nullptr);
        }
    }
}


#endif	/* SEPREDICTAPI_H */

