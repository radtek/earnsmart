#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <glib.h>

#include "IFunctions.h"
#include <vector>

using namespace std;

class CMainController
{
public:
    CMainController(GMainContext* gctxt);
    virtual ~CMainController();
    void Run();
    void Stop();
    void AddHandler(IFunction* handler);
    void RemoveHandler(IFunction* handler);
    void Interrupt();
    

private:
    static gboolean OnTimeout(gpointer data);
    gboolean Dispatch();

protected:

private:
    GMainContext* _gctxt;
    GMainLoop* _mainLoop;
    GSource* _timeoutSource;
    guint _timerid;
    //log4cxx::LoggerPtr _logger;
    vector<IFunction*> _handlers ;
    
};

#endif // MAINCONTROLLER_H
