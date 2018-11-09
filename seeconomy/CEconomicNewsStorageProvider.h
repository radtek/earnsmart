/* 
 * File:   CEconomicNewsStorageProvider.h
 * Author: santony
 *
 * Created on April 10, 2014, 6:29 PM
 */

#ifndef CECONOMICNEWSSTORAGEPROVIDER_H
#define	CECONOMICNEWSSTORAGEPROVIDER_H

namespace se
{
    namespace economy
    {

        class CEconomicNewsStorageProvider 
        {
        public:
            CEconomicNewsStorageProvider(ISession*);
            virtual ~CEconomicNewsStorageProvider();
            
            SE_CODE Run();
            
        private:
            void ProcessRssFeed(UID feedId, const string& feedurl, const string& dateformat, const string& tags);
            
        private:
            ISession* _ps;
            
            

        };

    }
}

#endif	/* CECONOMICNEWSSTORAGEPROVIDER_H */

