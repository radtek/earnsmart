#include <glib.h>
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include "ZmqServer.h"
#include "zhelpers.h"
#include <boost/shared_ptr.hpp>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/reader.h>

CZmqServer::CZmqServer(ZMQ_CTXT zmqCtxt) :
    _zmqCtxt(zmqCtxt),
    _logger(log4cxx::Logger::getLogger("se.bg.zmqserver")),
    _msgChunkSizeMax(0x1000)
{
    if (!g_thread_supported())
    {
        //g_thread_init(NULL);
    }
    _listenerThread = g_thread_create((GThreadFunc)ListenerThreadFunc, this, FALSE, &_threadError);
    if (!_listenerThread)
    {
        LOG4CXX_ASSERT(_logger, _listenerThread != 0, "Listener thread wasn't created.");
        g_error_free(_threadError);
    }
}

CZmqServer::~CZmqServer()
{
    //dtor
}

CZmqServer::CZmqServer(const CZmqServer& other) : _msgChunkSizeMax(0x1000)
{
    //copy ctor
}

gpointer CZmqServer::ListenerThreadFunc(gpointer parameter)
{
    CZmqServer* self = static_cast<CZmqServer*>(parameter);
    LOG4CXX_INFO(self->_logger, "Entering the listener thread.")
    // create the socket
    self->_responseSocket = zmq_socket(self->_zmqCtxt, ZMQ_REP);
    zmq_bind(self->_responseSocket, "tcp://*:8369");  // note that 8369 respresents "SE"
    LOG4CXX_INFO(self->_logger, "Bound and listening at port 8369.")
    while (1)
    {
        boost::shared_ptr<char> sReq(s_recv(self->_responseSocket), &free);
        //LOG4CXX_INFO(self->_logger, "Received message from a client.")
        // handle it
        boost::shared_ptr<char> sResp(self->Exchange(sReq.get()), &free);
        // send
        s_send(self->_responseSocket, sResp.get());
        //LOG4CXX_INFO(self->_logger, "Sending message to the client.")
    }
    LOG4CXX_INFO(self->_logger, "Exiting from listener thread.")
    return 0;
}

char* CZmqServer::Exchange(char* req)
{
    LOG4CXX_INFO(_logger, "Zmq server received : " << req);
    return (char*)"recvd";
}
