#include "api.hpp"
#include <main.hpp>

std::string api::access_token;

std::unordered_map<char*, char*> api::endpoints =
{
    { "auth", "https://api-gtm.grubhub.com/auth" },
    { "geocode", "https://api-gtm.grubhub.com/auth" },
};

std::vector<net::header> default_headers = 
{
    { "Accept", "*/*" },
    { "Accept-Encoding", "gzip, deflate, br" },
    { "Accept-Language", "en-US,en;q=0.9" },
    { "Access-Control-Request-Headers", "authorization,content-type,cache-control,if-modified-since" },
    { "Access-Control-Request-Method", "GET,POST" },
    { "Origin", "https://www.grubhub.com" },
    { "Referer", "https://www.grubhub.com/" },
    { "Sec-Fetch-Dest", "empty" },
    { "Sec-Fetch-Mode", "cors" },
    { "Sec-Fetch-Site", "same-site" },
    { "User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36" }
};

bool api::request_access(char* endpoint, char* query)
{
    auto resp = net::http_request(api::endpoints[endpoint], query, "OPTIONS", default_headers, nullptr);
    return resp.status_code == 200;
}

net::response api::request(char* endpoint, char* query, char* method)
{
    net::response resp;

    if(api::request_access(endpoint, query))
    {
        resp = net::http_request(api::endpoints[endpoint], query, method, default_headers, nullptr);
    }

    return resp;
}

bool api::auth_request(char* email, char* password)
{
    auth auth_login(email, password);
    cJSON* json = auth_login.serialize();
    char* serialized = cJSON_Print(json);
    auto resp = api::request("auth", serialized, "POST");

    if(resp.status_code == 200)
    {
        auto root = cJSON_Parse(resp.body.c_str());
        if(!root) return false;

        auto session_handle = cJSON_GetObjectItem(root, "session_handle");
        if(!session_handle)
        {
            cJSON_Delete(root);
            return false;
        }
        
        api::access_token = cJSON_GetObjectItem(session_handle, "access_token")->valuestring;
        fs::write_file("sd://WiiEat/refresh_token", cJSON_GetObjectItem(session_handle, "refresh_token")->valuestring);
        cJSON_Delete(root);
    }
    

    return resp.status_code == 200 && api::access_token != "";
}

bool api::auth_request(const char* refresh_token)
{
    auth_refresh auth_login(refresh_token);
    cJSON* json = auth_login.serialize();
    char* serialized = cJSON_Print(json);
    auto resp = api::request("auth", serialized, "POST");

    if(resp.status_code == 200)
    {
        auto root = cJSON_Parse(resp.body.c_str());
        if(!root) return false;

        auto session_handle = cJSON_GetObjectItem(root, "session_handle");
        if(!session_handle)
        {
            cJSON_Delete(root);
            return false;
        }
        
        api::access_token = cJSON_GetObjectItem(session_handle, "access_token")->valuestring;
        fs::write_file("sd://WiiEat/refresh_token", cJSON_GetObjectItem(session_handle, "refresh_token")->valuestring);
        cJSON_Delete(root);
    }
    

    return resp.status_code == 200 && api::access_token != "";
}