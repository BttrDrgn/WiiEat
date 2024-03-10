#ifndef _AUTH_H_
#define _AUTH_H_

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class auth
{
public:
    std::string brand;
    std::string client_id;
    std::string context_id;
    int device_id;
    std::string email;
    std::string password;

    // Constructor with default values
    auth(const std::string& email, const std::string& password) :
        brand("GRUBHUB"),
        client_id("beta_UmWlpstzQSFmocLy3h1UieYcVST"),
        context_id(""),
        device_id(887744498),
        email(email),
        password(password)
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"brand", brand},
            {"client_id", client_id},
            {"context_id", context_id},
            {"device_id", device_id},
            {"email", email},
            {"password", password}
        };
    }

    // Static Deserialize method
    static auth deserialize(const json& root)
    {
        const std::string email = root.at("email").get<std::string>();
        const std::string password = root.at("password").get<std::string>();
        return auth{email, password};
    }
};

#endif
