#ifndef AUTH_CODE
#define AUTH_CODE
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class auth_code
{
public:
    std::string brand;
    std::string client_id;
    std::string confirmation_code;
    std::string csrf_token;
    int device_id;
    std::string email;

    // Constructor with default values
    auth_code(const std::string& confirmation_code, const std::string& csrf_token, const std::string& email) :
        brand("GRUBHUB"),
        client_id("beta_UmWlpstzQSFmocLy3h1UieYcVST"),
        confirmation_code(confirmation_code),
        csrf_token(csrf_token),
        device_id(887744498),
        email(email)
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"brand", brand},
            {"client_id", client_id},
            {"confirmation_code", confirmation_code},
            {"csrf_token", csrf_token},
            {"device_id", device_id},
            {"email", email}
        };
    }

    // Static Deserialize method
    static auth_code deserialize(const json& root)
    {
        const std::string confirmation_code = root.at("confirmation_code").get<std::string>();
        const std::string csrf_token = root.at("csrf_token").get<std::string>();
        const std::string email = root.at("email").get<std::string>();
        return auth_code{confirmation_code, csrf_token, email};
    }
};

#endif /* AUTH_CODE */
