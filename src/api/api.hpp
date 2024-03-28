#ifndef API
#define API

#include <vector>
#include <menus/restaurant_menu/restaurant_menu.hpp>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <net/net.hpp>
#include <api/auth/auth.hpp>
#include <api/auth/auth_refresh.hpp>
#include <api/auth/auth_code.hpp>
#include <api/auth/confirmation_code/confirmation_code.hpp>
#include <api/auth/confirmation_code/confirmation_code_resp.hpp>
#include <api/carts/carts.hpp>

using json = nlohmann::json;

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

        struct img_data
        {
            uint8_t* data;
            size_t size;
        };

        static char address[ADDRESS_LEN];
        static char city[CITY_LEN];
        static char state[STATE_LEN];
        static char zip[ZIP_LEN];

        static std::string access_token;
        static std::string ud_id;
        static std::string csrf_token;
        static coordinates coords;
        static std::string geohash;
        static std::unordered_map<char*, char*> endpoints;
        static std::string operation_id;
        static int tz_offset;
        static std::string cart_id;

        static bool request_access(char* endpoint, const std::string& url, const std::string& method);

        //auth
        static api::error auth_request(char* email, char* password);
        static api::error auth_request(const char* refresh_token);
        static api::error auth_code_request(char* email, char* code);
        static api::error confirmation_code_request(char* email);

        //geocode
        static api::error geocode_request(char* address, char* city, char* state, char* zip);
        static bool is_address_complete();
        static std::string get_full_address();
        static bool load_address();

        //restaurants
        static api::error restaurants_request(json& json);
        static api::error restaurant_info_request(const std::string& id, json& json);
        static api::error category_items_request(const std::string& res_id, const std::string& category_id, const std::string& op_id, json& json);
        static api::error item_info_request(const std::string& res_id, const std::string& item_id, json& json);

        //cart
        static api::error create_cart_request();
        static api::error get_cart_request(json& json);
        static api::error add_item_request(const std::string& store_id, const std::string& menu_item_id, const std::string& item_id);
        static api::error put_delivery_info();

        //download
        static img_data download_image(const std::string& url);
};

#endif /* API */
