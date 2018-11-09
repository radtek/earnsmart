/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// types ///
RegStages = {
    None : 256,
    UserTypeAndAddress : 0,
    SendEmailForConfirmation : 1,
    EmailConfirmed : 2,
    PaymentProvided : 3,
    RegistrationCompleted : 4
};



/* ANGULAR JS STUFF */

var earnsmartApp = angular.module(
    "earnsmartApp", 
    ['ngRoute', 'ngResource', 'ngCookies']
);


function fJsonRpcServices($http, $log)
{
    var obj = new Object();
    obj.createObj = function(methodName, id)
    {
        var r = {
            "jsonrpc" : "2.0",
            "method" : methodName,
            "id" : id,
            "params" : []
        };
        return r;
    };
    
    obj.post = function(url, msg, fstatus)
    {
        $http({ method: "POST", url : url, data : angular.toJson(msg) , "headers": {'Content-Type': 'application/jsonrpc'} })
        .then(
            function onSuccess(response)
            {
                $log.log(response);
                if (response.data !== null)
                {
                    if (!angular.isUndefined(response.data.error) && response.data.error !== null)
                    {
                        (fstatus || angular.noop)(null, response.data.error);
                    }
                    else
                    {
                        (fstatus || angular.noop)(response.data, null);
                    }
                }
                else
                {
                    (fstatus || angular.noop)(null, { message: 'RPC responsed reported missing data.' } );
                }
            }, 
            function onError(response)
            {
                (fstatus || angular.noop)(null, {message: response} );
            }
        );
    };

    return obj;
}

function fAppServices($http, $rootScope, $interval, $log, jsonRpcServices)
{
    $log.log("AppService is loading...");
    
    $rootScope.appData = {
        MarketdataVendor : "",
        Version : ""
    };

    var defSession = 
    {
        // user session instance.
        isauthenticated : false,
        firstName : "",
        lastName : "",
        role : "anonymous",
        regstage : 0
    };
    
    $rootScope.session = defSession;
    
    var obj = 
    {
        Title : "Earnsmart - Path to successful security investment",
        broadcast : function(type, msg)
        {
            $rootScope.$broadcast(type, msg);
        }
    };

    obj.verifySession = function( fncallback )
    {
        var odata = jsonRpcServices.createObj("verifysession", 999);
        odata.params = [];
        jsonRpcServices.post
        (
            "/rpc/admin", 
            odata,
            function onreply(response, error)
            {
                $log.log(['verifySession', response, error]);
                if (error === null)
                {
                    $rootScope.session = response["result"];
                    obj.broadcast("appServiceUpdate", obj);
                    if (fncallback != null)
                    {
                        fncallback($rootScope.session);
                    }
                }
                else
                {
                    $rootScope.session = defSession;
                    obj.broadcast("appServiceUpdate", obj);
                    if (fncallback != null)
                    {
                        fncallback($rootScope.session);
                    }
                }
            }
        );
    };
    
    // populate applicaiton data
    obj.requestAppData = function()
    {
        var odata = jsonRpcServices.createObj("getappdata", 998);
        odata.params = [];
        jsonRpcServices.post(
            "/rpc",
            odata,
            function onreply(response, error)
            {
                if (error !== null)
                {
                    $rootScope.appData = response.result;
                }
            }
        );
    };

    obj.requestAppData();
    obj.verifySession(null);
    
    return obj;
};


function fUserServices($http, $location, $log, jsonRpcServices)
{
    var us = new Object();
    
    us.login = function(userInfo, fstatus)
    {
        $log.log(userInfo.emailAddress + " wants to login...");
        var odata = jsonRpcServices.createObj("login", 1000);
        odata.params = [{"emailAddress" : userInfo.emailAddress, "pwd" : userInfo.pwd, "remember" : userInfo.rememberMe}];
        jsonRpcServices.post
        (
            "/rpc/admin", 
            odata, 
            fstatus
        );
    };
    
    us.register = function(userInfo, fstatus)
    {
        $log.log("register is called");
        var odata = jsonRpcServices.createObj("register", 1005);
        odata.params = [ userInfo ];
        jsonRpcServices.post(
            "/rpc/admin",
            odata,
            fstatus
        );
        $log.log("registration info is sent.");
    };
    
    us.confirmEmail = function(esid, fstatus)
    {
        var odata = jsonRpcServices.createObj("confirmemail", 1006);
        odata.params = [ esid ];
        jsonRpcServices.post(
            "/rpc/admin", 
            odata,
            fstatus
        );
    };
    
    us.resetPwd = function(userInfo, fstatus)
    {
        $log.log("resetPwd is called");
        var odata = jsonRpcServices.createObj("resetpwd", 1001);
        odata.params = [ userInfo.emailAddress ];
        jsonRpcServices.post
        (
            "/rpc/admin", 
            odata, 
            fstatus
        );
    };
    
    return us;
}

function fCommonServices($http, $log, jsonRpcServices)
{
    var obj = new Object();
    obj.getCountries = function(fstatus)
    {
        $log.log("Fetching countries...");
        var odata = jsonRpcServices.createObj("countries", 1002);
        odata.params = [];
        jsonRpcServices.post("/rpc/common", odata, fstatus);
        $log.log("Request is sent for countries.");
    };
    obj.getStates = function(country, fstatus)
    {
        var odata = jsonRpcServices.createObj("states", 1003);
        odata.params = [country];
        jsonRpcServices.post("/rpc/common", odata, fstatus);
        $log.log("Request is sent for states.");
    };
    return obj;
};

function fAdminServices($http, $log, $rootScope, jsonRpcServices)
{
    var obj = new Object();
    obj.getPackageFees = function(fstatus)
    {
        $log.log("Fetching package fees...");
        var odata = jsonRpcServices.createObj("exchangepackages", 1004);
        odata.params = [$rootScope.Vendor];
        jsonRpcServices.post("/rpc/admin", odata, fstatus);
        $log.log("Request sent for exchange packages.");
    };
    return obj;
};

function MasterController($scope, appServices)
{
    $scope.Title = appServices.Title;
    $scope.session = {
        IsAuth : false,
        FirstName : "",
        LastName : "",
        RegStage : RegStages.None
    };
}

function TitleController($scope, $rootScope)
{
   $scope.title = $rootScope.Title;
}

function HeaderController($scope, $rootScope, $log, appServices)
{
    $log.log(appServices);
    
    function update()
    {
        $scope.isauth = $rootScope.session.isauthenticated;
        $scope.firstName = $rootScope.session.firstName;
    }
    
    $scope.$on("appServiceUpdate", function(event, appObj)
    {
        $log.log(appObj);
        update();
    });
    
    update();
}

function ToolbarController($scope, $http)
{
    $http.get("txt/menus.json").success(function(data)
    {
        $scope.menus = data.Menus;
    });
}

function ContentController($scope)
{
   $scope.master = "Earnsmart Application"; 
}

function FooterController($scope)
{
   $scope.master = "Earnsmart Application"; 
}

function HomeController($scope)
{
    if ( $scope.firstName &&  $scope.firstName.length)
        $scope.title = "Welcome " + $scope.firstName;
    else
        $scope.title = "Welcome";
}

function SignOnController($scope, $interval, $log, $location, userServices)
{
    $scope.errorMsg = "";
    $scope.emailAddress = "";
    $scope.pwd = "";
    $scope.rememberMe = false;
    
    $scope.logincall = function()
    {
        $log.log($scope);
        var userInfo = new Object();
        userInfo.emailAddress = $scope.emailAddress;
        userInfo.pwd = $scope.pwd;
        userInfo.rememberMe = $scope.rememberMe;
        userServices.login(userInfo, function onresponse(responseData, error)
        {
            if (error === null)
            {
                jsobj = angular.fromJson(responseData);
                $location.path('/home');
            }
            else
            {
                $scope.errorMsg = error.message;
                $interval(function ()
                {
                    $scope.errorMsg = "";
                }, 5000);
            }
        });
    };
}

function ResetPwdController($scope, $interval, $log, $location, userServices)
{
    $scope.emailAddress = "";
    $scope.running = false;
    $scope.disableView = function()
    {
        if ($scope.running)
        {
            return { 'pointer-events' : 'none', opacity: 0.4 };
        }
        else
        {
            return {};
        }
    };
    $scope.resetpwd = function()
    {
        $scope.running = true;
        $log.log("reset password is called.");
        var userInfo = new Object();
        userInfo.emailAddress = $scope.emailAddress;
        userServices.resetPwd(userInfo, function(responseData, error)
        {
            if (error === null)
            {
                jsobj = angular.fromJson(responseData);
                // TODO: navigate to login page.
                $log.log("resetpwd call succeeded.");
                $scope.running = true;
                $location.path('/signon');
            }
            else
            {
                $log.log(error.message);
                $scope.errorMsg = error.message;
                $interval(function ()
                {
                    $scope.errorMsg = "";
                }, 5000);
                $scope.running = false;
            }
        });
    };
}

function RegistrationController(
        $scope, 
        $routeParams, 
        $interval, 
        $log, 
        $rootScope, 
        $cookies, 
        $location,
        appServices, 
        userServices, 
        commonServices, 
        adminServices)
{
    // store all registration info at root scope
    // registration stages
    // userTypeAndAddress = 0, 
    // sendEmail = 1
    // EmailConfirmation = 2
    // Payment = 3
    // Completed = 4
    
    $scope.Countries = [];

    $scope.requestCountries = function()
    {
        commonServices.getCountries(function(response, error)
        {
            if (error === null)
            {
                $scope.Countries.push({ key: 0, name: '-- Not Selected --', ispp: false });
                angular.forEach(response.result, function(value, key)
                {
                    $scope.Countries.push( { key: value.key, name: value.name, ispp : value.ispaypalavailable } );
                });
                
                //TODO update country id using geoip
                
                $scope.userInfo.Country = 0;
            }
        });
    };
    
    $scope.requestStates = function(countrykey)
    {
        $scope.States = [{ key: 0, name: "--Not Selected--"}];
        commonServices.getStates(countrykey, function(response, error)
        {
            if (error === null)
            {
                angular.forEach(response.result, function(value, key)
                {
                    $scope.States.push( { key: value.key, name: value.name } );
                });
            }
            $scope.userInfo.State = 0;
        });
    };

    $scope.requestExchangePackages = function()
    {
        adminServices.getPackageFees(function(response, error)
        {
            $log.log(response.result);
            angular.forEach(response.result, function(value, key)
            {
                $scope.Packages.push({ key: value.key, name: value.name, description: value.description, fee: value.fee, isselected: false });
            });
        });
    };
    
    
    $scope.States = [{ key: 0, name: "--Not Selected--"}];
     
    $scope.Packages = [];
    $scope.EsId = $cookies.get("esid");
    $scope.ErrorMsg = "";
    
    $scope.userInfo = null;
        
    function initUiData()
    {
        return {
            RegStages : $rootScope.session.regstage,
            RegType : $rootScope.session.role == 'anonymous' ? 'standard' : $rootScope.session.role,
            FirsName : "",
            LastName : "",
            EmailAddress : "",
            Password : "",
            ConfirmPassword: "",
            RememberMe : false,
            Street1 : "",
            Street2 : "",
            Town : "",
            Country : 0,
            State : 0,
            StateProvince : "",
            PostalCode : ""
        };
    }
    
    $scope.userInfo = initUiData();
        
    function copySession(session, ui)
    {
        ui.RegStages = session.regstage;
        ui.RegType = session.role;
        ui.FirstName = session.firstName;
        ui.LastName = session.lastName;
        ui.emailAddress = session.userName;
    };
        
    $scope.initRegStates = function(ui)
    {
        $log.log("initRegStates called.");
        
        if (ui.RegStages > 0)
        {
            appServices.verifySession(function(session, error)
            {
                $log.log(['initRegStates', session, error]);
                copySession(session, ui);
                switch(ui.RegStages)
                {
                case 0:
                    if (ui.RegType === "licensed")
                    {
                        $log.log("initRegStates called: requesting countries...");
                        $scope.requestCountries();
                    }
                    else
                    {
                        $scope.Countries = [];
                        $scope.States = [];
                        $scope.userInfo.Town = "";
                        $scope.userInfo,PostalCode = "";
                        $scope.userInfo,Street1 = $scope.userInfo,Street2 = "";
                    }
                    break;
                default:
                    $scope.StepToNextStage(ui);
                    break;
                }
            });
        }
        else
        {
            if (ui.RegType === "licensed")
            {
                $log.log("initRegStates called: requesting countries...");
                $scope.requestCountries();
            }
            else
            {
                $scope.Countries = [];
                $scope.States = [];
                $scope.userInfo.Town = "";
                $scope.userInfo,PostalCode = "";
                $scope.userInfo,Street1 = $scope.userInfo,Street2 = "";
            }
        }
    };
    
    
    $scope.StepToNextStage = function(ui)
    {
        $log.log(['StepToNextStage', ui]);
        $scope.ErrorMsg = "";
        switch (ui.RegStages)
        {
        case 0:
            userServices.register(ui, function(response, error)
            {
                $log.log(['StepToNextStage', response, error]);
                if (error === null)
                {
                    $scope.userInfo.RegStages = 1;
                }
                else
                {
                    $log.log(error.message);
                    $scope.ErrorMsg = error.message;
                    $scope.userInfo.Password = $scope.userInfo.ConfirmPassword = "";
                }
            });
            break;
        case 1:
            {
                if (ui.RegType === "licensed")
                {
                    $scope.requestExchangePackages();
                }
                else if (ui.RegType === 'standard')
                {
                    var esid = $routeParams["regid"];
                    userServices.confirmEmail(esid, function(response, error)
                    {
                        $log.log(['confirmEmail', response, error ]);
                        if (error === null)
                        {
                            ui.RegStages = 2;
                            $scope.StepToNextStage(ui);
                        }
                        else
                        {
                            $scope.ErrorMsg = error.message;
                            $scope.userInfo = initUiData();
                        }
                        
                    });
                }
                break;
            }
        case 2:
            {
                if (ui.RegType === "licensed")
                {
                }
            }
            break;
        case 3:
            {
                $location.path('/home');
            }
            break;
        }
    };
   
    
    $scope.FeeTotal = function()
    {
        var sum = 0;
        for (var i=0; i !== $scope.Packages.length; ++i)
        {
            var package = $scope.Packages[i];
            if (package.isselected)
            {
                sum += package.fee;
            }
        }
        return sum;
    };

    $scope.initRegStates($scope.userInfo);
   
}

function PwdChangeController($scope, $interval, $log, $rootScope, userServices)
{
    
}

function AccountCloseController($scope, $interval, $log, $rootScope, userServices)
{
    
}


// Services
earnsmartApp.factory("jsonRpcServices", ['$http', '$log', fJsonRpcServices]);
earnsmartApp.factory("appServices", ['$http', '$rootScope', '$interval', '$log', 'jsonRpcServices', fAppServices]);
earnsmartApp.factory("userServices", ['$http', '$location', '$log', 'jsonRpcServices', fUserServices]);
earnsmartApp.factory("commonServices", ['$http', '$log', 'jsonRpcServices', fCommonServices]);
earnsmartApp.factory("adminServices", ['$http', '$log', '$rootScope', 'jsonRpcServices', fAdminServices]);

//Controllers
earnsmartApp.controller("MasterController", ['$scope', 'appServices', MasterController]);
earnsmartApp.controller("TitleController", ['$scope', '$rootScope', TitleController]);
earnsmartApp.controller("HeaderController", ['$scope', '$rootScope', '$log', 'appServices', HeaderController]);
earnsmartApp.controller("ToolbarController", ['$scope', '$http', ToolbarController]);
earnsmartApp.controller("ContentController", ['$scope', ContentController]);
earnsmartApp.controller("FooterController", ['$scope', FooterController]);
earnsmartApp.controller("HomeController", ['$scope', HomeController]);
earnsmartApp.controller("SignOnController", ['$scope','$http', '$interval', '$log', 'userServices', SignOnController]);
earnsmartApp.controller("ResetPwdController", ['$scope', '$interval', '$log', 'userServices', ResetPwdController]);
earnsmartApp.controller("RegistrationController", ['$scope', '$routeParams', '$interval', '$log', '$rootScope', '$cookies', 'appServices', 'userServices', 'commonServices', 'adminServices', RegistrationController]);


earnsmartApp.config(['$routeProvider', 
    function($routeProvider)
    {
        $routeProvider
            .when('/home', {
                templateUrl: 'views/home.html',
                controller: HomeController
            })
            .when('/signon', {
                templateUrl: 'views/signon.html',
                controller: SignOnController
            })
            .when('/resetpwd', {
                templateUrl: 'views/resetpwd.html',
                controller: ResetPwdController
            })
            .when('/register', {
                templateUrl: 'views/registration.html',
                controller: RegistrationController
            })
            .when('/register/:regid', {
                templateUrl: 'views/registration.html',
                controller: RegistrationController
            })
            .when('/changepwd', {
                templateUrl: 'views/pwd.html',
                controller: PwdChangeController
            })
            .when('/closeaccount', {
                templateUrl: 'views/closeaccount.html',
                controller: AccountCloseController
            })
            .otherwise({
                redirectTo: '/home'
            });
    }
]);

earnsmartApp.run(['$rootScope', 
    function($rootScope)
    {
        $rootScope.session = {};
    }
]);

/*
earnsmartApp.factory("NavMenus", ['$resource', 
    function($resource)
    {
        console.log("query menus....");
        return $resource("txt/menus.json", {}, { query: { method : 'GET', isArray:false} } );
    }
]);
*/