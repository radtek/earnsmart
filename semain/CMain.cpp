/* 
 * File:   CMain.cpp
 * Author: santony
 * 
 * Created on October 30, 2012, 7:54 PM
 */

#include "semaininternal.h"
#include "../ReturnCodes.h"
#include "CSession.h"
#include "../seglobal/Object.h"
#include "CApplication.h"
#include "../seglobal/CDirectory.h"
#include "../senet/senetapi.h"

namespace se
{
    extern "C" 
    {
        ErrorId SEMAINEXP SEGetApp(CSTR process, IApp** ppOut) 
        {
            if (!process || !strlen(process))
                process = CDirectory::GetCurDir().c_str();
            if (process)
                CDirectory::processpath = process;
            if (CApplication::pOnlyApp == 0)
            {
                std::lock_guard<std::recursive_mutex> g(CApplication::appLock);
                if (CApplication::pOnlyApp == 0)
                {
                    //boost::locale::generator gen;
                    // Create locale generator 
                    //std::locale::global(gen("")); 
                    // "" - the system default locale, set
                    // it globally    
                    
                    // Inject code messages
                    CApplication::__loadMessages__(
                    {
                        CApplication::CodeMessageItem(E_LogConfigFile, "Configuration file for logging is missing."),
                        CApplication::CodeMessageItem(E_DBConnFile, "Database connection file is missing."),
                        CApplication::CodeMessageItem(E_HttpError, "An error while connecting to an remote http source.")
                    }
                    );
                    
                    RETURNIFFAILED(CObject<CApplication>::Create(nullptr, &CApplication::pOnlyApp));

                    // ensure that configuration files are loaded. Return an exception if not.
                    SP<se::config::IConfigurator> pConfig;
                    RETURNIFFAILED(CApplication::pOnlyApp->get_Configurator(&pConfig));
                }
            }
            
            CApplication::pOnlyApp->AddRef();

            *ppOut = CApplication::pOnlyApp;
            
            return S_Success;
        }

        ErrorId SEMAINEXP SEOpenApp(IApp** ppOut) 
        {
            if (CDirectory::processpath.empty())
                return E_NoProcessName;
            *ppOut = CApplication::pOnlyApp;
            CApplication::pOnlyApp->AddRef();
            return S_Success;
        }

//        ErrorId SEMAINEXP SECloseApp(IApp* pIn)
//        {
//            CApplication::pOnlyApp = nullptr;
//            return S_Success;
//        }
    }
}


