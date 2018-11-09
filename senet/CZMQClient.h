/* 
 * File:   CZMQClient.h
 * Author: santony
 *
 * Created on September 22, 2012, 8:51 PM
 */

#ifndef CZMQCLIENT_H
#define	CZMQCLIENT_H

#include <zmq.h>
//#include <log4cxx/logger.h>
#include "senetapi.h"
#include <zmq_utils.h>

using namespace se;
//using namespace log4cxx;

namespace se
{
    namespace net
    {
        class CZMQClient : 
            public CObjectRoot, 
                    public se::IObject
        {
        public:
            CZMQClient(ISession*);
            virtual ~CZMQClient();

            /* A connection string is in the following format:
             * tcp://address:port
             */
            virtual ErrorId Init(CSTR connectionstring);
            
            /* Synchronous version of send and receive */
            size_t MakeRequest(CSTR requestMessage, IString** responseMessage);
            

        private:
            CZMQClient(const CZMQClient& orig);
            
        private:

            void *_zmqCtxt, 
                 *_reqSocket;
            
            //LoggerPtr _logger;
            
            const size_t _msgChunkSizeMax;

        };
        
    }
}


#endif	/* CZMQCLIENT_H */

