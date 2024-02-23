#ifndef API
#define API

#include <unordered_map>
#include <cjson/cJSON.h>
#include <net/net.hpp>
#include <api/auth/auth.hpp>
#include <api/auth/auth_refresh.hpp>
#include <api/auth/auth_code.hpp>
#include <api/auth/confirmation_code/confirmation_code.hpp>
#include <api/auth/confirmation_code/confirmation_code_resp.hpp>

#define ADDRESS_LEN 50
#define CITY_LEN 50
#define STATE_LEN 3
#define ZIP_LEN 50   

class api
{
    public:
        enum error
        {
            UNKNOWN,
            NONE,
            BAD_JSON,
            UNAUTHORIZED,
            EMAIL_2FA
        };

        struct coordinates
        {
            double latitude;
            double longitude;
        };

        static char address[ADDRESS_LEN];
        static char city[CITY_LEN];
        static char state[STATE_LEN];
        static char zip[ZIP_LEN];

        static std::string access_token;
        static std::string ud_id;
        static std::string csrf_token;
        static coordinates coords;
        static std::unordered_map<char*, char*> endpoints;

        static bool request_access(const std::string& url, const std::string& method);

        //auth
        static api::error auth_request(char* email, char* password);
        static api::error auth_request(const char* refresh_token);
        static api::error auth_code_request(char* email, char* code);
        static api::error confirmation_code_request(char* email);

        //geocode
        static api::error geocode_request(char* address, char* city, char* state, char* zip);
};

#endif /* API */
