/* 
 * File:   COptions.cpp
 * Author: santony
 * 
 * Created on December 3, 2013, 5:45 PM
 */

#include "internal.h"
#include "COptions.h"

namespace se
{
    namespace intel
    {
        COptions::COptions() : _options(OPT_None)
        {
        }

        COptions::COptions(const COptions& orig) 
        {
            _options = orig._options;
        }

        COptions::~COptions() 
        {
        }

        void COptions::set_Option(OPT option)
        {
            _options |= (I64) option;
            
            if (OPT_UseRulesOnly == (_options & OPT_UseRulesOnly))
            {
                CHOOSEMODE = false;
                USEINSTANCES = false;
            }
            else if (OPT_UseRulesAndInstances == (_options & OPT_UseRulesAndInstances))
            {
                CHOOSEMODE = false;
                USEINSTANCES = true;
            }
            else if (OPT_RulesInstanceAuto == (_options & OPT_RulesInstanceAuto))
            {
                CHOOSEMODE = true;
                USEINSTANCES = true;
            }
            if (OPT_AllowUnbiasedRules == (_options & OPT_AllowUnbiasedRules))
            {
                UNBIASED = true;
            }
        }
        
        void COptions::unset_Option(OPT option)
        {
            _options &= ~(I64)option;
//            if (OPT_UseNumberOfNeighbours == option)
//                _numNeighbors = 0;
//            if (OPT_UseCommittee == option)
//                _numCommitees = 0;
        }
        
#ifdef VerbOpt
        I32 COptions::get_Verbosity()
        {
            return VERBOSITY;
        }
        
        void COptions::set_Verbosity(signed int num)
        {
            VERBOSITY = num;
        }
#endif        
        bool COptions::get_IsOptionSet(OPT option)
        {
            return option == (_options & (I64)option);
        }
        
        void COptions::set_NumberOfNearestNeighbors(signed int num)
        {
            if (num > NNMAX || num < 1)
                throw seexception("Permitted range for maximum number of rules : 1-9");
            NN = num;
        }
        
        I32 COptions::get_NumberOfNearestNeighbors()
        {
            return NN;
        }
        
        void COptions::set_Sampling(R8 num)
        {
            if (num > 99.9 || num < 0.1)
                throw seexception("Permitted range for maximum number of rules : 0.1-99.9");
            SAMPLE = num / 100;
        }
        
        R8 COptions::get_Sampling()
        {
            return SAMPLE * 100;
        }
        
        
        void COptions::set_RandomSeed(I32 num)
        {
            KRInit = num & 07777;
        }
        
        I32 COptions::get_RandomSeed()
        {
            return KRInit;
        }       
        
        void COptions::set_NumberOfCommittees(signed int num)
        {
            if (num > 100 || num < 1)
                throw seexception("Permitted range for maximum number of rules : 1-100");

            MEMBERS = num;
        }
        
        I32 COptions::get_NumberOfCommittees()
        {
            return MEMBERS;
        }
        
        void COptions::set_MaximumNumOfRules(signed int num)
        {
            if (num > 1000000 || num < 1)
                throw seexception("Permitted range for maximum number of rules : 1-1000000");

            MAXRULES = num;
        }
        
        I32 COptions::get_MaximumNumOfRules()
        {
            return MAXRULES;
        }
        
        void COptions::set_ExtrapolationLimit(R8 num)
        {
            if (num > 100.0 || num < 0.0)
                throw seexception("Permitted range for maximum number of rules : 0-100");
            
            EXTRAP = num/100;
        }
        
        R8 COptions::get_ExtrapolationLimit()
        {
            return EXTRAP * 100;
        }
        
        void COptions::set_CrossValidationFolds(I32 num)
        {
            if (num > 1000 || num < 1)
                throw seexception("Permitted range for maximum number of rules : 1-1000");
            FOLDS = num;
        }
        
        I32 COptions::get_CrossValidationFolds()
        {
            return FOLDS;
        }
        
    }
}
