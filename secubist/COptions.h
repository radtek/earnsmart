/* 
 * File:   COptions.h
 * Author: santony
 *
 * Created on December 3, 2013, 5:45 PM
 */

#ifndef COPTIONS_H
#define	COPTIONS_H

#include "../seglobaltypedefs.h"

namespace se
{
    namespace math
    {
        /*
         * 
-u	generate unbiased rules
-i	use instances and rules (composite models)
-a	allow the use of instances and rules
-n neighbors	set the number of nearest neighbors (1 to 9)
-C members	construct a committee model
-S percent	use the sampling option
-I seed	set the sampling seed value
-X folds	carry out a cross-validation
-r rules	set the maximum number of rules
-e percent	set the extrapolation limit

* There are several options that control the format of the output:

-f filestem	identify the application (required)
-p	show the saved model
-e	show estimated error bounds for each prediction in the form +-E
-i	for composite models, show each nearest neighbor and its distance from the case

         * 
         *          */
        
        
        typedef enum enOptions
        {
            OPT_None                   =       0x0,
            OPT_AllowUnbiasedRules      =       0x1,
            OPT_UseRulesOnly            =       0x2,
            OPT_UseRulesAndInstances    =       0x4,
            OPT_RulesInstanceAuto       =       0x8,
            //OPT_UseNumberOfNeighbours   =       0xF,
            //OPT_UseCommittee            =       0x20,
        } OPTIONS, OPT;
        
        typedef enum enOutputOptions
        {
            OOPT_ShowEstimatedErrorBound        =       0x1,
            OOPT_ShowNearestNeighborDistance    =       0x2
        } OUTOPTIONS, OOPT;
        
        class CCubist;
        
        class COptions 
        {
        public:
            COptions(CCubist* cube);
            COptions(const COptions& orig);
            virtual ~COptions();

            void set_Option(OPT option);
            void unset_Option(OPT option);
            bool get_IsOptionSet(OPT option);
            
            void set_NumberOfNearestNeighbors(I32 num); // 1-9 range
            I32 get_NumberOfNearestNeighbors();

            void set_NumberOfCommittees(I32 num); 
            I32 get_NumberOfCommittees();
            
            void set_MaximumNumOfRules(I32 num); // 1-1000000 range
            I32 get_MaximumNumOfRules();

            void set_ExtrapolationLimit(R8 num); // 1-1000000 range
            R8 get_ExtrapolationLimit();
            
            void set_Sampling(R8 num);
            R8 get_Sampling();

            void set_RandomSeed(I32 num);
            I32 get_RandomSeed();

            void set_CrossValidationFolds(I32 num);
            I32 get_CrossValidationFolds();
            
#ifdef VerbOpt
            void set_Verbosity(I32 num);
            I32 get_Verbosity();
#endif
            
        private:
            CCubist* pCube;
            I64 _options ;
        };
    }
}
#endif	/* COPTIONS_H */

