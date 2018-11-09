/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on August 26, 2015, 9:13 AM
 */

#include <cstdlib>
#include "CTestEodData.h"
#include "../semain/semain.h"
#include "CVariadicTemplates.h"
#include "CTestFredDataSource.h"

using namespace std;
using namespace se;
using namespace se::config;

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    SP<se::IApp> pApp;
    SEGetApp(argv[0], &pApp);
    
    std::array<CUnitTest*, 1> tests = 
    {
        //new CTestEodData(),
        //new CVariadicTemplates(),
        new CTestFredDataSource()
    };
    
    for (auto p : tests)
        p->Run();
    
    // pointers will be deleted when process exists.

    return 0;
}

