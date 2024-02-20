#ifndef _API_H_
#define _API_H_

#include <unordered_map>
#include <cjson/cJSON.h>
#include <net/net.hpp>
#include <api/auth/auth.hpp>
#include <api/auth/auth_refresh.hpp>

class api
{
    public:
        static std::string access_token;
        static std::unordered_map<char*, char*> endpoints;
        static bool request_access(char* endpoint, char* query);
        static net::response request(char* endpoint, char* query, char* method);

        static bool auth_request(char* email, char* password);
        static bool auth_request(const char* refresh_token);
};

#endif