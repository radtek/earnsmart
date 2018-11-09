/* 
 * File:   CEconomicIndicatorFeeder.h
 * Author: santony
 *
 * Created on June 19, 2014, 12:24 AM
 */

#ifndef CECONOMICINDICATORFEEDER_H
#define	CECONOMICINDICATORFEEDER_H

using namespace se::data;

namespace se
{
    
    namespace economy
    {

        class CEconomicIndicatorFeeder 
        {
        public:
            CEconomicIndicatorFeeder();
            CEconomicIndicatorFeeder(const CEconomicIndicatorFeeder& orig);
            virtual ~CEconomicIndicatorFeeder();
            
            SE_CODE Run();
            SE_CODE Run(UID eid);
            
        private:
            //LoggerPtr _logger;
        };
        
    }
    
}

#endif	/* CECONOMICINDICATORFEEDER_H */

