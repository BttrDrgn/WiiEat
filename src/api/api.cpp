#include "api.hpp"
#include <main.hpp>
#include <geohash/geohash.h>

std::string api::access_token = "";
std::string api::ud_id = "";
std::string api::csrf_token = "";
std::string api::geohash = "";
api::coordinates api::coords;

char api::address[ADDRESS_LEN] = "";
char api::city[CITY_LEN] = "";
char api::state[STATE_LEN] = "";
char api::zip[ZIP_LEN] = "";

std::unordered_map<char*, char*> api::endpoints =
{
    { "auth", "https://api-gtm.grubhub.com/auth" },
    { "confirmation_code", "https://api-gtm.grubhub.com/auth/confirmation_code" },
    { "geocode", "https://api-gtm.grubhub.com/geocode" },
};

std::unordered_map<char*, char*> access_control =
{
    { "auth", "authorization,content-type" },
    { "confirmation_code", "" },
    { "geocode", "authorization,cache-control,if-modified-since" },
};

bool api::request_access(char* endpoint, const std::string& url, const std::string& method)
{
    std::vector<net::header> headers = 
    {
        { "Accept", "*/*"},
        { "Access-Control-Request-Headers", access_control[endpoint]},
        { "Access-Control-Request-Method", method.c_str()},
        { "Cache-Control", "max-age=0"},
    };

    auto resp = net::http_request(url, "OPTIONS", headers);
    return resp.status_code == 200;
}

bool save_token(const std::string& json)
{
    cJSON* root = cJSON_Parse(json.c_str());
    if(!root) return false;

    auto session_handle = cJSON_GetObjectItem(root, "session_handle");

    if(!session_handle)
    {
        cJSON_Delete(root);
        return false;
    }
    else
    {
        api::access_token = cJSON_GetObjectItem(session_handle, "access_token")->valuestring;
        const char* refresh_token = cJSON_GetObjectItem(session_handle, "refresh_token")->valuestring;
        if(refresh_token)
        {
            fs::write_file("sd://WiiEat/refresh_token", refresh_token);
        }

        auto claims_arr = cJSON_GetObjectItem(root, "claims");
        if (claims_arr || cJSON_IsArray(claims_arr))
        {
            auto claim_obj = cJSON_GetArrayItem(claims_arr, 0);
            api::ud_id = cJSON_GetObjectItem(claim_obj, "ud_id")->valuestring;
            if(api::ud_id != "")
            {
                fs::write_file("sd://WiiEat/ud_id", api::ud_id);
            }
        }
    }

    cJSON_Delete(root);

    return true;
}

bool save_address(const std::string& full_address, const std::string& json)
{
    fs::write_file("sd://WiiEat/address", full_address);

    auto root = cJSON_Parse(json.c_str());
    if(!root) return false;

    auto root_arr = cJSON_GetArrayItem(root, 0);
    if(!root_arr) 
    {
        cJSON_Delete(root);
        return false;
    }

    auto coordinates = cJSON_GetObjectItem(root_arr, "coordinates");
    api::coords.latitude = cJSON_GetObjectItem(coordinates, "latitude")->valuedouble;
    api::coords.longitude  = cJSON_GetObjectItem(coordinates, "longitude")->valuedouble;
    api::geohash = geohash_encode(api::coords.latitude, api::coords.longitude, 6);

    fs::write_file("sd://WiiEat/coordinates", format::va("%f %f", api::coords.latitude, api::coords.longitude));
    fs::write_file("sd://WiiEat/geohash", api::geohash);

    cJSON_Delete(root);

    return true;
}

api::error api::auth_request(char* email, char* password)
{
    auth auth_login(email, password);
    cJSON* json = auth_login.serialize();
    char* serialized = cJSON_Print(json);
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, serialized);

        if(resp.status_code == 463)
        {
            return api::error::EMAIL_2FA;
        }
        else if(resp.status_code == 200)
        {
            if(!save_token(resp.body))
            {
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
        else if(resp.status_code == 403)
        {
            return api::error::UNAUTHORIZED;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::auth_request(const char* refresh_token)
{
    auth_refresh auth_login(refresh_token);
    cJSON* json = auth_login.serialize();
    char* serialized = cJSON_Print(json);
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, serialized);

        if(resp.status_code == 463)
        {
            return api::error::EMAIL_2FA;
        }
        else if(resp.status_code == 200)
        {
            if(!save_token(resp.body))
            {
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
        else if(resp.status_code == 403)
        {
            return api::error::UNAUTHORIZED;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::auth_code_request(char* email, char* code)
{
    auth_code auth_login(code, api::csrf_token, email);
    cJSON* json = auth_login.serialize();
    char* serialized = cJSON_Print(json);
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, serialized);

        if(resp.status_code == 200)
        {
            if(!save_token(resp.body))
            {
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
        else if(resp.status_code == 401)
        {
            return api::error::UNAUTHORIZED;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::confirmation_code_request(char* email)
{
    confirmation_code conf_code(email);
    cJSON* json = conf_code.serialize();
    char* serialized = cJSON_Print(json);
    net::response resp;

    char* endpoint = "confirmation_code";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
            { "Access-Control-Request-Headers", "authorization,content-type"},
            { "Access-Control-Request-Method", "POST" },
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, serialized);

        if(resp.status_code == 200)
        {
            auto root = cJSON_Parse(resp.body.c_str());
            if(!root) return api::error::UNKNOWN;

            api::csrf_token = cJSON_GetObjectItem(root, "csrf_token")->valuestring;

            return api::error::NONE;
        }
    }
    
    return api::error::UNKNOWN;
};


api::error api::geocode_request(char* address, char* city, char* state, char* zip)
{
    char* endpoint = "geocode";
    auto full_address = format::va("%s, %s, %s %s", address, city, state, zip);
    auto url = format::va("%s?address=%s", api::endpoints[endpoint], full_address.c_str());

    if(api::request_access(endpoint, url, "GET"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", format::va("Bearer %s", api::access_token.c_str()).c_str() },
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            if(!save_address(full_address, resp.body))
            {
                return api::error::BAD_JSON;
            }
            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::restaurants_request()
{
    /*
    https://api-gtm.grubhub.com/restaurants/search/search_listing?
    orderMethod=delivery
    &locationMode=DELIVERY
    &facetSet=umamiV6
    &pageSize=0
    &hideHateos=true
    &searchMetrics=true
    &location=POINT({lat} {lng})
    &preciseLocation=true
    &geohash={geohash}
    &sortSetId=umamiv3
    &countOmittingTimes=true
    */


}

api::img_data api::download_image(const std::string& url)
{
    api::img_data img = { 0, 0 };

    auto resp = net::http_request(url, "GET", {});
    if(resp.status_code == 200)
    {
        size_t total_size = resp.body.size();
        img.data = (uint8_t*)malloc(total_size);

        if(img.data)
        {
            memcpy(img.data, resp.body.data(), total_size);
            img.size = total_size;
        }
    }

    return img;
}

bool api::is_address_complete()
{
    return !strcmp(api::address, "") && !strcmp(api::city, "") && !strcmp(api::state, "") && !strcmp(api::zip, "") && api::geohash != "";
}

std::string api::get_full_address()
{
    return format::va("%s, %s, %s %s", api::address, api::city, api::state, api::zip);
}