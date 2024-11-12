#include "api.hpp"
#include "restaurant.hpp"
#include <main.hpp>
#include <geohash/geohash.h>

std::string api::access_token = "";
std::string api::ud_id = "";
std::string api::csrf_token = "";
std::string api::geohash = "";
api::coordinates api::coords;
std::string api::operation_id = "";
int api::tz_offset = 0;
int api::device_id = 0;
std::string api::cart_id;
std::string api::locked_store_id;
std::string api::locked_store_name;

char api::address[ADDRESS_LEN] = "";
char api::city[CITY_LEN] = "";
char api::state[STATE_LEN] = "";
char api::zip[ZIP_LEN] = "";

std::unordered_map<char*, char*> api::endpoints =
{
    { "auth", "https://api-gtm.grubhub.com/auth" },
    { "confirmation_code", "https://api-gtm.grubhub.com/auth/confirmation_code" },
    { "geocode", "https://api-gtm.grubhub.com/geocode" },
    { "restaurants", "https://api-gtm.grubhub.com/restaurants/search" },
    { "info_nonvolatile", "https://api-gtm.grubhub.com/restaurant_gateway/info/nonvolatile" },
    { "info_volatile", "https://api-gtm.grubhub.com/restaurant_gateway/info/volatile" },
    { "feed", "https://api-gtm.grubhub.com/restaurant_gateway/feed" },
    { "menu_item", "https://api-gtm.grubhub.com/restaurants/{resId}/menu_items" },
    { "carts", "https://api-gtm.grubhub.com/carts" },
    { "delivery_info", "https://api-gtm.grubhub.com/carts" },
    { "incomplete_delivery", "https://api-gtm.grubhub.com/carts/{cartId}/incomplete_delivery" },
    { "lines", "https://api-gtm.grubhub.com/carts/{cartId}/lines" },
    { "client_token", "https://api-gtm.grubhub.com/payments/client_token" }, //{"payment_type":"CREDIT_CARD","frontend_capabilities":[]}
    { "credit_card", "https://api-cde-gtm.grubhub.com/tokenizer/{token}/credit_card" }, //{"credit_card_number":"","cvv":"","expiration_month":"","expiration_year":"","cc_zipcode":"","vaulted":true}
    { "credentials", "https://api-gtm.grubhub.com/credentials/" },
    { "payments", "https://api-gtm.grubhub.com/payments/{udId}/payments" },
};

std::unordered_map<char*, char*> access_control =
{
    { "auth", "authorization,content-type" },
    { "confirmation_code", "" },
    { "geocode", "authorization,cache-control,if-modified-since" },
    { "restaurants", "authorization,cache-control,if-modified-since" },
    { "info_volatile", "authorization,cache-control,if-modified-since" },
    { "feed", "authorization,cache-control,if-modified-since" },
    { "menu_item", "authorization,cache-control,if-modified-since" },
    { "carts", "authorization,cache-control,content-type,if-modified-since" },
    { "incomplete_delivery", "authorization,cache-control,content-type,if-modified-since" },
    { "lines", "authorization,cache-control,content-type,if-modified-since" },
    { "delivery_info", "authorization,cache-control,content-type,if-modified-since" },
    { "client_token", "authorization,cache-control,content-type,if-modified-since" },
    { "credit_card", "content-type" },
    { "credentials", "content-type" },
    { "payments", "authorization,cache-control,if-modified-since" },
};

bool api::request_access(char* endpoint, const std::string& url, const std::string& method)
{
    std::vector<net::header> headers = 
    {
        { "Accept", "*/*"},
        { "Access-Control-Request-Headers", access_control[endpoint]},
        { "Access-Control-Request-Method", method.c_str()},
    };

    auto resp = net::http_request(url, "OPTIONS", headers);
    return resp.status_code == 200;
}

bool save_token(const std::string& json)
{
    try
    {
        auto root = json::parse(json);

        auto session_handle = root.find("session_handle");
        if (session_handle == root.end())
        {
            return false;
        }
        else
        {
            api::access_token = (*session_handle)["access_token"].get<std::string>();
            auto refresh_token = (*session_handle).find("refresh_token");
            if (refresh_token != session_handle->end())
            {
                io::write_file("sd://WiiEat/refresh_token", refresh_token->get<std::string>());
            }

            auto claims_arr = root.find("claims");
            if (claims_arr != root.end() && claims_arr->is_array() && !claims_arr->empty())
            {
                auto claim_obj = (*claims_arr)[0];
                api::ud_id = claim_obj["ud_id"].get<std::string>();
                if (!api::ud_id.empty())
                {
                    io::write_file("sd://WiiEat/ud_id", api::ud_id);
                }
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        // Handle exception (e.g., JSON parsing error)
        return false;
    }
}

bool save_address(const std::string& full_address, const std::string& json)
{
    io::write_file("sd://WiiEat/address", full_address);

    try
    {
        auto root = json::parse(json);

        if (root.empty())
        {
            console_menu::write_line("unable to parse geocode json");
            return false;
        }

        auto root_arr = root.at(0);
        if (root_arr.empty())
        {
            console_menu::write_line("unable to find first element in root array");
            return false;
        }

        auto temp_lat = root_arr["latitude"].get<std::string>();
        auto temp_lng = root_arr["longitude"].get<std::string>();

        if (!format::check_is_double(temp_lat, api::coords.latitude) || !format::check_is_double(temp_lng, api::coords.longitude))
        {
            console_menu::write_line("failed to parse coordinates");
            return false;
        }

        io::write_file("sd://WiiEat/coordinates", format::va("%f %f", api::coords.latitude, api::coords.longitude));

        api::geohash = geohash_encode(api::coords.latitude, api::coords.longitude, 12);
        io::write_file("sd://WiiEat/geohash", api::geohash);

        api::tz_offset = root_arr["time_zone"]["raw_offset"].get<int>();
        io::write_file("sd://WiiEat/tz_offset", std::to_string(api::tz_offset));

        return true;
    }
    catch (const std::exception& e)
    {
        // Handle exception (e.g., JSON parsing error)
        console_menu::write_line(e.what());
        return false;
    }
}

api::error api::auth_request(char* email, char* password)
{
    auth auth_login(device_id, email, password);
    auto json = auth_login.serialize();
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, json.dump());

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
    auto json = auth_login.serialize();
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, json.dump());

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
    auto json = auth_login.serialize();
    net::response resp;

    char* endpoint = "auth";
    if(api::request_access(endpoint, api::endpoints[endpoint], "POST"))
    {
        std::vector<net::header> headers = 
        {
            { "Accept", "*/*"},
        };

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, json.dump());

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
    auto json = conf_code.serialize();
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

        resp = net::http_request(api::endpoints[endpoint], "POST", headers, json.dump());

        if (resp.status_code == 200)
        {
            try
            {
                auto json = nlohmann::json::parse(resp.body);
                api::csrf_token = json["csrf_token"].get<std::string>();
                return api::error::NONE;
            }
            catch (const std::exception& e)
            {
                console_menu::write_line(e.what());
                return api::error::BAD_JSON;
            }
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
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
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

api::error api::restaurants_request(json& json)
{
    /*
    https://api-gtm.grubhub.com/restaurants/search
    ?orderMethod=delivery
    &locationMode=DELIVERY
    &facetSet=umamiV6
    &pageSize=36
    &hideHateos=true
    &searchMetrics=true
    &location=POINT(lng lat)
    &preciseLocation=true
    &geohash=djfn99ju3dej
    &includeOffers=true
    &sortSetId=umamiv3
    &sponsoredSize=3
    &countOmittingTimes=true
    &tab=all
    */

    //yes lat lng is reversed
    char* endpoint = "restaurants";
    auto url = format::va("%s?orderMethod=delivery&locationMode=DELIVERY&facetSet=umamiV6&pageSize=36&hideHateos=true&searchMetrics=true&location=POINT(%f %f)&preciseLocation=true&geohash=%s&includeOffers=true&sortSetId=umamiv3&sponsoredSize=3&countOmittingTimes=true&tab=all",
        api::endpoints[endpoint],
        api::coords.longitude, api::coords.latitude,
        api::geohash.c_str()
    );

    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch (const std::exception& e)
            {
                console_menu::write_line(e.what());
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

api::error api::restaurant_info_request(const std::string& id, json& json)
{
    /*
    https://api-gtm.grubhub.com/restaurant_gateway/info/nonvolatile/{id}
    ?orderType=STANDARD
    &platform=WEB
    &enhancedFeed=true
    &location=POINT(lng lat)
    */

    char* endpoint = "info_volatile";
    auto url = format::va("%s/%s?orderType=STANDARD&platform=WEB&enhancedFeed=true&location=POINT(%f %f)", api::endpoints[endpoint], id.c_str(), api::coords.longitude, api::coords.latitude);

    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch (const std::exception& e)
            {
                console_menu::write_line(e.what());
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

api::error api::category_items_request(const std::string& res_id, const std::string& category_id, const std::string& op_id, json& json)
{
    char* endpoint = "feed";
    auto url = format::va("%s/%s/%s?time=%llu&location=POINT(%f %f)&operationId={opId}&isFutureOrder=false&restaurantStatus=ORDERABLE&isConvenienceMerchant=false&orderType=STANDARD&agent=false&task=CATEGORY&platform=WEB",
            api::endpoints[endpoint], res_id.c_str(), category_id.c_str(),
            io::time_now(api::tz_offset), api::coords.longitude, api::coords.latitude
    );

    //I have absolutely no idea why but whenever I va the op_id into the string it returns nothing
    //However, if i replace {opId} within the string it works fine; if i print op_id it works fine;
    //if i use it somewhere else it works fine; if i make it a static variable within api class IT DOESNT WORK AT ALL???
    //genuinely confused by this
    url = format::replace(url.c_str(), "{opId}", op_id.c_str());

    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch(const std::exception& e)
            {
                console_menu::write_line(e.what());
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

api::error api::item_info_request(const std::string& res_id, const std::string& item_id, json& json)
{
    /*
    https://api-gtm.grubhub.com/restaurants/{resId}/menu_items/{itemId}
    ?time=1710451227679
    &hideUnavailableMenuItems=true
    &orderType=standard
    &version=4
    &location=POINT({lng} {lat})
    */

    char* endpoint = "menu_item";
    auto url = format::va("%s/%s",
        format::replace(api::endpoints[endpoint], "{resId}", res_id.c_str()).c_str(),
        item_id.c_str()
    );

    url = format::va("%s?time=%llu&hideUnavailableMenuItems=true&orderType=standard&version=4&location=POINT(%f %f)", url.c_str(), io::time_now(api::tz_offset), api::coords.longitude, api::coords.latitude);

    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch(const std::exception& e)
            {
                console_menu::write_line(e.what());
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::create_cart_request()
{
    try
    {
        carts* cart = new carts();
        json carts_json = cart->serialize();

        char* endpoint = "carts";
        auto url = api::endpoints[endpoint];

        if(api::request_access(endpoint, url, "POST"))
        {
            auto bearer = format::va("Bearer %s", api::access_token.c_str());
            std::vector<net::header> headers = 
            {
                { "Accept", "application/json"},
                { "Authorization", bearer.c_str()},
                { "Cache-Control", "max-age=0"},
            };

            auto resp = net::http_request(url, "POST", headers, carts_json.dump());
            if(resp.status_code == 201)
            {
                auto json = nlohmann::json::parse(resp.body);
                api::cart_id = json["id"].get<std::string>();
                api::put_incomplete_delivery();
                return api::error::NONE;
            }
        }
    }
    catch(const std::exception& e)
    {
        console_menu::write_line(e.what());
    }

    return api::error::UNKNOWN;
}

api::error api::get_cart_request(const std::string& cart_id, json& json)
{
    char* endpoint = "carts";
    auto url = format::va("%s/%s", api::endpoints[endpoint], cart_id.c_str());
    
    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch(const std::exception& e)
            {
                console_menu::write_line(e.what());
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::put_incomplete_delivery()
{
    char* endpoint = "incomplete_delivery";
    incomplete_delivery incomp_del(coords.latitude, coords.longitude);
    auto json = incomp_del.serialize();
    console_menu::write_line(json.dump().c_str());

    auto url = format::va(format::replace(api::endpoints[endpoint], "{cartId}", cart_id.c_str()).c_str());
    
    if(api::request_access(endpoint, url, "PUT"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "PUT", headers, json.dump());
        if(resp.status_code == 204)
        {
            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::add_item_request(const std::string& cart_id, const std::string& store_id, const std::string& menu_item_id, double cost)
{
    cart_lines new_line(store_id, menu_item_id, 1, cost);
    auto post_json = new_line.serialize();
    char* endpoint = "lines";
    auto url = format::va(format::replace(api::endpoints[endpoint], "{cartId}", cart_id.c_str()).c_str());
    console_menu::write_line(post_json.dump().c_str());
    
    if(api::request_access(endpoint, url, "POST"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "POST", headers, post_json.dump());
        if(resp.status_code == 201)
        {
            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
}

api::error api::get_payments(const std::string& udId, json& json)
{
    char* endpoint = "payments";
    auto url = format::va(format::replace(api::endpoints[endpoint], "{udId}", udId.c_str()).c_str());
    
    if(api::request_access(endpoint, url, "GET"))
    {
        auto bearer = format::va("Bearer %s", api::access_token.c_str());
        std::vector<net::header> headers = 
        {
            { "Accept", "application/json"},
            { "Authorization", bearer.c_str()},
            { "Cache-Control", "max-age=0"},
        };

        auto resp = net::http_request(url, "GET", headers);
        if(resp.status_code == 200)
        {
            try
            {
                json = nlohmann::json::parse(resp.body);
            }
            catch(const std::exception& e)
            {
                console_menu::write_line(e.what());
                return api::error::BAD_JSON;
            }

            return api::error::NONE;
        }
    }

    return api::error::UNKNOWN;
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
    return  (strlen(address) > 0 && strlen(city) > 0 && strlen(state) > 0 && strlen(zip) > 0 && api::geohash != "");
}

std::string api::get_full_address()
{
    return format::va("%s, %s, %s %s", api::address, api::city, api::state, api::zip);
}

bool api::load_address()
{
    if(!io::file_exists("sd://WiiEat/address")) 
    {
        console_menu::write_line("could not find sd://WiiEat/address");
        return false;
    }

    auto split_address = format::split(io::read_file("sd://WiiEat/address"), ",");
    if(split_address.size() != 3)
    {
        console_menu::write_line("split_address != 3");
        io::delete_file("sd://WiiEat/address");
        return false;
    }

    auto state_zip = format::split(split_address[2], " ");
    //Remove first space
    //ex: " , XX, 11111" -> "XX, 11111"
    state_zip.erase(state_zip.begin());
    if(state_zip.size() != 2)
    {
        console_menu::write_line("state_zip != 2");
        io::delete_file("sd://WiiEat/address");
        return false;
    }

    split_address[0].copy(api::address, ADDRESS_LEN);
    split_address[1].erase(0, 1).copy(api::city, CITY_LEN); //Removes first space between address,[]city
    state_zip[0].copy(api::state, STATE_LEN);
    state_zip[1].copy(api::zip, ZIP_LEN);

    if(!io::file_exists("sd://WiiEat/geohash")) 
    {
        console_menu::write_line("could not find sd://WiiEat/geohash");
        return false;
    }
    api::geohash = io::read_file("sd://WiiEat/geohash");

    if(!io::file_exists("sd://WiiEat/coordinates")) 
    {
        console_menu::write_line("sd://WiiEat/coordinates");
        return false;
    }

    auto split_coords = format::split(io::read_file("sd://WiiEat/coordinates"), " ");
    if(split_coords.size() != 2)
    {
        console_menu::write_line("split_coords != 2");
        io::delete_file("sd://WiiEat/coordinates");
        return false;
    }

    if( !format::check_is_double(split_coords[0], api::coords.latitude) ||
        !format::check_is_double(split_coords[1], api::coords.longitude)
    )
    {
        console_menu::write_line("split_coords != 2");
        io::delete_file("sd://WiiEat/coordinates");
        return false;
    }

    if(!io::file_exists("sd://WiiEat/tz_offset"))
    {
        console_menu::write_line("tz_offset not found");
        return false;
    }
    api::tz_offset = std::stoi(io::read_file("sd://WiiEat/tz_offset"));

    return true;
}
