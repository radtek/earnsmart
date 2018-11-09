/* 
 * File:   CZMQClient.cpp
 * Author: santony
 * 
 * Created on September 22, 2012, 8:51 PM
 */

#include "../seglobal/CObjectRoot.h"
#include <string.h>
#include "CZMQClient.h"
#include <boost/shared_ptr.hpp>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>

namespace se
{
    namespace net
    {

        CZMQClient::CZMQClient(ISession* ps) : CObjectRoot(ps), _zmqCtxt(0), _reqSocket(0), 
                //_logger(Logger::getLogger("se.net.zmqclient")),
                _msgChunkSizeMax(0x1000)
        {
        }

        CZMQClient::CZMQClient(const CZMQClient& orig) : CObjectRoot(orig), _msgChunkSizeMax(0x1000)
        {
            
        }

        CZMQClient::~CZMQClient() 
        {
            //LOG4CXX_INFO(_logger, "Closing zmq client.");
            if (_zmqCtxt)
            {
                zmq_close(_reqSocket);
                zmq_ctx_destroy(_zmqCtxt);
            }
            _zmqCtxt = 0;
            //LOG4CXX_INFO(_logger, "Closed zmq client.");
        }

        ErrorId CZMQClient::Init(const char* connectionstring)
        {
            // create the context
            _zmqCtxt = zmq_ctx_new();
            //LOG4CXX_INFO(_logger, "Opening a zmq client socket");
            _reqSocket = zmq_socket(_zmqCtxt, ZMQ_REQ);
            if (_reqSocket == NULL) // error
            {
                goto ErrorExit0;
            }
            //LOG4CXX_INFO(_logger, "Connecting to the remove server.");
            if (-1 == zmq_connect(_reqSocket, connectionstring))
            {
                goto ErrorExit1;
            }

            //LOG4CXX_INFO(_logger, "Connecting to the remove server succeeded.");
            
            return S_Success;
            
            ErrorExit1:
                //LOG4CXX_ERROR(_logger, "zmq server connection failed with " << strerror(errno) << "reported.");
                zmq_close(_reqSocket);
                _reqSocket = 0;
            
            ErrorExit0:
                //LOG4CXX_ERROR(_logger, "zmq socket opening failed with " << strerror(errno) << "reported.");
                zmq_ctx_destroy(_zmqCtxt);
                _zmqCtxt = 0;
                return E_ZMQError;
        }
        
        size_t CZMQClient::MakeRequest(CSTR requestMessage, IString** responseMessage)
        {
            return 0;
        }
    }
}