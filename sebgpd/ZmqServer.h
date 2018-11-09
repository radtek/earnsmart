#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <zmq.h>

// the zeromq conext.
#define ZMQ_CTXT    void*

/*
This type is a single listener running in a concurrent background thread
listening to commands from clients like smart-earn fastcgi library.
*/
class CZmqServer
{
    public:
        CZmqServer(ZMQ_CTXT zmqCtxt);
        ~CZmqServer();

    private:
        CZmqServer(const CZmqServer& other);
        static gpointer ListenerThreadFunc(gpointer parameter);

        // receives request message, returns response message.
        char* Exchange(char* req);

    protected:

    private:
        ZMQ_CTXT _zmqCtxt;
        void* _responseSocket;

        GThread* _listenerThread;
        log4cxx::LoggerPtr _logger;
        GError* _threadError;

        const size_t _msgChunkSizeMax;
        
};

#endif // ZMQSERVER_H
