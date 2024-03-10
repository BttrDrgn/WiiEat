#ifndef AUTH_REFRESH
#define AUTH_REFRESH

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class auth_refresh
{
public:
    std::string brand;
    std::string client_id;
    std::string context_id;
    int device_id;
    std::string refresh_token;

    // Constructor with default values
    auth_refresh(const std::string& refresh_token) :
        brand("GRUBHUB"),
        client_id("beta_UmWlpstzQSFmocLy3h1UieYcVST"),
        context_id(""),
        device_id(887744498),
        refresh_token(refresh_token)
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"brand", brand},
            {"client_id", client_id},
            {"context_id", context_id},
            {"device_id", device_id},
            {"refresh_token", refresh_token}
        };
    }

    // Static Deserialize method
    static auth_refresh deserialize(const json& root)
    {
        const std::string refresh_token = root.at("refresh_token").get<std::string>();
        return auth_refresh{ refresh_token };
    }
};

#endif /* AUTH_REFRESH */
