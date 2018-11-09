/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: santony
 *
 * Created on December 31, 2015, 4:19 PM
 */

#include <cstdlib>
#include <getopt.h>
#include "../semysql/CSEConnections.h"
#include <string>
#include "../semain/semain.h"

using namespace std;
using namespace se;
using namespace se::data;

string encrypt(const string& cleartext); 
string decrypt(const string& secret); 
void adduser(const string& options);


/*
 * 
 */
int main(int argc, char** argv)
{
    
    SP<IApp> papp;
    SEGetApp("", &papp);
    
    int c;
    int digit_optind = 0;
    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"adduser", required_argument, 0, 0},
            {"encrypt", required_argument, 0, 0},
            {"decrypt", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "e:d:",
            long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                string option = long_options[option_index].name;
                if (option == "encrypt")
                    cout << encrypt(optarg) << endl;
                else if (option == "decrypt")
                    cout << decrypt(optarg) << endl;
                else if (option == "adduser")
                    cout << adduser(optarg) << endl;
                break;            
            case 'a':
                cout << adduser(optarg) << endl;
                break;
            case 'e':
                cout << encrypt(optarg) << endl;
                break;
            case 'd':
                cout << decrypt(optarg) << endl;
                break;

            case '?':
                break;

            default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc)
    {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    return 0;
}


string encrypt(const string& cleartext)
{
    CSmartConnection conn(CSEConnections::GetMain());
    auto q = conn->query();
    q << "select AES_ENCRYPT(" << quote << cleartext << ", " << quote << conn.get_Key() << ")";
    auto qr = q.store();
    if (qr.size())
    {
        auto cstr = (CSTR) qr[0][0];
        return cstr;
    }
    return "";
}

string decrypt(const string& secret)
{
    return "Not implemented";
}

void adduser(const string& options)
{
    //const string& userId, const string& password, string& firstName, const string& lastName, string role
    
}