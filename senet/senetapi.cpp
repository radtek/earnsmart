/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "senetinternal.h"
#include "CCurlClient.h"
#include <jsoncpp/json/json.h>
#include "CHttpClient.h"

namespace se
{
    namespace net
    {
        SE_CODE SendEmailUsingSparkPost(const char* from, const char* to, 
                const char* subject, const char* body, IString** retMsg = nullptr);
        SE_CODE SendEmailUsingOffice365(const char* from, const char* to, 
                const char* subject, const char* body, IString** retMsg = nullptr);

        SE_CODE NETEXP SendEmail(
                EMLPROVD serviceProvider, const char* from, const char* to, 
                const char* subject, const char* body,
                IString** retMsg
                )
        {
            switch(serviceProvider)
            {
                case EMLPROVD::SparkPost:
                    return SendEmailUsingSparkPost(from, to, subject, body, retMsg);
                case EMLPROVD::Office365:
                    return SendEmailUsingOffice365(from, to, subject, body, retMsg);
                default:
                    return E_NotImpl;
            }
        }

        SE_CODE SendEmailUsingSparkPost(const char* from, const char* to, const char* subject, const char* body, IString** retMsg)
        {
            // POSTED DATA
            /*
             * {
        >       "from": "sandbox@sparkpostbox.com",
        >       "subject": "Thundercats are GO!!!",
        >       "text": "Sword of Omens, give me sight BEYOND sight"
        >     }
             */

            Json::Value data ;
            Json::Value& content = data["content"];
            content["from"] = from;
            content["subject"] = subject;
            content["html"] = body;
            Json::Value address;
            address["address"] = to;
            data["recipients"].append(address);

            CHttpClient http;
            http.EnableHostNameVerification();
            http.EnablePeerVerification();
            std::vector<string> headers;
            headers.push_back("Authorization: 5929abcb4a88d068d20568c0039700da95549c89");
            auto retCode = http.PostJson("https://api.sparkpost.com/api/v1/transmissions", data, headers, retMsg);
            // parse response
            CSTR buffer = (*retMsg)->get_Buffer();
            BDBG << buffer;
            Json::Reader reader;
            Json::Value response;
            if (!reader.parse((string)buffer, response, false))
            {
                return E_JsonParseError;
            }
            if (response.isMember("errors"))
            {
                BERR << "Sparkpost email sending error (code: " << response["errors"]["code"].asCString() << ", message: " << response["errors"]["message"].asCString() << ", desc: " << response["errors"]["description"].asCString() << ").";
                return E_HttpError;
            }
            return S_OK;
        }
        
        SE_CODE SendEmailUsingOffice365(const char* from, const char* to, const char* subject, const char* msg, IString** retMsg)
        {
            Json::Value data ;
            
            Json::Value& message = data["Message"];
            message["Subject"] = subject;
            
            Json::Value& body = message["Body"];
            body["Content"] = msg;
            body["ContentType"] = "Html";
            
            Json::Value& receipients = message["ToRecipients"];

            Json::Value emailAddr;
            emailAddr["EmailAddress"]["Address"] = to;
            
            receipients.append(emailAddr);
            
            data["SaveToSentItems"] = true;
            
            BDBG << data;
            

            CHttpClient http;
            http.EnableHostNameVerification();
            http.EnablePeerVerification();
            std::vector<string> headers;
            headers.push_back("Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsIng1dCI6IlJyUXF1OXJ5ZEJWUldtY29jdVhVYjIwSEdSTSIsImtpZCI6IlJyUXF1OXJ5ZEJWUldtY29jdVhVYjIwSEdSTSJ9.eyJhdWQiOiJodHRwczovL291dGxvb2sub2ZmaWNlLmNvbSIsImlzcyI6Imh0dHBzOi8vc3RzLndpbmRvd3MubmV0LzBhNTQ1ZGJhLTVmMjQtNDM0MC1hMGY5LTU5N2IxOGFjOTRmYS8iLCJpYXQiOjE0ODE0MDczOTEsIm5iZiI6MTQ4MTQwNzM5MSwiZXhwIjoxNDgxNDExMjkxLCJhY3IiOiIxIiwiYW1yIjpbInB3ZCJdLCJhcHBpZCI6IjMyNjEzZmM1LWU3YWMtNDg5NC1hYzk0LWZiYzM5YzlmM2U0YSIsImFwcGlkYWNyIjoiMSIsImlwYWRkciI6Ijk4LjEwOS4yMDcuMjMxIiwibmFtZSI6IlBvc3RtYXN0ZXIiLCJvaWQiOiJhODhmZWRhMi0yZjExLTQ4ZTAtYTYyMy1iYmUxNWQ2MDBhZTMiLCJwbGF0ZiI6IjMiLCJwdWlkIjoiMTAwMzdGRkU5QzlFMTVEQiIsInNjcCI6IkNhbGVuZGFycy5SZWFkV3JpdGUgQ2FsZW5kYXJzLlJlYWRXcml0ZS5TaGFyZWQgQ29udGFjdHMuUmVhZFdyaXRlIE1haWwuUmVhZFdyaXRlIE1haWwuUmVhZFdyaXRlLlNoYXJlZCBNYWlsLlNlbmQgTWFpbC5TZW5kLlNoYXJlZCBUYXNrcy5SZWFkV3JpdGUiLCJzdWIiOiJHQ3pGNk1YVEVEWWNLMEQ0bm9nb2JwTnFBODNqa1hTaFlVZXoxWE1LeWNNIiwidGlkIjoiMGE1NDVkYmEtNWYyNC00MzQwLWEwZjktNTk3YjE4YWM5NGZhIiwidW5pcXVlX25hbWUiOiJwb3N0bWFzdGVyQHNtYXJ0Z2F6ZWxsYy5jb20iLCJ1cG4iOiJwb3N0bWFzdGVyQHNtYXJ0Z2F6ZWxsYy5jb20iLCJ2ZXIiOiIxLjAifQ.cNkIKEo7p-usFx-y-nBJPT4un5z_UsDzSp20WbROq2HXCv1DZHXeB1vdXuU7o653SKOMwh9cj4hLni_hsJJWiiwmMCWxwUXVCayX8ZrFdeQIAHMiH5934iKGbdyy9DWe0Kn9HLOddA8kgIaU0EfA-wOtA3rSFeWdd9aEHFMp9WB3PZgwF9Wa19n5giHx6uXOYW6BKqPmMQZazsmb4HvieGeUBvvLMTDz-onZQ0Rtvy5Uav0YCBvn-Guc6oIH0P4Wnl3iPhzG0T2EsckemHNN7BXeEBI99CuvtqnpHt6gWWZYJW6MV-v45eHRXgAxtckUx2LMh4RlgeZ2av9TEklTbg");
            headers.push_back("Accept: application/json; odata.metadata=none");
            auto retCode = http.PostJson("https://outlook.office.com/api/v2.0/me/sendmail", data, headers, retMsg);
            
            CSTR buffer = (*retMsg)->get_Buffer();
            BDBG << buffer;
            
            if (strlen(buffer))
            {            //error message looks like this
                // {"error":{"code":"RequestBodyRead","message":"An unexpected 'StartObject' node was found for property named 'ToRecipients' when reading from the JSON reader. A 'StartArray' node was expected."}}
                Json::Reader reader;
                Json::Value response;
                if (!reader.parse((string)buffer, response, false))
                {
                    return E_JsonParseError;
                }
                if (response.isMember("errors"))
                {
                    BERR << "Sparkpost email sending error (code: " << response["errors"]["code"].asCString() << ", message: " << response["errors"]["message"].asCString() << ", desc: " << response["errors"]["description"].asCString() << ").";
                    return E_HttpError;
                }
            }
            return S_OK;            
        }
        
    }
}

