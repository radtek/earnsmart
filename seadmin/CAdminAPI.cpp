/* 
 * File:   CAdminAPI.cpp
 * Author: santony
 * 
 * Created on March 22, 2013, 9:20 PM
 * 
 * 
 */

#include "internal.h"
#include "CAdminAPI.h"
#include "../seglobal/CList.h"
#include "CAudit.h"
#include "../seglobal/CROList.h"
#include "CUser.h"
#include "CUserType.h"
#include "../semysql/CMyqlUtilities.h"
#include "../seglobal/exceptions.h"
#include "CProduct.h"
#include "CCustomer.h"
#include "CCustomerSubscriptionType.h"
#include "CCustomerSubscriptionInfo.h"
#include "CPurchase.h"
#include "CCustomerStatus.h"
#include <boost/tokenizer.hpp>
#include "../seglobal/exceptions.h"
#include "../seglobal/CFunctionCache.h"
#include "CPaypalExpressCheckout.h"
#include "../seglobal/CROList.h"
#include "CCountry.h"
#include "CCountryState.h"
#include "CWebSession.h"
#include "../seglobal/CSecure.h"
#include "../seglobal/CString.h"
#include "../external/mariadbpp/mystring.h"
#include <uuid/uuid.h>
//#include <pegtl/rules_string.hh>
#include "../semain/CSession.h"
#include "../seglobal/CStringUtility.h"
#include "../senet/CSmtpClient.h"
#include "CHelpDocTree.h"
#include "../senet/CEmailTemplate.h"
#include "CPositions.h"
#include "../senet/CSmtpVmimeClient.h"
#include "../senet/senetapi.h"
#include "../semath/CRandomizer.h"


#define SEDBCONN        CSEConnections::GetMain()
#define SEDBADM         CSEConnections::GetAdmin()

#define FROMEMAIL "postmaster@earn-smart.com"
#define SESSIONEXPIREDAYS 14

using namespace se;
using namespace se::data;




namespace se
{
    namespace admin
    {
        Mutex CAdminAPI::adminLock;
        SP<IAdminAPI> CAdminAPI::pApi;
        
#define QUERYFIELDLOGIC        \
                        if (lgx == LGX_And)             \
                        {                               \
                            if (!bWhere)                \
                            {                           \
                                query << " where ";     \
                                bWhere = true;          \
                            }                           \
                            else                        \
                                query << " and ";       \
                        }                               \
                        else                            \
                        {                               \
                            if (!bWhere)                \
                            {                           \
                                query << " where ";     \
                                bWhere = true;          \
                            }                           \
                            else                        \
                                query << " or ";        \
                        }            

        
        //NamedObjectNamingType IAdminAPI::Name = 0xD;

        NamedObjectNamingType CAdminAPI::get_TypeId()
        {
            return IAdminAPIName;
        }


        CAdminAPI::CAdminAPI(ISession* ps) : CObjectRoot(ps)
        {
        }

        CAdminAPI::~CAdminAPI() 
        {
        }

        long unsigned int CAdminAPI::Init()
        {
            return S_OK;
        }

        
        RP<IAudit> CAdminAPI::GetAuditor()
        {
            IAudit* pAudit;
            CObject<CAudit>::Create(get_Session(), &pAudit);
            return pAudit;
        }
        
        ErrorId CAdminAPI::get_Address(UID id, IAddress** ppOut) 
        {
            if (id ==0 ) return S_False;
            CSmartConnection conn(SEDBCONN);
            try
            {
                Query q = conn->query();
                q << "select addresses.*, se_gbl_countries.country_name as country, se_gbl_states.name as state from addresses \
                    left join se_gbl_countries on addresses.country_id = se_gbl_countries.country_id \
                    left join se_gbl_states on addresses.state_id = se_gbl_states.state_id  \
                    where addresses.addr_id = " << id;
                auto qRes = q.store();
                if(qRes.size())
                {
                    const Row& row = qRes[0];
                    SP<IAddress> pAddr ;
                    auto retCode = CreateAddress(get_Session(), &pAddr);
                    if (FAILED(retCode))
                    {
                       // LOGE(_logger, "Failed to create address instance.");
                        return retCode;
                    }
                    pAddr->set_Name(((string)row["name"]).c_str());
                    pAddr->set_City(((string)row["city"]).c_str());
                    pAddr->set_State(((string)row["state"]).c_str());
                    pAddr->set_Street(((string)row["street1"]).c_str());
                    pAddr->set_Street2(((string)row["street2"]).c_str());
                    pAddr->set_Country(((string)row["country"]).c_str());
                    pAddr.CopyTo(ppOut); 
                    return S_Success;
                }
                return E_NotFound;
            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "Error : " << ex.what());
                return E_DBQueryException;
            }
            
        }
        
        ErrorId CAdminAPI::set_Address(IAddress* pIn) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                ErrorId retCode = set_Address(conn, pIn);
                if (SUCCEEDED(retCode))
                    conn.CommitTransaction();
                else
                    conn.RollbackTransaction();
                return retCode;
            }
            CATCHCLAUSE("set_Address");

        }

        
        ErrorId CAdminAPI::set_Address(CSmartConnection& conn, IAddress* pIn)
        {
            try
            {
                // the user may provide the country and state in 
                // full text or code. Verify both.
                I16 country_id;
                Query q = conn->query();
                q << "select country_id from  se_gbl_countries where country_name = " ;
                q << quote << pIn->get_Country();
                StoreQueryResult qr = q.store();
                if (qr.size())
                    country_id = qr[0][0];
                else
                {
                    //q = conn->query();
                    q << "select country_id from  se_gbl_countries where country_code = " ;
                    q << quote << pIn->get_Country();
                    qr = q.store();
                    if (qr.size())
                        country_id = qr[0][0];
                    else
                    {
                        //q = conn->query();
                        q << "select country_id from  se_gbl_countries where country_code1 = " ;
                        q << quote << pIn->get_Country();
                        qr = q.store();
                        if (qr.size())
                            country_id = qr[0][0];
                    }
                }
                
                // match and then insert
                //q = conn->query();
                q << "SELECT address_id from addresses where ";
                q << "city = " << quote << pIn->get_City();
                q << " and state = " << quote << pIn->get_State();
                q << " and street1 = " << quote << pIn->get_Street();
                q << " and street2 = " << quote << pIn->get_Street2();
                q << " and postcode = " << quote << pIn->get_PostalCode();
                q << " and country_id = " << country_id;
                qr = q.store();
                if (!qr.size())
                {
                    q << "INSERT INTO addresses (name, city, country_id, state ,street1,street2, postcode) ";
                    q << "VALUES (" ;
                    q << quote << pIn->get_Name() << ", " ;
                    q << quote << pIn->get_City() << ", " ;
                    q << country_id << ", " ;
                    q << quote << pIn->get_State() << ", " ;
                    q << quote << pIn->get_Street() << ", " ;
                    q << quote << pIn->get_Street2() << ", ";
                    q << quote << pIn->get_PostalCode();
                    q << " )";
                   // LOGIP(q.str());
                    q.exec();
                    // get auto increment id
                    UID addrid = (UID)q.insert_id();
                    pIn->set_Id(addrid);
                }
                else
                    pIn->set_Id((UID)qr[0][0]);
                return S_Success;
            }
            CATCHCLAUSE("set_Address (private)");
        }
        
        
        long unsigned int CAdminAPI::set_UserAddress(CSmartConnection& conn, IUser* pUser, IAddress* pAddress) 
        {
            try
            {
                SP<IUser> puser(pUser);
                SP<IAddress> pAddr(pAddress) ;
                I64 addId = 0;
                if (pAddr)
                {                
                    THROWIFFAILED(set_Address(conn, pAddr), "Failed to get address from user.");
                    addId = pAddr->get_Id();
                    Query q = conn->query();
                    q << "SELECT * FROM  user_addr where is_current = 1 and ";
                    q << "user_id = " << puser->get_Id() << " and addr_id = " << pAddr->get_Id();
                    auto qRes = q.store();
                    if(!qRes.size())
                    {
                        q = conn->query();
                        q << "update user_addr set is_current = 0 where ";
                        q << "user_id = " << puser->get_Id() << " and addr_id = " << pAddr->get_Id();
                        q.execute();
                        q = conn->query();
                        q << "INSERT INTO user_addr (user_id, addr_id, is_current, seton ) ";
                        q << "VALUES (" <<puser->get_Id()<<", "<<pAddr->get_Id()<<", 1, now())";
                        q.exec();                          
                    }
                    else
                    {
                        q = conn->query();
                        q << "UPDATE  user_addr set  ";
                        q << " is_current = 1, seton = now() ";
                        q << "where user_id = "<<puser->get_Id() << "and addr_id = "<<pAddr->get_Id();
                        q.exec();
                    }                    
                }
                return S_Success;
            }
            CATCHCLAUSE("set_UserAddress (private)");
        }

        ////////////////////////Private method begins///////////////////////////

        ErrorId CAdminAPI::get_User(CSTR userId, IUser** ppUser) 
        {
            try
            {
                const string funcName = "se.admin.get_User1";
                
                //TODO: utilize get_user that accept UID. in function , save userId -> uid in function map.
                // then using cached uid, get Iuser instance from function cache.
                CSmartConnection conn(SEDBCONN);
                Row row ;
                Query q = conn->query();
                q<<"SELECT * FROM  users where user_login = "<<quote<<userId;
                auto qRes = q.store();
                if (qRes.size())
                {
                    const Row& row = qRes[0];
                    ErrorId retCode = SetUser(conn, row, ppUser);
                    if (FAILED(retCode))
                    {
                    // LOGE(_logger, "Failed with error code " << retCode);
                    }
                    return retCode;
                }
               // LOGE(_logger, "Failed with error code " << E_NotFound);
                return E_NotFound;
            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "Error : " << ex.what());
                return E_DBQueryException;
            }
            
        }

        ErrorId CAdminAPI::get_User(UID id, IUser** ppUser) 
        {
            try
            {
                ostringstream funcName ;
                funcName << "se.admin.get_User_" << id;
                IUser* p;
                if (!FC.TrySPGetFValue(funcName.str(), id, p))
                {
                    Poco::ScopedLock<Mutex> g(adminLock);
                    if (!FC.TrySPGetFValue(funcName.str(), id, p))
                    {
                        // if id == 0, then anonymous user.
                        if (id == 0)
                        {
                            THROWIFFAILED(IUser::CreateAnonymous(get_Session(), &p), "Failed to create anonymous user.");
                            p->set_Active(true);
                            FC.SPSetFValue(funcName.str(), id, p);
                            *ppUser = p;
                            return S_Success;
                        }
                        else
                        {
                            CSmartConnection conn(SEDBADM);
                            Query q = conn->query();
                            {
                                q << "SELECT * FROM saadmin.users where ID = " <<quote << id;
                                auto qRes = q.store();
                                if (qRes.size())
                                {
                                    Row& row = qRes[0];
                                    ErrorId retCode = SetUser(conn, row, &p);
                                    if (FAILED(retCode))
                                    {
                                        if (FAILED(retCode))
                                            throw seexception(retCode, "Failed to open an user instance.");
                                    }
                                    FC.SPSetFValue(funcName.str(), id, p);
                                    *ppUser = p;
                                    return S_Success;
                                }
                                else
                                    throw seexception(E_NotFound, "User not found.");
                            }
                        }                      
                    }
                }
                p->AddRef();
                *ppUser = p;
                return S_Success;
            }
            CATCHCLAUSE("get_User");
        }

        
        long unsigned int CAdminAPI::get_UserFromSessionUniqueID(const char* uniqueSessionId, IUser** ppUser)
        {
            try
            {
                CSmartConnection conn(SEDBADM);
                Query q = conn->query();
                q << "select `ID` from saadmin.users where user_sessionid = " << quote << uniqueSessionId;
                auto store = q.store();
                UID userId = 0;
                if (store.size())
                {
                    userId = store[0][0];
                }
                return get_User(userId, ppUser);   // this method takes care of any anonymous user.
            }
            CATCHCLAUSE("get_UserFromSessionUniqueID");
        }

        
        
        ////////////////////// Private method begins///////////////////////////////
        
        /////////// private method begins/////////////////////////////

        long unsigned int CAdminAPI::set_User(CSmartConnection& conn, IUser* pUser) 
        {
            try
            {
                // ensure we have a safe pointer
                SP<IUser> pIn(pUser);
                if (pUser->get_Id())    // if saving an existing user info.
                {
                    Query q = conn->query();
                    q << "UPDATE users set";
                    q << " user_login = " << quote << pIn->get_UserId();
                    q << ", user_roles = " << (I32)pIn->get_Roles();
                    q << ", user_email = " << quote << pIn->get_UserId();
                    q << ", user_firstname = " << quote << pIn->get_FirstName();
                    q << ", user_lastname = " << quote << pIn->get_LastName();
                    q << ", user_isactive = " << quote << pIn->get_IsActive() ? '1' : '0';
                    q << ", user_updatedon = NOW() ";
                    q << " where ID = " << pIn->get_Id();
                    q.exec();
                    
                }
                else
                {   
                    Query q = conn->query();
                    q << "select ID from users where user_login = " << quote << pUser->get_UserId();
                    if (q.store().size())
                    {
                       // LOGE(_logger, "User with// LOGin " << pUser->get_UserId() << " already exists. Cannot continue.");
                        return E_UserLoginExists;
                    }
                    q = conn->query();
                    q << "INSERT INTO users ";
                    q << "(user_login, user_roles, user_email, user_registeredon, user_firstname, user_lastname, user_isactive, user_createdon ) ";
                    q << "VALUES ("; 
                    q << quote << pIn->get_UserId();
                    q << ", " << (I32)pIn->get_Roles();
                    q << ", " << quote <<  pIn->get_UserId();
                    q << ", NOW() ";
                    q << ", " << quote << pIn->get_FirstName();
                    q << ", " << quote << pIn->get_LastName();
                    q << ", " << pIn->get_IsActive() ? '1' : '0';
                    q << ", NOW() ";
                    q << ") " ;
                    q.exec();
                    pIn->set_Id((UID)q.insert_id());                             
                }
                return S_Success;
            }
            CATCHCLAUSE("set_User - private -");
        }

        
        //////////// private method ends/////////////////////////

        ErrorId CAdminAPI::set_User(IUser* pIn) 
        {
            try
            {
                /*start a transaction since multiple tables are involved.
                 * 1. grab address id from pIn->get_Address()
                 * 2. If this address_id = 0, then insert an address in address table
                 * 3. get the new address id in address instance. 
                 * 4. save user information + addressid
                 * 5. insert or update a row in user_addr also.
                 * 
                 */
                CSmartConnection conn(SEDBCONN);
                conn.BeginTransaction();
                ErrorId retCode = set_User(conn, pIn);
                if (SUCCEEDED(retCode))
                    conn.CommitTransaction();
                else
                    conn.RollbackTransaction();
                return retCode;
            }
            CATCHCLAUSE("set_User");
        }

        ErrorId CAdminAPI::get_Users(CSTR criteria, IROSPList<IUser> **ppUsers) 
        {
            string line = criteria;
            using namespace boost;
            typedef tokenizer< char_separator<char> > Tokenizer;
            typedef vector<string> TokenVector;
            TokenVector vec;
            char_separator<char> sep(";", "", keep_empty_tokens);
            Tokenizer tok(line, sep);
            vec.assign(tok.begin(), tok.end());
            I32 i = 0;
            string aWhere;
            
            while(i <vec.size())
            {
                string sub = vec[i];
                TokenVector vecSub;
                char_separator<char> sepSub("=", "", keep_empty_tokens);
                Tokenizer tokSub(sub, sepSub);
                vecSub.assign(tokSub.begin(), tokSub.end());
                string sVlue = vecSub[0];
                
                if(sVlue == "Id")
                {
                  aWhere = aWhere + "ID LIKE '%" +vecSub[1]+"'%";
                }
                else if(sVlue == "UserId")
                {
                    aWhere = aWhere + "user_login LIKE '%" +vecSub[1]+"'%";
                }
                else if(sVlue == "FirstName")
                {
                    aWhere = aWhere + "user_firstname LIKE '%" +vecSub[1]+"'%";
                }
                else if(sVlue == "LastName")
                {
                    aWhere = aWhere + "user_lastname LIKE '%" +vecSub[1]+"'%";
                }
                if(i<(vec.size()-1))
                    aWhere = aWhere+" and ";
                i = i++;
            }
           
            CSmartConnection conn(SEDBCONN);
            SP<CROSPList<IUser>> pList;
            ErrorId retCode = CObject<CROSPList<IUser>>::Create(get_Session(), &pList);
            if (FAILED(retCode))
            {
               // LOGEP("Error Creating List");
                return retCode;
            }
            Row row ;
            Query q = conn->query();
            
            q<<"SELECT * FROM  users where "<<aWhere;
            auto qRes = q.store();
            for (const Row& row : qRes)
            {
                SP<IUser> pUser;
                retCode = IUser::Create(get_Session(), &pUser);
                if (FAILED(retCode))
                {
                   // LOGE(_logger, "can't create User Type");
                    return retCode;
                }
                retCode = SetUser(conn, row, &pUser);
                if (FAILED(retCode))
                {
                   // LOGE(_logger, "Failed with error code " << retCode);
                    return retCode;
                }
                pList->Add(pUser._ptr);
            }
            pList.CopyTo(ppUsers);
            return S_Success;
        }

        long unsigned int CAdminAPI::set_UserPassword(CSmartConnection& conn, unsigned long userId, const char* clearpassword) 
        {
            try
            {
                Query q = conn->query();
                q << "update users set ";
                q << " user_pass = AES_ENCRYPT(" << quote << clearpassword << ", " << quote << conn.get_Key() << ")";
                q << " where ID = " << userId;
                //LOGIP(q.str());
                q.execute();
            }
            CATCHCLAUSE("set_UserPassword");
        }

        long unsigned int CAdminAPI::LoadRegistrationData(const char* uuid, IString** ppOut) 
        {
            using namespace mysqlpp;
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select regdata from user_registrations where uuid = " << quote << uuid;
                auto uqr = q.use();
                Row row;
                if (uqr and (row = uqr.fetch_row()))
                {
                    THROWIFFAILED( CString::Create( (CSTR)row["regdata"].c_str(), ppOut), "CString::Create");
                    return S_OK;
                }
                return E_NoDataToReturn;
            }
            CATCHCLAUSE("LoadRegistrationData");

        }


        long unsigned int CAdminAPI::SaveRegistrationData(const char* uuid, const char* regdata) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                /*
                 * INSERT INTO `sa`.`user_registrations`
                (`uuid`,
                `regdata`,
                `createdon`,
                `updatedon`)
                VALUES
                (<{uuid: }>,
                <{regdata: }>,
                <{createdon: }>,
                <{updatedon: }>);
                                 * 
                                 * 
                UPDATE `sa`.`user_registrations`
                SET
                `uuid` = <{uuid: }>,
                `regdata` = <{regdata: }>,
                `createdon` = <{createdon: }>,
                `updatedon` = <{updatedon: }>
                WHERE `uuid` = <{expr}>;

                 */
                 
                
                        
                Query q = conn->query();
                q << "select uuid from user_registrations where uuid = " << quote << uuid;
                if (q.store().size())
                {
                    q << "update user_registrations set";
                    q << " regdata = " << quote << regdata;
                    q << " and updatedon = now()";
                    q << " where uuid = " << quote << uuid;
                }
                else
                {
                    q << "insert into user_registrations (uuid, regdata, createdon) ";
                    q << "values (" << quote << uuid << ", " << quote << regdata << ", now())";
                }
                q.exec();
                return S_OK;
            }
            CATCHCLAUSE("SaveRegistrationData");
        }

        long unsigned int CAdminAPI::RegisterUser(const Json::Value& regInfo) 
        {
            
            // @regInfo : The regInfo parameter is json object of below format.
            // {
            //    RegStages : 0,           //// Registration stages 0=UserTypeAndAddress, 1=sendEmailToConfirmation, 2=emailConfirmed, 3=Paymentprovided, 4=RegistrationCompleted
            //    RegType : "standard",    //// or licensed)
            //    FirsName : "",
            //    LastName : "",
            //    EmailAddress : "",
            //    Password : "",
            //    ConfirmPassword: "",
            //    RememberMe : false,
            //    Street1 : "",
            //    Street2 : "",
            //    Town : "",
            //    Country : 0,
            //    State : 0,
            //    StateProvince : "",
            //    PostalCode : ""
            // }
            
#ifdef __DEBUG__
            Json::StyledWriter writer;
            BDBG << writer.write(regInfo);
#endif
            // validate data
            REGSTAGES regStage;
            std::string regType = "standard";
            UserRole role = UserRole::Standard; 
            std::string firstName, lastName, emailAddress;
            CSmartConnection conn(SEDBADM);
            std::string password, confirmPwd;
            std::string street1, street2, town, pc, stateprovince;
            I32 country = 0, state = 0;
            bool remember;
            firstName = regInfo.get("FirstName", "").asString();
            lastName = regInfo.get("LastName", "").asString();
            emailAddress = regInfo.get("EmailAddress", "").asString();
            password = regInfo.get("Password", "").asString();
            confirmPwd = regInfo.get("ConfirmPassword", "").asString();
            remember = regInfo.get("RememberMe", false).asBool();
            U64 emailCode = se::math::CRandomizer::DefaultRandomNumber();
            try
            {
                if (emailAddress.empty())
                {
                    return E_ValidateEmail;
                }

                if (password.empty() or password != confirmPwd)
                {
                    return E_ValidatePassword;
                }

                if (firstName.empty() and lastName.empty())
                {
                    return E_ValidateName;
                }

                if (!se::utils::CStringUtility::ValidateEmail(emailAddress))
                {
                    return E_ValidateEmail;
                }

                SP<IUser> pUser;
                if (SUCCEEDED(CheckUserLogin(emailAddress.c_str(), &pUser)))
                {

                    ((ISessionInterface*)get_Session())->Reset(pUser);
                    auto retSync = SyncUserSession(conn, pUser->get_Id(), get_Session()->get_UniqueId());
                    if (FAILED(retSync))
                    {
                        return retSync;
                    }
                    return E_PartiallyRegistered;
                }
                if (regType == "licensed")
                {
                    street1 = regInfo.get("Street1", "").asString();
                    street2 = regInfo.get("Street2", "").asString();
                    town = regInfo.get("Town", "").asString();
                    pc = regInfo.get("PostalCode", "").asString();
                    state = regInfo.get("State", 0).asInt();
                    country = regInfo.get("Country", 0).asInt();
                    if (street1.empty() or town.empty() or pc.empty())
                    {
                        BERR << "street1, town or postal code not given.";
                        return E_Validation;
                    }
                    if (state == 0)
                    {
                        stateprovince = regInfo.get("StateProvince", "").asString();
                    }
                    if (stateprovince.empty())
                    {
                        if (state<= 0 or state>=10000)
                        {
                            return E_ValidateState;
                        }
                    }
                    if (country <= 0 or country >=1000)
                    {
                        return E_ValidateCountry;
                    }
                    else
                    {
                        // verify state against country
                        if (state > 0)
                        {
                            Json::Value jsstate;
                            CSqlAst ast = AST_CONDITION(QUERYFIELDS::State::State::Id, AST_OPERATOR::Equal, state);
                            ast & AST_CONDITION(QUERYFIELDS::Country::Id, AST_OPERATOR::Equal, country);
                            BDBG << ast;
                            if (FAILED(get_State(jsstate, &ast)))
                            {
                                BERR << "There is no state that matches state_id " << state << " for country with id " << country;
                                return E_ValidateState;
                            }
                            // print state info
                            BDBG << jsstate;
                        }
                    }
                }
                conn.BeginTransaction();
                Query q = conn->query();
                UID userId = 0;
                q << "INSERT INTO saadmin.users ";
                q << "(user_login, user_pass, user_firstname, user_lastname, user_email, user_emailconfirmcode, user_registeredon, user_regstage, user_role, user_isactive, user_sessionid, user_remember, user_createdon) ";
                q << "VALUES ";
                q << "(" << quote << emailAddress << ", AES_ENCRYPT(" << quote << password << ", " << quote << conn.get_Key() << "), ";
                q << quote << firstName << ", ";
                q << quote << lastName << ", ";
                q << quote << emailAddress << ", ";
                q << quote << emailCode << ", ";
                q << "now(), " << (U16)REGSTAGES::SendEmailForConfirmation << ", " << (regType == "licensed" ? (int)UserRole::Licensed : (int)UserRole::Standard ) << ", 1, " << quote << get_Session()->get_UniqueId() << ", ";
                q << quote << (remember ? "1" : "0") << ", now() )";

                BDBG << q.str();
                q.exec();
                userId = q.insert_id();
                // obtain a reference to the user instance.
                pUser.Clear();
                if (!street1.empty() && ISOK(get_User(userId, &pUser)))
                {
                    //TODO
                    SP<IAddress> paddr;
                    IAddress::Create(this->get_Session(), &paddr);
                    paddr->set_City(street1.c_str());
                    paddr->set_Street2((street2.c_str()));
                    paddr->set_City(town.c_str());
                    paddr->set_State(stateprovince.c_str()); // paddr->set_State(state) //TODO takes integer
                    //paddr->set_Country() // TODO : takes integer.
                    THROWIFFAILED(pUser->set_Address(paddr), "User address not saved.");
                }
                // register user activity
                auto retCode = RecordSessionActivity(conn, userId);
                if (FAILED(retCode))
                {
                    throw se::seexception(retCode);
                }
                get_Session()->set_Id(userId);
            }
            RBCATCHCLAUSE("RegisterUser")
            try
            {
                // send an email
                string emailTemplate = "standardregistration.html";
                se::net::TemplateVaribles vars;
                vars.insert(make_pair("FirstName", regInfo.get("FirstName", "").asString()));
                std::ostringstream os;
                // if device is IOS ///TODO FOR ANDROID, WINDOWS, BROWSER, ETC.
                os << "earnsmart://confirmemail=";
                os << emailCode;
                vars.insert(make_pair("ConfirmUrl",  os.str() ));
                se::net::CEmailTemplate etempl(this->get_Session(),emailTemplate, vars);
                std::string emailContent = etempl.Apply();
                SP<IString> retMsg;
                auto retVal = se::net::SendEmail(
                        se::net::EMLPROVD::Office365, 
                        FROMEMAIL,
                        emailAddress.c_str(),
                        "Earnsmart Registration : Email Confirmation Required",
                        emailContent.c_str(),
                        &retMsg
                        );
                if (FAILED(retVal))
                {
                    BERR << "Failed to send email with error '" << get_CodeMessage(retVal) << "'";
                    throw se::seexception(E_EmailSendError);
                }
                else
                {
                    BINF << retMsg->get_Buffer();
                }
                conn.CommitTransaction();
                return S_OK;
            }
            RBCATCHCLAUSE("RegisterUser(SendEmailConfirmation)")
        }
        
        SE_CODE CAdminAPI::RegisterUser(IUser** ppUser)
        {
            ISession* pSession = this->get_Session();
            // generate a unique id
            uuid_t uniqueId;
            uuid_generate_time_safe(uniqueId);
            char buff2[256];
            uuid_unparse_lower(uniqueId, buff2);
            pSession->set_UniqueId(buff2);
            pSession->set_Id(0);
            // save a record in user sessions and return.
            CSmartConnection conn(SEDBADM);
            try
            {
                Query q = conn->query();
                q << "INSERT INTO user_sessions (";
                q << "session_id, user_id, session_date, session_type ";
                q << ") VALUES (";
                q << quote << buff2 << ", 0, NOW(), 1";
                q << ")";
                BDBG << q.str();
                q.exec();
                
                if (ppUser)
                {
                    return IUser::CreateAnonymous(get_Session(), ppUser);
                }
                return S_OK;
            }
            CATCHCLAUSE("RegisterUser")
        }

        long unsigned int CAdminAPI::ConfirmUser(const char* emailCode) 
        {
            CSmartConnection conn(SEDBADM);
            try
            {
                conn.BeginTransaction();
                // flag the user a email confirmed.
                UID userId = 0;
                Query q = conn->query();
                q << "select ID from users where user_emailconfirmcode = " << quote << emailCode;
                auto qstore = q.store();
                if (qstore.size())
                    userId = qstore[0][0];
                else
                {
                    BERR << "Register user not found in the system.";
                    conn.RollbackTransaction();
                    return E_UserRegistrationNotFound;
                }
                q = conn->query();
                q << "UPDATE saadmin.users SET ";
                q << "user_regstage = " << (U16)REGSTAGES::RegistrationCompleted << ", user_updatedon = NOW() ";
                q << "WHERE `ID` = " << userId;
                q.exec();
                conn.CommitTransaction();
                return S_RegistrationCompleted;
            }
            RBCATCHCLAUSE("RegisterUser(EmailConfirmed)")
        }

        
        long unsigned int CAdminAPI::UnregisterUser(unsigned long userId) 
        {
            try
            {
                CSmartConnection conn(SEDBADM);
                conn.BeginTransaction();
                try
                {
                    // delete customer record 'customers'
                    // delete user type records
                    // delete session record.
                    // delete address record
                    // delete user record.
                    Query q = conn->query();
                    q << "delete from customers where user_id = " << userId;
                    q.execute();
                    //q = conn->query();
                    q << "delete from user_usertypes where user_id = " << userId;
                    q.execute();
                    //q = conn->query();
                    q << "delete from user_addr where user_id = " << userId;
                    q.execute();
                    //q = conn->query();
                    q << "delete from users where ID = " << userId;
                    q.execute();
                    //q = conn->query();
                    q << "delete from user_sessions where user_id = " << userId;
                    q.execute();
                    conn.CommitTransaction();
                    return S_Success;
                }
                RBCATCHCLAUSE("UnregisterUser");
            }
            CATCHCLAUSE("UnregisterUser");
        }

        SE_CODE CAdminAPI::UpgradeUser(const Json::Value& upgradeInfo) 
        {
            /*
             * 
             */
            
            // capture session user
            RP<IUser> pUser = ((ISessionInterface*)get_Session())->get_User();
            if (!pUser)
            {
                BERR << "get_Session() didn't return a user object of standard license.";
                return E_UserRegistrationNotFound;
            }
            
            if (pUser->get_IsInRole(UserRole::Licensed))
            {
                return E_UserAlreadyLIcensed;
            }
            
            if (!pUser->get_IsInRole(UserRole::Standard))
            {
                return E_CannotUpgradeUser;
            }
            
            if (upgradeInfo.empty() || !upgradeInfo.isMember("paymentdetails") || !upgradeInfo.isMember("packages"))
            {
                BERR << "Invalid parameter 'upgradeInfo'. Doesn't contain required information.";
                return E_JsonRpcInsufficientParameters;
            }
            
            assert(pUser->get_UserId() > 0);
            
            CSTR customertoken = upgradeInfo["paymentdetails"]["customertoken"].asCString();
            
            R8 fees = upgradeInfo["paymentdetails"]["fees"].asDouble();
            
            const Json::Value& packages = upgradeInfo["packages"];
            
            // verify user
            auto ret = this->CheckUserLogin(pUser->get_UserId(), &pUser);

            RETIFFAILED(ret);
            
            CSmartConnection conn(SEDBADM);
            conn.BeginTransaction();
            try
            {
                // update users table (regstage completed, 
             
                Query q = conn->query();
                q << "update users set user_role = " << (U32)UserRole::Licensed;
                q << ", user_registered = Now() ";
                q << ", user_updatedon = Now() ";
                q << " where ID = " << pUser->get_Id();
                BDBG << q.str();
                q.execute();
                
                ERRIFFAILED(BeginAsLicensedUser(conn, pUser->get_Id(), 1 /* paypal */, packages.asCString(), fees));
                
                conn.CommitTransaction();

                this->RecordSessionActivity();

                return S_OK;
            }
            RBCATCHCLAUSE("UpgradeUser")
        }
        
        SE_CODE CAdminAPI::DowngradeUser(CSTR userlogin) 
        {
            // apart from downgrading in our tables, we also want to cancel
            // the payment processing of the user in the paypay server.
            if (!userlogin || !strlen(userlogin))
            {
                return E_InvalidArg;
            }
            
            // verify user
            SP<IUser> pUser;
            auto ret = this->CheckUserLogin(userlogin, &pUser);
            RETIFFAILED(ret);

            if (pUser->get_Roles() != UserRole::Licensed)
            {
                return E_CannotDowngradeUser;
            }

            try
            {
                CSmartConnection conn(SEDBADM);
                conn.BeginTransaction();
                try
                {
                    Query q = conn->query();
                    q << "update users set user_role = " << (U32)UserRole::Standard;
                    q << ", user_registered = Now() ";
                    q << ", user_updatedon = Now() ";
                    q << " where ID = " << pUser->get_Id();
                    BDBG << q.str();
                    q.execute();
                    ERRIFFAILED(EndAsLicensedUser(conn, pUser->get_Id()));
                    conn.CommitTransaction();
                    this->RecordSessionActivity();
                }
                RBCATCHCLAUSE("DowngradeUser")
                bool yes  =false;
                if (yes)
                {
                SP<IPaypalExpressCheckout> ppCheckout;
                auto retCode= this->get_PaypalExpressCheckOut(&ppCheckout);
                if (ISOK(retCode))
                {
                    SP<IString> profileId;
                    retCode = ppCheckout->Step0x1_CancelRecurringPaymentsProfileStatus("ss", "ss", &profileId);
                }
                if (FAILED(retCode))
                {
                    // EMAIL TO ADMINISTRATION TO HANDLE THIS MANUALLY.
                    se::net::SendEmail(se::net::EMLPROVD::SparkPost, FROMEMAIL, "postmaster@earn-smart.com", "ADMIN-URGENT: Paypal Cancellation Failed", "User xxx's downgrade failed to paypal cancelation failure.");
                }
                }
                return S_OK;
            }
            catch(const std::exception& ex)
            {
                BERR << ex.what();
                return E_DBUpdateError;
            }
        }

        ErrorId CAdminAPI::CheckUserLogin(CSTR login, IUser** ppUser)
        {
            try
            {
                if (!strlen(login))
                    throw seexception(E_InvalidArg, "login parameter is empty.");
                
                //is email ?
                bool isemail = se::utils::CStringUtility::ValidateEmail(login);
                CSmartConnection conn(SEDBCONN);
                Query qCheck = conn->query();
                qCheck << "SELECT ID FROM saadmin.users where ";
                if (isemail)
                    qCheck << " user_email = " << quote << login;
                else
                    qCheck << " user_login = " << quote << login;
                BDBG << qCheck.str();
                auto store = qCheck.store();
                if (store.size())
                {
                    UID userId = store[0][0];
                    if (ppUser)
                    {
                        return get_User(userId, ppUser);
                    }
                    return S_OK;
                }
                else
                {
                    return E_UnknownUser;
                }
            }
            CATCHCLAUSE("CheckUserLogin")
        }

        long unsigned int CAdminAPI::ResetGetTemporaryPassword(const char* login, IString** ppOut) 
        {
            CSmartConnection conn(SEDBADM);
            try
            {
                if (!strlen(login))
                    throw seexception(E_InvalidArg, "login parameter is empty.");
                conn.BeginTransaction();
                Query qCheck = conn->query();
                qCheck << "SELECT ID, user_pass FROM saadmin.users where user_login = " << quote << login;
                auto sqr = qCheck.store();
                if (!sqr.size())
                    throw seexception(E_NotFound, "Login not registered in the system.");
                
                // reset password process involves generating a new password with 8 chars and then
                // encrypting it and save to the database.
                char buff1[8];
                CSecure::GenerateRandomPassword(buff1, 8);
                CString::Create(buff1, ppOut);
                
                // generate a unique session id and store back in the table and return in the session instance.
                uuid_t uniqueId;
                uuid_generate_time_safe(uniqueId);
                char buff2[256];
                uuid_unparse_lower(uniqueId, buff2);
                
                
                auto q = conn->query();
                q << "Update saadmin.users set " ;
                q << "user_pass=AES_ENCRYPT(" << quote << buff1 << ", " << quote << conn.get_Key() << ")";
                q << ", user_sessionid = " << quote << buff2;
                q << " where user_login=" << quote << login;
                q.exec();
                
                get_Session()->set_UniqueId(buff2);
                
                conn.CommitTransaction();
                return S_OK;
            }
            RBCATCHCLAUSE(__FUNCTION__)
        }

        long unsigned int CAdminAPI::UpdateAuthentication(const char* login, const char* currentPwd, const char* newPwd) 
        {
            
            if (!newPwd or !strlen(newPwd))
            {
                BERR << "New password is invalid.";
                return E_IncorrectOperation;
            }
            
            UID uid;
            auto rcode = VerifyUser(login, currentPwd, &uid);
            if (E_SecurityAuthorization == rcode)
            {
                BWRN << "Authentication failed for " << login;
                return rcode;
            }
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "Update users set user_pass=AES_ENCRYPT(" << quote << newPwd << ", " << quote << conn.get_Key() << ")";
                q << " where ID=" << uid;
                q.exec();
                return S_Success;
            }
            CATCHCLAUSE(__FUNCTION__)
        }

        long unsigned int CAdminAPI::get_HasUserSessionExpired(const char* sessionId) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select user_sessionid from users";
                q << " where user_sessionid = " << quote << sessionId;
                q << " and datediff(now(), coalesce(user_createdon, user_updatedon)) > " << SESSIONEXPIREDAYS;
                return q.store().size() ? S_False : E_SessionExpired;
            }
            CATCHCLAUSEAUTO
        }
        

        long unsigned int CAdminAPI::VerifyUser(const char* login, const char* pwd, UID* uid) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                
                Query q = conn->query();
                q << "select * from users ";
                q << "where user_login = " << quote << login ;
                q << " and ";
                q << " aes_encrypt(" << quote << pwd << ", " << quote << conn.get_Key() << ") ";
                q << " = user_pass";
                //LOGIP(q.str());
                auto qRes = q.store();
                if (!qRes.size())
                    throw seexception(E_SecurityAuthorization, "Security authentication failed for the user.");
                if (uid)
                    *uid = qRes[0]["ID"];
                return S_Success;
            }
            CATCHCLAUSE(__FUNCTION__)

        }

        
        ErrorId CAdminAPI::AuthenticateUser(CSTR userId, CSTR password, bool IsEmailAddress) 
        {
            try
            {
                
                // generate a unique session id and store back in the table and return in the session instance.
                uuid_t uniqueId;
                uuid_generate_time_safe(uniqueId);
                char buff[256];
                uuid_unparse_lower(uniqueId, buff);
                
                CSmartConnection conn(SEDBADM);
                Query q = conn->query();
                q << "select * from users ";
                q << "where ";
                if (IsEmailAddress)
                    q << "user_email = " << quote << userId;
                else
                    q << "user_login = " << quote << userId ;
                q << " and ";
                q << " aes_encrypt(" << quote << password << ", " << quote << conn.get_Key() << ") ";
                q << " = user_pass";
                BDBG << q.str();
                auto qRes = q.store();
                if (qRes.size())
                {
                    UID uid = qRes[0]["ID"];
                    get_Session()->set_Id(uid);
                    q = conn->query();
                    q << "update users set user_sessionid = " << quote << buff << ", user_updatedon = NOW() where ID = " << uid;
                    q.exec();
                    ((CSession*)get_Session())->set_UniqueId(buff);
                    return S_Success;
                }
                else
                {
                    ((CSession*)get_Session())->set_UniqueId(buff);
                    throw seexception(E_SecurityAuthorization, "Security authentication failed for the user.");
                }
            }
            CATCHCLAUSE("AuthenticateUser")
        }

        long unsigned int CAdminAPI::SetSessionAndRememberStates(CSmartConnection& conn, unsigned long UserId, const char* sessionId, bool Remember) 
        {
            try
            {
                Query q = conn->query();
                q << "update users set user_sessionid = " << quote << sessionId;
                q << ", user_remember=" << (Remember ? '1' : '0') ;
                q << " where ID = " << UserId;
                q.exec();
                return S_OK;
            }
            CATCHCLAUSE(__FUNCTION__)
        }


        ErrorId CAdminAPI::set_UserStatus(CSTR userId, bool IsActive) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q<<"UPDATE users set user_status = "<<IsActive<<" where user_nickname = "<<quote<<userId;
                q.exec();
                return S_Success;
            }
            CATCHCLAUSE(__FUNCTION__)
            
        }

        long unsigned int CAdminAPI::set_UserRoles(unsigned long userId, UserRole roles) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "UPDATE users set user_roles = "<< (I32)roles <<" where ID = " << userId;
                q.exec();
                return S_Success;
            }
            CATCHCLAUSE(__FUNCTION__)
        }
        

        ///////////////////////////// User's session management ////////////////////////////

        long unsigned int CAdminAPI::RecordSessionActivity() 
        {
            if (!this->get_Session())
            {
                return E_SessionUnknown;
            }
            CSmartConnection conn(SEDBADM);
            return RecordSessionActivity(conn, get_Session()->get_UserId());
        }

        long unsigned int CAdminAPI::RecordSessionActivity(CSmartConnection& conn, UID userId) 
        {
            if (!this->get_Session())
            {
                return E_SessionUnknown;
            }
        
            try
            {
                Query q = conn->query();
                q << "CALL spRegisterUserSession(" << quote << get_Session()->get_UniqueId() << ", " << userId << ")";
                q.execute();
                return S_OK;
            }
            CATCHCLAUSE("RecordSessionActivity")            
        }

        
        
        /////////////////////////////////Global Methods///////////////////////////////////
        
        SE_CODE CAdminAPI::get_Countries(mysqlpp::StoreQueryResult& sqr, const std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)>& criteria)
        {
            try
            {
                // ask for fields needed
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                if (!criteria)
                    query << "select * from se_gbl_countries where country_id > 0 order by country_name";
                else
                {
                    bool bWhere = false;
                    query << "select * from se_gbl_countries ";
                    BOOL bresult = false;
                    LGX lgx = LGX_None;
                    CSTR sret = criteria(ICountry::QUERYFLDS::Name, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << " country_name = " << quote << sret;
                        bWhere=true;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::TwoChar, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << " country_code = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::ThreeChar, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_code1 = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::IsPaypalavailable, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_ispaypalavailable = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::Id, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_id = " << sret;
                    }
                    query << " order by country_name";
                }
                
               // LOGIP(query.str());
                
                sqr = query.store();
                
                if (!sqr.size())
                    return E_NotFound;            
                
                return S_OK;
            }
            CATCHCLAUSE("get_Countries(private)");
        }

        long unsigned int CAdminAPI::get_Countries(Json::Value& jsonOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, bool&) > criteria) 
        {
            try
            {
                Json::Value& list = jsonOut;
                mysqlpp::StoreQueryResult sqr;
                THROWIFFAILED(get_Countries(sqr, criteria), "Failed to query data for countries.");
                for (const Row& row : sqr)
                {
                    Json::Value cntry ;
                    cntry["key"] = (I16)row["country_id"];
                    cntry["name"] = (string)row["country_name"];
                    cntry["code1"] = (string)row["country_code1"]; 
                    cntry["code"] = (string)row["country_code"];
                    cntry["ispaypalavailable"] = (string)row["country_ispaypalavailable"]=="1";
                    list.append(cntry);
                }
                return S_OK;
            }
            CATCHCLAUSE("get_Countries(json)");
        }

        
        long unsigned int CAdminAPI::get_Countries(IROSPList<ICountry>** ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)> criteria) 
        {
            try
            {
                mysqlpp::StoreQueryResult sqr;
                THROWIFFAILED(get_Countries(sqr, criteria), "Failed to query data for countries.");
                SP<CROSPList<ICountry>> pList;
                THROWIFFAILED(CROSPList<ICountry>::CreateROSPList(get_Session(), &pList), "Failed to create ROSPList for countries.");
                for (const Row& row : sqr)
                {
                    SP<ICountry> pc;
                    THROWIFFAILED(SetCountry(row, &pc), "Failed to create a country instance.");
                    pList->Add(pc);
                }
                pList.CopyTo(ppOut);
                return S_Success;
            }
            CATCHCLAUSE("get_Countries(list)");
        }
        

        long unsigned int CAdminAPI::get_Country(ICountry** ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)> criteria) 
        {
            try
            {
                // ask for fields needed
                CSmartConnection conn(SEDBCONN);
                
                Query query = conn->query();
                if (!criteria)
                    query << "select * from se_gbl_countries order by country_name";
                else
                {
                    bool bWhere = false;
                    query << "select * from se_gbl_countries ";
                    BOOL bresult = false;
                    LGX lgx = LGX_None;
                    CSTR sret = criteria(ICountry::QUERYFLDS::Name, lgx, bresult);
                    if (bresult && sret)
                    {
                        query << " where " << "country_name = " << quote << sret;
                        bWhere=true;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::TwoChar, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << " country_code = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::ThreeChar, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_code1 = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::IsPaypalavailable, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_ispaypalavailable = " << quote << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(ICountry::QUERYFLDS::Id, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_id = " << sret;
                    }
                    query << " order by country_name";
                }
                
               // LOGIP(query.str());
                
                auto result = query.store();
                if (result.size())
                {
                    const Row& row = result[0];
                    return SetCountry(row, ppOut);
                }
                return E_NotFound;
            }
            CATCHCLAUSE("get_Country");

        }

        SE_CODE CAdminAPI::get_Country(Json::Value& outVal, CSqlAst* criteria)
        {
            if (!criteria)
            {
                return E_InvalidArg;
            }
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                query << "select * from list_CountryStates where " << *criteria;
                auto store = query.store();
                if (!store.size())
                    return E_NotFound;
                SetCountry(store[0], outVal);
                return S_OK;
            }
            CATCHCLAUSE("get_Country");
        }
        
        // private method begin
        void CAdminAPI::SetCountry(const Row& row, Json::Value& out) 
        {
            out["country_id"] = (I16)row["country_id"];
            out["country_name"] = (string)row["country_name"];
            out["country_code1"] = (string)row["country_code1"];
            out["country_code"] = (string)row["country_code"];
            out["country_ispaypalavailable"] = (string)row["country_ispaypalavailable"];
        }

        
        long unsigned int CAdminAPI::SetCountry(const Row& row, ICountry** ppOut) 
        {
            return CObject<CCountry>::Create(
                    get_Session(),
                    (I16)row["country_id"],
                    (string)row["country_name"], 
                    (string)row["country_code1"], 
                    (string)row["country_code"], 
                    (string)row["country_ispaypalavailable"]=="1", 
                    ppOut);
        }
        
        
        
        // private method end

        ErrorId CAdminAPI::get_States(IStateList** ppOut, std::function<CSTR(IState::QUERYFLDS,LGX&,bool&)> criteria)
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                if (!criteria)
                    query << "select * from se_gbl_states order by name";
                else
                {
                    bool bWhere = false;
                    query << "select * from se_gbl_states ";
                    BOOL bresult = false;
                    LGX lgx = LGX_None;
                    CSTR sret = criteria(IState::QUERYFLDS::Name, lgx, bresult);
                    if (bresult && sret)
                    {
                        query << " where " << "name = " << quote << sret;
                        bWhere=true;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(IState::QUERYFLDS::Code, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "code = " << quote << sret;                        
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(IState::QUERYFLDS::CountryId, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "country_id = " << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(IState::QUERYFLDS::CountryCode, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        auto qcnty = conn->query();
                        qcnty << "select country_id from se_gbl_countries where country_code=" << quote << sret;
                        auto scnty = qcnty.store();
                        query << "country_id = " << (scnty.size() ? (CSTR)scnty[0][0] : "0");
                    }
                    query << " order by name";
                }
               // LOGIP(query.str());
                auto qr = query.store();
                if (!qr.size())
                    return E_NotFound;
                else
                {
                    SP<CROSPList<IState>> pList;
                    THROWIFFAILED(CROSPList<IState>::CreateROSPList(get_Session(), &pList), "Failed to create ROSPList for countries.");
                    for (const Row& row : qr)
                    {
                        SP<IState> pc;
                        THROWIFFAILED(SetState(row, &pc), "Failed to create a state instance.");
                        pList->Add(pc);
                    }
                    pList.CopyTo(ppOut);
                    return S_Success;
                }
              
            }
            CATCHCLAUSE("get_States");
        }


        SE_CODE CAdminAPI::get_States(Json::Value& jsonOut, CSqlAst* criteria)
        {
            try
            {
                
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                if (!criteria)
                    query << "select * from list_CountryStates order by country_name, state_name";
                else
                {
                    const CSqlAst& ast = *criteria;
                    query << "select * from list_CountryStates where ";
                    query << ast;
                    query << " order by state_name";
                }
                BDBG << query.str();
                auto qr = query.store();
                if (!qr.size())
                    return E_NotFound;
                else
                {
                    for (const Row& row : qr)
                    {
                        Json::Value state;
                        THROWIFFAILED(SetState(row, state), "Failed to create a state instance.");
                        jsonOut.append(state);
                    }
                    return S_Success;
                }
              
            }
            CATCHCLAUSE("get_States");
        }

        long unsigned int CAdminAPI::get_State(Json::Value& jsonOut, CSqlAst* criteria) 
        {
            if (!criteria)
            {
                BERR << "Invalid argument criteria.";
                return E_InvalidArg;
            }
            
            try
            {
                
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                query << "select * from list_CountryStates where " << *criteria;
                BDBG << query.str();
                auto store = query.store();
                if (!store.size())
                {
                    return E_NotFound;
                }
                SetState(store[0], jsonOut);
                return S_OK;
            }
            CATCHCLAUSE("get_State");
        }

        
        // private method begin

        long unsigned int CAdminAPI::SetState(const Row& row, IState** ppOut) 
        {
            return CObject<CCountryState>::Create(
                    get_Session(),
                    (UID)row["state_id"],
                    (string)row["name"], 
                    (string)row["code"], 
                    (I16)row["country_id"], 
                    ppOut);
        }
        
        long unsigned int CAdminAPI::SetState(const Row& row, Json::Value& jsout) 
        {
            jsout["key"] = (I32)row["state_id"];
            jsout["name"] = (string)row["state_name"];
            jsout["code"] = (string)row["state_code"];
            jsout["countrykey"] = (I16)row["country_id"];
            SetCountry(row, jsout);
            return S_OK;
        }
        
        /**********************Exchanges and Fees*************************/
        /*
         *  Returns exchange code, name and fee
         * @vendorSources (in): ['VENDOR1', 'VENDOR2']
         * @jsOut (out): [{ key: 99, code : 'xx', name: 'xxxxx', fee: 99.99, vendor: 'xx'}]
         * 
         */

        long unsigned int CAdminAPI::get_ExchangePackages(Json::Value& jsOut) 
        {
            try
            {
                CSmartConnection conn(SEDBADM);
                Query q = conn->query();
                q << "select * from exchange_packages";
                for (const Row& row : q.store())
                {
                    Json::Value exchData;
                    exchData["key"] = (I32)row["package_id"];
                    exchData["name"] = (string)row["name"];
                    exchData["description"] = (string)row["description"];
                    exchData["fee"] = (R8)row["fee"];
                    jsOut.append(exchData);
                }
                return S_OK;
            }
            CATCHCLAUSE("get_ExchangePackages")
        }

        
        // private method end
        /*************** busines operations **************/
        
        ErrorId CAdminAPI::get_PaypalExpressCheckOut(IPaypalExpressCheckout** ppOut)
        {
            try
            {
                return CObject<CPaypalExpressCheckout>::Create(get_Session(), ppOut);
            }
            CATCHCLAUSE("get_PaypalExpressCheckOut");
        }
        

        ErrorId CAdminAPI::get_Product(UID id, IProduct** ppOut) 
        {
            ErrorId retCode = E_UnknownError;
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            try
            {   
                Row row;
               
                Query q = conn->query();
                q << "select products.*, product_prices.monthly_price as monthly_price, product_prices.quarterly_price as quarterly_price, ";
                q << "product_prices.yearly_price as yearly_price from products " ;
                q << "left join product_prices on products.product_id = product_prices.product_id ";
                q << "where products.product_id =" << id;
                auto qRes = q.use();
                row = qRes.fetch_row();
               
                if(row)
                {
                    retCode = SetProduct(row,ppOut);
                    if (FAILED(retCode))
                    {
                       // LOGE(_logger, "Failed with error code " << retCode);
                    }
                    return retCode;
                }
               return E_NotFound;
            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "Error update: " << ex.what());
                return E_DBUpdateError;
            }
            
        }

        ErrorId CAdminAPI::set_Product(IProduct* pIn) 
        {
            CSmartConnection conn(SEDBCONN);
            Query q = conn->query();
            try
            {
                {
                    q<<"INSERT INTO products (name) VALUES ("<<quote<<pIn->get_Name()<<")";
                    q.exec();
                    I64 pdt_Id = (I64)q.insert_id();
                    pIn->set_Id(pdt_Id);
                }
                {
                    q<<"INSERT INTO product_prices (product_id, monthly_price, quarterly_price, yearly_price) VALUES ("<<pIn->get_Id()<<",\
                            "<<pIn->get_PriceMonthlyRate()<<","<<pIn->get_PriceQuarterlyRate()<<","<<pIn->get_PriceYearlyRate()<<")";
                    q.exec();
                }
                return S_Success;
            }
            catch (const std::exception& ex)
            {
               // LOGE(_logger, "Error update: " << ex.what());
                return E_DBUpdateError;
            }
            
        }

        ErrorId CAdminAPI::get_Products(IROSPList<IProduct>** ppOut) 
        {
            ErrorId retCode = E_UnknownError;
            CSmartConnection conn(SEDBCONN); 
            SP<CROSPList<IProduct>> pList;
            retCode = CObject<CROSPList<IProduct>>::Create(get_Session(), &pList);
            if (FAILED(retCode))
            {
               // LOGEP("Error Creating List");
                return retCode;
            }
            Query q = conn->query();
            q << "select products.*, product_prices.monthly_price as monthly_price, product_prices.quarterly_price as quarterly_price, \
                    product_prices.yearly_price as yearly_price from products \
                    join product_prices on products.product_id = product_prices.product_id\
                    order by products.name";
            auto qRes = q.use();
            if (!qRes)
            {
               // LOGE(_logger, "can't found products");
                return E_DBQueryException;
            }
            Row row ;
            while (row = qRes.fetch_row())
            {
                SP<IProduct> pProduct ;
                retCode = IProduct::Create(get_Session(), &pProduct);
                
                if (FAILED(retCode))
                {
                   // LOGE(_logger, "can't create products");
                    return retCode;
                }
                retCode = SetProduct(row, &pProduct);
                if (FAILED(retCode))
                {
                   // LOGE(_logger, "Failed with error code " << retCode);
                    return retCode;
                }
                
                pList->Add(pProduct);
            }
            pList.CopyTo(ppOut);
            return S_Success;
            
        }
        
        
        /////////////////////////////// Portfolio Operations /////////////////////////////////

        long unsigned int CAdminAPI::get_Porfolio(unsigned int portfolioId, IPortfolio** ppOut) 
        {

        }

        long unsigned int CAdminAPI::get_Porfolios(unsigned long ownerId, IPortfolioList** ppOut) 
        {

        }
        

        long unsigned int CAdminAPI::get_Positions(unsigned long ownerId, IPositions** ppOut) 
        {
            Json::Value val;
            auto ret = CObject<CPositions>::Create(get_Session(), val, ppOut);
            return ret;
        }

        
        /********************* Private Methods ************************/
       
        ErrorId CAdminAPI::SetUser(CSmartConnection &conn, Row const& row, IUser** ppOut)
        {
            try
            {
                ErrorId retCode = CObject<CUser>::Create(get_Session(), ppOut);
                if (FAILED(retCode))
                {
                   // LOGE(_logger, "can't create User Type");
                    return retCode;
                }
                (*ppOut)->set_Id(row["ID"]);
                (*ppOut)->set_UserId(row["user_login"]);
                (*ppOut)->add_Role((UserRole)(I32)row["user_role"]);
                (*ppOut)->set_FirstName(row["user_firstname"]);
                (*ppOut)->set_LastName(row["user_lastname"]);
                (*ppOut)->set_Active(row["user_isactive"]);
                (*ppOut)->set_RegistrationStage((REGSTAGES)(I32)row["user_regstage"]);
                ((CUser*)(*ppOut))->set_CurrentSessionId(row["user_sessionid"]);
                
                auto q = conn->query();
                q << "select a.* from addresses a, (select addr_id from user_addr where ";
                q << "user_id = " << (UID)row["ID"] << " and is_current = 1 order by seton limit 1) b where a.address_id = b.addr_id";
                StoreQueryResult result ;
                if ((result = q.store()).size())
                {
                    ((CUser*)(*ppOut))->_addrId = row[0];
                }
                return retCode;
            }
            catch (std::exception const& ex)
            {
               // LOGE(_logger, "Error: " << ex.what());
                return E_DBQueryException;
            }
        }
        
        SE_CODE CAdminAPI::SyncUserSession(CSmartConnection &conn, UID userId, CSTR sessionId)
        {
            try
            {
                conn.BeginTransaction();
                Query q = conn->query();
                q << "update users set user_sessionid = " << quote << sessionId << ", user_updatedon = NOW() where ID = " << userId;
                q.exec();
                if (FAILED(this->RecordSessionActivity(conn, userId)))
                {
                    throw se::seexception("Failed to record session activity. Rolling back.");
                }
                conn.CommitTransaction();
            }
            catch(const se::seexception& ex)
            {
                conn.RollbackTransaction();
                BERR << ex.what();
                return E_DBUpdateError;
            }
            catch(const mysqlpp::Exception& ex)
            {
                conn.RollbackTransaction();
                BERR << ex.what();
                return E_DBUpdateError;
            }
            catch (const std::exception& ex)
            {
                conn.RollbackTransaction();
                BERR << ex.what();
                return E_DBUpdateError;
            }
        }
        
        ErrorId CAdminAPI::SetProduct(Row const& row, IProduct** ppOut)
        {
            
            if (!row) return E_NotFound;
            
            ErrorId retCode = CObject<CProduct>::Create(get_Session(), ppOut);
            if (FAILED(retCode))
            {
                return retCode;
            }
            (*ppOut)->set_Id(row["product_id"]);
            (*ppOut)->set_Name(row["name"]);
            (*ppOut)->set_PriceMonthlyRate(row["monthly_price"]);
            (*ppOut)->set_PriceQuarterlyRate(row["quarterly_price"]);
            (*ppOut)->set_PriceYearlyRate(row["yearly_price"]);
            return retCode;
        }
        
        // Billing related
        
        SE_CODE CAdminAPI::BeginAsLicensedUser(CSmartConnection& conn, UID userId, U16 paymentType, CSTR packages, R8 fee, UID* newId)
        {
            try
            {
                Query q = conn->query();
                q << "insert into cust_licensesetup ";
                q << "( userid, paymenttype, packagekeys, fees, startedon ) ";
                q << "VALUES ";
                q << "(" << userId << ", " << paymentType << ", " << quote << packages << ", " << fee << ", NOW())";
                q.execute();
                if (newId)
                {
                    *newId = q.insert_id();
                }
            }            
            CATCHCLAUSEAUTO
        }
        
        SE_CODE CAdminAPI::EndAsLicensedUser(CSmartConnection& conn, UID userId)
        {
            try
            {
                Query q = conn->query();
                q << "update cust_licensesetup set endedon = NOW() where userid = " << userId;
                q.execute();
            }            
            CATCHCLAUSEAUTO
        }
        
        SE_CODE CAdminAPI::RefundLicensedUser(CSmartConnection& conn, UID userId, R8 amount, CSTR notes, UID refId)
        {
            return BillLicensee(conn, userId, BILLINGTYPES::Refunds, amount, notes, refId);
        }

        SE_CODE CAdminAPI::BillLicensee(CSmartConnection& conn, UID userId, BILLINGTYPES billingType, R8 amount, CSTR notes, UID refId, TRANSACTIONTYPES transType)
        {
            // this table has only inserts. no deletes 
            // eg. if a fee is charged twice accidently and customer needs refund
            // the duplicate entry remains there, but will capture its unique id and 
            // then create a new table for 'Debit-Refund' and refid set to the unique id.
            
            if (!userId)
            {
                BERR << "User id not given.";
                return E_InvalidArg;
            }
            
            if (!amount || amount <= 0)
            {
                BERR << "amount parameter must be a positive amount";
                return E_InvalidArg;
            }
            
            if (!notes || !strlen(notes))
            {
                BERR << "notes parameter is empty";
                return E_InvalidArg;
            }
            
            TRANSACTIONTYPES tt;
            switch(billingType)
            {
                case BILLINGTYPES::SubscriptionFee:
                    tt = TRANSACTIONTYPES::Credit;
                    break;
                case BILLINGTYPES::Refunds:
                    tt = TRANSACTIONTYPES::Debit;
                    if (!refId)
                    {
                        BERR << "refId parameter must be greater than zero.";
                        return E_InsufficientParameters;
                    }
                    break;
                case BILLINGTYPES::OnceOnly:
                    tt = transType;
            }
            
            if (refId)
            {
                Query q = conn->query();
                q << "select billingid from cust_billing where billingid = " << refId;
                if (!q.store().size())
                {
                    BERR << "refId provided is not found";
                    return E_IncorrectOperation;
                }
            }
            
            try
            {
                Query q1 = conn->query();
                q1 << "insert into cust_billing ";
                q1 << "( userid, billingtype, transtype, transdate, amount, referid, notes ) ";
                q1 << "VALUES (";
                q1 << userId << ", ";
                q1 << (U16)billingType << ", ";
                q1 << quote << (char)transType << ", ";
                q1 << "NOW(), ";
                q1 << amount << ", ";
                q1 << refId << ", ";
                q1 << quote << notes ;
                q1 << ")";
                q1.execute();
            }
            CATCHCLAUSEAUTO
        }

        
        
        /////////////////////////// Help Related Methods ////////////////////////

        RP<IHelpDocTree> CAdminAPI::get_HelpDocTree()
        {
            SP<IHelpDocTree> docTree ;
            if (FAILED(CObject<CHelpDocTree>::Create(get_Session(), &docTree)))
                return nullptr;
            return docTree;
        }
        
   }
   
}