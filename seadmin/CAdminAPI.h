/* 
 * File:   CAdminAPI.h
 * Author: santony
 *
 * Created on March 22, 2013, 9:20 PM
 */

#ifndef CADMINAPI_H
#define	CADMINAPI_H

#include "seadminapi.h"
////#include <log4cxx/logger.h>
#include "../semysql/CSEConnections.h"
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>
#include <random>
#include "../seglobal/CTimer.h"

using namespace mysqlpp;
using namespace se::data;
using namespace Poco;

namespace se
{
    namespace admin
    {
        class CAdminAPI : public CObjectRoot, public IAdminAPI
        {
        private:
            
        private:
            static void OnTimerCallback(const CTimer* timer, LPVOID state);
            
        public:
            CAdminAPI(ISession*);
            CAdminAPI(const CAdminAPI& orig) = delete;
            virtual ~CAdminAPI();
            
            long unsigned int Init() override;
            
            NamedObjectNamingType get_TypeId() override;

            
            virtual RP<IAudit> GetAuditor();
            
            virtual ErrorId get_Address(UID id, IAddress** ppOut) ;
            virtual ErrorId set_Address(IAddress* pIn) ;
            
            // get user .
            virtual ErrorId get_User(CSTR userId, IUser** ppUser) ;
            virtual ErrorId get_User(UID id, IUser** ppUser) ;
            
            long unsigned int get_UserFromSessionUniqueID(const char* uniqueSessionId, IUser** ppUser) override;

            
            // set user.
            virtual ErrorId set_User(IUser* ppIn) ;
            // get all users. the criteria is a keyvalue pair like fieldname=fieldvalue*
            virtual ErrorId get_Users(CSTR criteria, IROSPList<IUser> **ppUsers) ;
            
            virtual ErrorId CheckUserLogin(CSTR login, IUser** ppUser= nullptr);

            long unsigned int get_HasUserSessionExpired(const char* sessionId) override;

            
            virtual long unsigned int VerifyUser(const char* login, const char* pwd, UID* uid = nullptr);


            virtual long unsigned int UpdateAuthentication(const char* login, const char* currentPwd, const char* newPwd);


            virtual long unsigned int ResetGetTemporaryPassword(const char* login, IString** ppOut);


            virtual SE_CODE LoadRegistrationData(const char* uuid, IString** ppOut);

            virtual SE_CODE SaveRegistrationData(CSTR uuid, CSTR regdata);


            virtual SE_CODE RegisterUser(IUser** ppUser = nullptr) ;
            
            long unsigned int RegisterUser(const Json::Value& regInfo) override;
            
            long unsigned int ConfirmUser(const char* emailCode) override;
            
            
            // Completely unregister a user from the system. Note that this operation will 
            // remove all records about the user and hence should be used only in special case,
            // such as an incomplete registration.
            virtual ErrorId UnregisterUser(UID userId);
            
            SE_CODE UpgradeUser(const Json::Value& upgradeInfo) override;
            
            SE_CODE DowngradeUser(CSTR userlogin) override;
            
            // authentication
            virtual ErrorId AuthenticateUser(CSTR userId, CSTR password, bool IsEmailAddress = false) ;
          
            // enable or disable user.
            virtual ErrorId set_UserStatus(CSTR userId, bool IsActive) ;

            virtual long unsigned int set_UserRoles(unsigned long userId, UserRole roles);

            ///////////////////////////// User's session management ////////////////////////////
            // Utilizes underlying session instance to record user's session activity.
            SE_CODE RecordSessionActivity() override;

            
            /////////////////////////////Global methods/////////////////////////////

            virtual long unsigned int get_Countries(IROSPList<ICountry>** ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)> criteria = nullptr);


            virtual long unsigned int get_Countries(Json::Value& jsonOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, bool&) > criteria);

            
            virtual long unsigned int get_Country(ICountry** ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)> criteria = nullptr);

            long unsigned int get_Country(Json::Value& outVal, CSqlAst* criteria) override;

            
            virtual ErrorId get_States(IStateList** ppOut, std::function<CSTR (IState::QUERYFLDS, LGX&, BOOL& include)> criteria = nullptr);

            long unsigned int get_States(Json::Value& jsonOut, CSqlAst* criteria = nullptr) override;

            long unsigned int get_State(Json::Value& jsonOut, CSqlAst* criteria) override;

           
            /**********************Exchanges and Fees*************************/
            /*
             *  Returns exchange code, name and fee
             * @vendorSources (in): ['VENDOR1', 'VENDOR2']
             * @jsOut (out): [{ key: 99, code : 'xx', name: 'xxxxx', fee: 99.99, vendor: 'xx'}]
             * 
             */
            long unsigned int get_ExchangePackages(Json::Value& jsOut) override;

            
            /*************** busines operations **************/
            // Payment related
            virtual ErrorId get_PaypalExpressCheckOut(IPaypalExpressCheckout** ppOut);

            // Products
            // get product.
            virtual ErrorId get_Product(UID id, IProduct** ppOut) ;
            // set product
            virtual ErrorId set_Product(IProduct* pIn) ;
            // get list of products
            virtual ErrorId get_Products(IROSPList<IProduct>** ppOut) ;
            
            
            //////////////////////// Portfolio Operations ////////////////////////////////////

            virtual long unsigned int get_Porfolios(unsigned long ownerId, IPortfolioList** ppOut);
            
            virtual long unsigned int get_Porfolio(unsigned int portfolioId, IPortfolio** ppOut);
           
            long unsigned int get_Positions(unsigned long ownerId, IPositions** ppOut) override;

            
            // Help related methods.
            RP<IHelpDocTree> get_HelpDocTree() override;

            
        private:
            // used by set_User facade method. Connection is passed as an additional paramter
            ErrorId set_User(CSmartConnection& conn, IUser* pUser);
            ErrorId set_UserPassword(CSmartConnection& conn, UID userId, CSTR clearpassword);
            ErrorId set_Address(CSmartConnection& conn, IAddress* pIn);
            ErrorId set_UserAddress(CSmartConnection& conn, IUser* pUser, IAddress* pAddress);

            ErrorId SetProduct(Row const& row, IProduct** ppOut);
            ErrorId SetUser(CSmartConnection &conn,  Row const& row, IUser** ppOut);
            SE_CODE SyncUserSession(CSmartConnection &conn, UID useId, CSTR sessionId);

           
            SE_CODE get_Countries(mysqlpp::StoreQueryResult& sqr, const std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& include)>& criteria);
            ErrorId SetCountry(Row const& row, ICountry** ppOut);
            void SetCountry(Row const& row, Json::Value& out);
            ErrorId SetState(Row const& row, IState** ppOut);
            SE_CODE SetState(Row const& row, Json::Value& jsout);
            ErrorId SetSessionAndRememberStates(CSmartConnection& conn, UID UserId, CSTR sessionId, bool Remember);
            SE_CODE RecordSessionActivity(CSmartConnection& conn, UID userId);
            
            // Billing related
            // Start a customer as licensee. 
            // @param userId : customer user id
            // @param paymentType : a value from paymenttypes table (1 for paypal)
            // @param packages : a string value which is a comma seperated list of package ids (see exchange_packages table).
            // @param fee : real amount charged on the customer.
            SE_CODE BeginAsLicensedUser(CSmartConnection& conn, UID userId, U16 paymentType, CSTR packages, R8 fee, UID* newId = nullptr);
            
            // Downgrade  a customer to basic
            SE_CODE EndAsLicensedUser(CSmartConnection& conn, UID userId);
            
            // Misc refunds given to a customer.
            SE_CODE RefundLicensedUser(CSmartConnection& conn, UID userId, R8 amount, CSTR notes, UID refId);
            
            // Bill a customer (using a subscription type of engine).
            // @param billintType: 1 = Subscription Fee , 1 = Once Only, 2 = Refunds
            // @param refId = if this transaction is a complement to another transaction, the transid if that one.
            // @param tt = Transaction type (Debug or Credit). Not this parameter works only if billingType = OnceOnly
            SE_CODE BillLicensee(CSmartConnection& conn, UID userId, BILLINGTYPES billingType, R8 amount, CSTR notes, UID refId = 0, TRANSACTIONTYPES tt = TRANSACTIONTYPES::Credit);
            
            
        public:
            static Mutex adminLock;
            static SP<IAdminAPI> pApi;

        };
        
        
        /////////////////////////////////Global Methods///////////////////////////////////

        
        
    }
}


#endif	/* CADMINAPI_H */

