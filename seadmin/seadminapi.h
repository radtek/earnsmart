/* 
 * File:   seadminapi.h
 * Author: santony
 *
 * Created on March 17, 2013, 1:12 PM
 */

#ifndef SEADMINAPI_H
#define	SEADMINAPI_H

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../seglobalenums.h"
#include "../sedata/QueryFields.h"
#include "../seglobal/CTimeStamp.h"
#include "../seglobal/CDate.h"
#include "../sesecurities/sesecuritiesapi.h"
#include "../seglobal/CAst.h"

#define ADMEXP __attribute__ ((visibility ("default")))

using namespace se;
using namespace se::securities;

namespace se
{
    namespace admin
    {
        
        /*
         * Error codes are defined the following way in a 32-bit number (HRESULT)
         * 
         *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
         *------------------------------------------------------------------------------------------
         *  E| |C|          |     Facility     | T |                    Code                        |
         * 
         * Facility code of this library is 11
         * 
         */
        
        #define E_PaypalError                                   0x800B0001
        //#define E_PaypalProfileNotActiveOrSuspended           0x800B0002
        //#define E_PaypalProfileNotActive                      0x800B0003
        //#define E_PaypalProfileNotSuspended                   0x800B0004
        #define E_UserLoginExists                               0x800B0005
        #define E_RegistrationInfoInComplete                    0x800B0006
        #define E_Validation                                    0x800B0007  //  Failed to validate give input data.
        #define E_ValidateEmail                                 0x800B0008  //  Failed to validate give input data.
        #define E_ValidateCountry                               0x800B0009  //  Failed to validate give input data.
        #define E_ValidateState                                 0x800B000A  //  Failed to validate give input data.
        #define E_ValidatePassword                              0x800B000B  //  Password not given or password doesn't match with password confirmation.
        #define E_ValidateName                                  0x800B000C  //  First or last name or both not provided.
        #define E_HelpDocHasChildren                            0x800B000D  //  The action cannot take place because this item has children.
        #define E_RegistrationEmailFailed                       0x800B000E  // Registration succeeded, but failed to notify the registered user through an email.
        #define E_PartiallyRegistered                           0x800B000F
        #define E_UnknownUser                                   0x800B0010  // Unknown user identifier or login id.
        #define E_UserAlreadyLIcensed                           0x800B0011  // User is already licensed.
        #define E_CannotUpgradeUser                             0x800B0012  // User is not a standard user. Upgrade failed.
        #define E_CannotDowngradeUser                           0x800B0013  // User is not a licensed user. Downgrade failed.
        #define E_UserRegistrationNotFound                      0x800B0014  // No registration information found in our system. It could be because the previously provided registration information has expired. Please register again.
        
        #define S_NewUserAddress                                0x000B0001      // success, but a new user address record is created
        
        #define S_AnonymousSession                              0x000B0002      // CAnonymous session is created
        #define S_RegistrationCompleted                         0x000B0003  // Registration completed.      
        
        typedef enum 
        {
            AUDIMP_Normal = 0x1,
            AUDIMP_Moderate = 0x2,
            AUDIMP_Critical = 0x3,
        } AUDIT_IMPORTANCE, AUDIMP;
        
        typedef enum enUserSessionType
        {
            UST_Unknown = 0x0,
            UST_SignIn = 0x1,
            UST_SignOut = 0x2,
            UST_Registration = 0x3,
            UST_SessionInit = 0x4,
            UST_SessionExpired = 0x5,
            UST_Others = 0xFF
        } USER_SESSION_TYPE, UST;
        
        typedef enum 
        {
            None = 0xFF,
            UserTypeAndAddress = 0,
            SendEmailForConfirmation = 1,
            EmailConfirmed = 2,
            PaymentProvided = 3,
            RegistrationCompleted = 4
        } REGISTRATION_STAGES, REGSTAGES;
        
        enum class BILLINGTYPES : U16
        {
            SubscriptionFee = 1,
            OnceOnly = 2,
            Refunds = 3
        };
        
        enum class TRANSACTIONTYPES : char
        {
            Debit = 'D',
            Credit = 'C'
        };

        struct IAudit : public IObject
        {
            virtual void Critical(CSTR user, CSTR category, CSTR message) = 0;
            virtual void Moderate(CSTR user, CSTR category, CSTR message) = 0;
            virtual void Normal(CSTR user, CSTR category, CSTR message) = 0;
        };

        struct ICountry : public IIdName
        {
            virtual CSTR get_TwoCharCode() = 0;
            virtual CSTR get_ThreeCharCode() = 0;
            virtual bool get_IsPaypalAvailable() = 0;
            
            enum QUERYFLDS
            {
                Id,
                Name,
                TwoChar,
                ThreeChar,
                IsPaypalavailable
            };
            
        };
        
        struct IState : public IIdName
        {
            virtual CSTR get_Code() = 0;
            virtual ErrorId get_Country(ICountry**) = 0;
            
            enum QUERYFLDS
            {
                Id,
                Name,
                Code,
                CountryId,
                CountryCode
            };
        };
        
        struct IUserType : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, IUserType**) ;
            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR value) = 0;
            virtual bool get_IsAdmin() = 0;
            virtual void set_IsAdmin(bool) = 0;
        };
        
        
        struct IUser;
        
        typedef IROSPList<IUserType> IUserTypeList;

        struct IUser : public ISDObject, public IObject, public ISerialization, public IDeserialization
        {
            static ErrorId Create(ISession*, IUser**) ;

            virtual CSTR get_UserId() = 0;
            virtual void set_UserId(CSTR val) = 0;
            
            virtual CSTR get_FirstName() = 0;
            virtual void set_FirstName(CSTR val) = 0;

            virtual CSTR get_LastName() = 0;
            virtual void set_LastName(CSTR val) = 0;

            virtual CSTR get_FullName() = 0;
            
            virtual bool get_IsActive() = 0;
            virtual void set_Active(bool val) = 0;
            
            virtual ErrorId get_Address(IAddress** ppVal) = 0;
            virtual ErrorId set_Address(IAddress* pVal) = 0;
            
            virtual UserRole get_Roles() = 0;
            virtual void add_Role(UserRole roleToAdd) = 0;
            virtual void rem_Role(UserRole roleToRemove) = 0;

            virtual bool get_IsInRole(UserRole role) = 0;

            virtual CTimeStamp& get_RegisteredOn() = 0;
            virtual CSTR get_ActivationKey() = 0;
            
            virtual REGSTAGES get_RegistrationStage() = 0;
            virtual void set_RegistrationStage(REGSTAGES) = 0;
            
            // specific permanent update methods.
            virtual SE_CODE store_CurrentRoles() = 0;
            virtual SE_CODE store_CurrentStatus() = 0;
            
            // application session id
            virtual CSTR get_CurrentSessionId() = 0;
            
            
            // Static Methods
            static SE_CODE ADMEXP CreateAnonymous(ISession*, IUser** ppOut);
            static SE_CODE ADMEXP CreateBasic(ISession*,  IUser** ppOut);
            static SE_CODE ADMEXP CreateLicensed(ISession*, IUser** ppOut);
            
        };
        
        struct IWebSession : public IObject
        {
            virtual CSTR get_SessionId() = 0;
            virtual UID get_UserId() = 0;
            virtual SE_CODE get_ProductFinds(ISecurityDescriptionList** ppOut, I16 limitTo = 50) = 0;
            virtual SE_CODE set_NewProductFound(UID ProductId) = 0;

            virtual void set_SessionId(CSTR sid) = 0;
            virtual void set_UserId(UID uid) = 0;
        
            virtual RP<IUser> get_User() = 0;
            
            ///////////// Will contain all services accessible to this web session /////////////
            
            
        };
        
        struct ICustomerSubscriptionType : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, ICustomerSubscriptionType**) ;

            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR value) = 0;
        };
        
        struct ICustomerStatus : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, ICustomerStatus**) ;
            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR value) = 0;
        };
        
        // forward declaration.
        struct ICustomer;
        
        struct IProduct : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, IProduct**) ;
            virtual CSTR get_Name() = 0;
            virtual void set_Name(CSTR name) = 0;
            virtual R4 get_PriceMonthlyRate() = 0;
            virtual R4 get_PriceQuarterlyRate() = 0;
            virtual R4 get_PriceYearlyRate() = 0;
            virtual void set_PriceMonthlyRate(R4 value) = 0;
            virtual void set_PriceQuarterlyRate(R4 value) = 0;
            virtual void set_PriceYearlyRate(R4 value) = 0;
        };
        
        struct ICustomerSubscriptionInfo : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, ICustomerSubscriptionInfo**) ;
            virtual ErrorId get_Customer(ICustomer** ppOut) = 0;
            virtual void set_CustomerId(UID customerId) = 0;
            virtual DATETIME get_StartedOn() = 0;
            virtual void set_Started(DATETIME const& value) = 0;
            virtual DATETIME get_EndedOn() = 0;
            virtual void set_EndedOn(DATETIME const& value) = 0;
            virtual RP<ICustomerStatus> get_Status() = 0;
            virtual void set_Status(ICustomerStatus* pIn) = 0;
            virtual RP<ICustomerSubscriptionType> get_Type() = 0;
            virtual void set_SubscriptionType(ICustomerSubscriptionType* pIn) = 0;
            virtual PERIOD get_PayFrequency() = 0;
            virtual void set_PayFrequency(PERIOD const& value) = 0;
        };
        
        struct ICustomer : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, ICustomer**) ;
            virtual CSTR get_Code() = 0;
            virtual ErrorId get_Details(IUser** ppOut) = 0;
            virtual ErrorId get_Address(IAddress** ppOut) = 0;
            virtual void set_Code(CSTR value) = 0;
            virtual ErrorId set_Details(IUser *pIn) = 0;
            virtual ErrorId set_Address(IAddress *pIn) = 0;
            virtual ErrorId get_CurrentSubscriptionInfo(ICustomerSubscriptionInfo** ppOut) = 0;
            virtual ErrorId set_CurrentSubscriptionInfo(ICustomerSubscriptionInfo* pIn) = 0;
        };
        
        struct IPurchase : public IObject, public ISerialization
        {
            static ErrorId Create(ISession*, IPurchase**) ;
//            virtual ICustomer* get_Customer() = 0;
            virtual ErrorId get_Customer(ICustomer** ppOut) = 0;
            
//            virtual IProduct* get_Product() = 0;
            virtual ErrorId get_Product(IProduct** ppOut) = 0;
            virtual ErrorId get_SubscriptionInfo(ICustomerSubscriptionInfo** ppOut) = 0;
//            virtual ICustomerSubscriptionInfo* get_SubscriptionInfo() = 0;
            virtual CDate get_PurchaseDate() = 0;
            virtual R4 get_Amount() = 0;
            virtual ErrorId get_PurchaseType(ICustomerSubscriptionType** ppOut) = 0;
//            virtual ICustomerSubscriptionType* get_PurchaseType() = 0;

            virtual void set_CustomerId(UID id) = 0;
            virtual void set_ProductId(UID id) = 0;
            virtual void set_PurchaseTypeId(UID id) = 0;
            virtual void set_PurchaseDate(CDate const& value) = 0;
            virtual void set_Amount(R4 value) = 0;
            virtual void set_CustomerSubscriptionInfoId(UID value) =0;
        };
        
        struct IPortfolioItem : public IObject, public ISerialization
        {
            // get id returns portfolio id.
            virtual SE_CODE get_SecurityDescription(ISecurityDescription** ppOut) = 0;
            virtual SE_CODE set_SecurityDescription(ISecurityDescription* pIn) = 0;
            
            virtual const OmniType& get_Corelation() const = 0;
            virtual void set_Corelation(const OmniType& val) = 0;
            
        };
        
        struct IPositions : public IJsonObject
        {
            
        };

        struct IPortfolioItemList : public ISPList<IPortfolioItem>
        {
        };
                
        struct IPortfolio : public IIdName
        {
            virtual CSTR get_Notes() const = 0;
            virtual void set_Notes(CSTR v) = 0;
            
            virtual const OmniType& get_AMean() const = 0;
            virtual const OmniType& get_GMean() const = 0;
            virtual const OmniType& get_ReturnRisk() const = 0;
            virtual const OmniType& get_Volatility() const = 0;
            virtual const OmniType& get_Alpha() const = 0;
            virtual const OmniType& get_Beta() const = 0;
            virtual const OmniType& get_RiskFreeReturn() const = 0;
            virtual const DateTime& get_CreatedOn() const = 0;
            virtual const DateTime& get_ModifiedOn() const = 0;

            virtual void set_AMean(const OmniType& val)= 0;
            virtual void set_GMean(const OmniType& val)= 0;
            virtual void set_ReturnRisk(const OmniType& val)= 0;
            virtual void set_Volatility(const OmniType& val)= 0;
            virtual void set_Alpha(const OmniType& val)= 0;
            virtual void set_Beta(const OmniType& val)= 0;
            virtual void set_RiskFreeReturn(const OmniType& val)= 0;

            virtual SE_CODE get_Items(IPortfolioItemList** ppOut) = 0;
            
            virtual SE_CODE Save() = 0;
        };
        
        struct IPortfolioList : public ISPList<IPortfolio>
        {
        };
        
        struct IPaypalExpressCheckout : public IObject
        {
            // start with intiating an express check out. Provide the redirect and cancellation urls
            // logurl can be set to 0 (NULL)
            // the return value pptoken will be used to redirect user to paypal login site (https://www.sandbox.paypal.com/cgi-bin/webscr?cmd=_express-checkout&token=InsertTokenHere).
            virtual ErrorId Step01_SetExpressCheckout(CSTR returnUrl, CSTR cancelUrl, CSTR logoUrl, IString** ppToken) = 0;

            // using the internal token, obtain paye rid.
            virtual ErrorId Step02_GetExpressCheckoutDetails(CSTR token, BOOL* HasAccepted, IUser *&pInOut, IString** PayerId) = 0;
            
            // Setup recurring payment for the payer with trial subscription.
            // The profile status id returned is either "ActiveProfile" o r"PendingProfile"
            virtual ErrorId Step03_CreateRecurringPaymentsWithTrialProfile(IProduct* product, CSTR payerId, DATETIME& startDate, IString** ppProfileId, IString** ppProvileStatus) = 0;
            
            // this is for canceling  a recurring payment. The return value error code can be
            // one of the above E_PaypalProfileXXXXX. The ppProfileId will be same as profileid.
            virtual ErrorId Step0x1_CancelRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId ) = 0;  
            virtual ErrorId Step0x2_SuspendRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId ) = 0;  
            virtual ErrorId Step0x3_ReactivateRecurringPaymentsProfileStatus(CSTR profileid, CSTR note, IString** ppProfileId ) = 0;  
            
            //TODO virtual ErrorId UpdateRecurringPaymentsProfile(CSTR profileId, CDate& profileStartDate, CSTR subscriberName, CSTR note, R8 amount = 0.0, R8 taxAmount = 0.0) = 0;
            
        };
        
        typedef IROSPList<ICountry> ICountryList;
        typedef IROSPList<IState> IStateList;

        static NamedObjectNamingType IAdminAPIName = 0xD;


        /*
         * Financial documentation tree data structure.
         */
        struct IHelpDocTree : public IObject
        {
            //returns a document tree upto given sublevel. Default is 10. Set to -1 for entire document.
            virtual SE_CODE get_RootDoc(Json::Value& value, int sublevels = 10) = 0;
            // Returns Json::null when nothng to return.
            virtual SE_CODE get_Doc(UID docId, Json::Value& value, int sublevels = 10) = 0;
            // Save a new or existing document under given parent id.
            // If saving an existing document, docid must be a value
            // from the store. If the parentdocid is different 
            // then the document will be saved under that parent.
            virtual SE_CODE set_Doc(Json::Value& value) = 0;
            
            virtual SE_CODE del_Doc(UID docId, bool agreedToDelChildren = false) = 0;
        };
        
        struct IAdminAPI : public INamedObject, public IErrorDescription
        {
            
            virtual ErrorId get_Address(UID id, IAddress** ppOut) = 0;
            virtual ErrorId set_Address(IAddress* pIn) = 0;
            
            // get user .
            virtual ErrorId get_User(CSTR userId, IUser** ppUser) = 0;
            virtual ErrorId get_User(UID id, IUser** ppUser) = 0;
            // get a user from user's previously used session id.
            virtual SE_CODE get_UserFromSessionUniqueID(CSTR uniqueSessionId, IUser** ppUser) = 0;
            
            // set user.
            virtual ErrorId set_User(IUser* ppIn) = 0;
            // get all users. the criteria is a keyvalue pair like fieldname=fieldvalue*
            virtual ErrorId get_Users(CSTR criteria, IROSPList<IUser> **ppUsers) = 0;
            
            virtual SE_CODE SaveRegistrationData(CSTR uuid, CSTR regdata) = 0;
            
            virtual SE_CODE LoadRegistrationData(CSTR uuid, IString** ppOut) = 0;
            
            // Register an anonymous user. A user need to be registered 
            // before the EARNSMART can be used.
            virtual SE_CODE RegisterUser(IUser** ppUser = nullptr) = 0;

            // Register a user who has a minimum role 'Standard' (Admin, internal, standard or licensed). 
            // A user need to be registered before the EARNSMART can be used.
            //[[deprecated("This method is depecrated. Use RegisterUser() or RegisterUser()]]
            //virtual SE_CODE RegisterUser(ISession* ps, IUser*, CSTR pwd) = 0;
            
            // Register as a trial user with paypal payment features.
            //[[deprecated("This method is depecrated. Use RegisterUser(ISesssion*, IUser**")]]
            //virtual ErrorId RegisterUser(IUser* pUser, CSTR password, CSTR ppprofileid, IUserSession*& pInOut) = 0;
            
            // Register a user. A registered user may be standard or licensed.
            // We are not offering any trial period. Payments are not refundable.
            // User can unregister any time. Their registration will be active until
            // their billing cycle period ends.
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
            //    StateProvince : "", // this value used when State == 0 .
            //    PostalCode : ""
            // }
            virtual SE_CODE RegisterUser(const Json::Value& regInfo) = 0;
            
            // User confirmation is achieved by a email code.
            virtual SE_CODE ConfirmUser(CSTR emailCode) = 0;
            
            // Completely unregister a user from the system. Note that this operation will 
            // remove all records about the user and hence should be used only in special case,
            // such as an incomplete registration.
            virtual ErrorId UnregisterUser(UID userId) = 0;            
            
            // A basic user upgrades to licensed user and starts making payments.
            // upgradeInfo param is a json object in the form
            /*  
             {
               "method" : "upgradeuser",
               "jsonrpc" : "2.0",
               "id" : "1007",
               "params" : [
                 {
                    "paymentdetails" : {
                     "customertoken" : "xdsds",
                     "fees" : 12.0
                   },
                   "packages" :[1,4]
                 }
               ]
             }
             */
            virtual SE_CODE UpgradeUser(const Json::Value& upgradeInfo) = 0;
            
            // A licensed user can downgrade to basic user and stop making payments
            virtual SE_CODE DowngradeUser(CSTR userlogin) = 0;
            
            // check if email address already available. Returns S_Success if already exists. Returns E_NotFound otherwise.
            // If ppUser is not null, then the use object representing the login is returned.
            virtual ErrorId CheckUserLogin(CSTR login, IUser** ppUser= nullptr) = 0;
            
            // Verify if a user's session has expired. If expired, return S_OK, else S_False
            virtual SE_CODE get_HasUserSessionExpired(CSTR sessionId) = 0;

            // Simply verifies if the user's login and password matches.
            // Optionally returns the user unique system id.
            virtual ErrorId VerifyUser(CSTR login, CSTR pwd, UID* uid = nullptr) = 0;
            
            // Update password
            virtual SE_CODE UpdateAuthentication(CSTR login, CSTR currentPwd, CSTR newPwd) = 0;
            
            // Reset and retrieve temporary password in ppOut parameter.
            virtual ErrorId ResetGetTemporaryPassword(CSTR login, IString** ppOut) = 0;
            
            // authentication. Return E_SecurityAuthorization when authorization failed. E_DBQueryException for any database error.
            // @param IsEmailAddress : Is the userId provided user's email address (true) or user name (false)
            virtual ErrorId AuthenticateUser(CSTR userId, CSTR password, bool IsEmailAddress = false) = 0;
            // enable or disable user.
            virtual ErrorId set_UserStatus(CSTR userId, bool IsActive) = 0;
            // update current user roles 
            virtual SE_CODE set_UserRoles(UID userId, UserRole roles) = 0;

            ///////////////////////////// User's session management ////////////////////////////
            // Utilizes underlying session instance to record user's session activity.
            virtual SE_CODE RecordSessionActivity() = 0;
            
            // get auditor 
            virtual RP<IAudit> GetAuditor() = 0 ;
            
            
            /********************Global methods************************/
            virtual SE_CODE get_Countries(ICountryList **ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& )> criteria = nullptr) = 0;
            virtual SE_CODE get_Countries(Json::Value& jsonOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& )> criteria = nullptr) = 0;
            virtual SE_CODE get_Country(ICountry** ppOut, std::function<CSTR (ICountry::QUERYFLDS, LGX&, BOOL& )> criteria = nullptr) = 0;
            virtual SE_CODE get_Country(Json::Value& outVal, CSqlAst* criteria) = 0;
            
            virtual SE_CODE get_States(IStateList** ppOut, std::function<CSTR (IState::QUERYFLDS, LGX&, BOOL& )> criteria = nullptr) = 0;
            virtual SE_CODE get_States(Json::Value& jsonOut, CSqlAst* criteria = nullptr) = 0;
            virtual SE_CODE get_State(Json::Value& jsonOut, CSqlAst* criteria = nullptr) = 0;
            /**********************Exchanges and Fees*************************/
            /*
             *  Returns exchange code, name and fee
             * @vendorSources (in): ['VENDOR1', 'VENDOR2']
             * @jsOut (out): [{ key: 99, code : 'xx', name: 'xxxxx', fee: 99.99 }]
             * 
             */
            virtual SE_CODE get_ExchangePackages(Json::Value& jsOut) = 0;
            
            
            /*************** busines operations **************/
            // Payment related
            virtual ErrorId get_PaypalExpressCheckOut(IPaypalExpressCheckout** ppOut) = 0;
            
            // Products
            // get product.
            virtual ErrorId get_Product(UID id, IProduct** ppOut) = 0;
            // set product
            virtual ErrorId set_Product(IProduct* pIn) = 0;
            // get list of products
            virtual ErrorId get_Products(IROSPList<IProduct>** ppOut) = 0;
            
            
            virtual SE_CODE get_Porfolios(UID ownerId, IPortfolioList** ppOut) = 0;

            virtual SE_CODE get_Porfolio(U32 portfolioId, IPortfolio** ppOut) = 0;
            
            virtual SE_CODE get_Positions(UID ownerId, IPositions** ppOut) = 0;
            
            // Help related methods.
            virtual RP<IHelpDocTree> get_HelpDocTree() = 0;
            
        };
        
        extern "C"
        {
            ErrorId ADMEXP AdminOpen(ISession* ps, IAdminAPI** ppGateway) ;
        }        
    }
}

#endif	/* SEADMINAPI_H */

