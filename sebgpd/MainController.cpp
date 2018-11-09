
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include "MainController.h"

#define TIMEOUTINTERVAL     1000    // milliseconds

CMainController::CMainController(GMainContext* ctxt) :
    _gctxt(ctxt),
    _logger(log4cxx::Logger::getLogger("se.bg.maincontroller")),
        _handlers(0)
{
    LOG4CXX_INFO(_logger, "Opening main controller.")
    //ctor
    _mainLoop = g_main_loop_new(_gctxt, TRUE);
    _timeoutSource = g_timeout_source_new(TIMEOUTINTERVAL);
    _timerid = g_source_attach(_timeoutSource, _gctxt);
    g_source_set_callback(_timeoutSource, OnTimeout, this, 0);
    LOG4CXX_INFO(_logger, "Exiting main controller ctor after setting timer callback.")
}

CMainController::~CMainController()
{
    //dtor
    g_main_loop_ref(_mainLoop);
    LOG4CXX_INFO(_logger, "Exiting main controller.")
}

void CMainController::Run()
{
    LOG4CXX_INFO(_logger, "Starting the main loop.")
    g_main_loop_run(_mainLoop);
    LOG4CXX_INFO(_logger, "Quiting main loop.")
}

void CMainController::Stop()
{
    LOG4CXX_INFO(_logger, "Request to stop the main loop.")
    g_main_loop_quit(_mainLoop);
    LOG4CXX_INFO(_logger, "Main loop exit in progress.")
}

gboolean CMainController::Dispatch()
{
    auto lSize = _handlers.size();
    LOG4CXX_INFO(_logger, "Controller has " << _handlers.size() << " function handlers.");
//    for (IFunction* func: _handlers)
//    {
    for (vector<IFunction*>::const_iterator it = _handlers.begin(); 
            it != _handlers.end(); ++it)
    {
        (*it)->Execute();
     //func->Execute();   
    }
    return TRUE;
}

gboolean CMainController::OnTimeout(gpointer data)
{
    CMainController* self = static_cast<CMainController*>(data);
    if (!self->Dispatch())
    {
        g_main_loop_quit(self->_mainLoop);
        return FALSE;
    }
    return TRUE;
}

void CMainController::AddHandler(IFunction* handler)
{
    if (handler)
    {
        auto size = _handlers.size();
        _handlers.push_back(handler);
    }
}

void CMainController::RemoveHandler(IFunction* handler)
{
    if (handler)
    {
        //_handlers.erase(handler);
    }
}

void CMainController::Interrupt()
{
    for (auto f : _handlers)
    {
        f->Interrupt();
    }
}