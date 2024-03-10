#ifndef CONFIRMATION_CODE
#define CONFIRMATION_CODE

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class confirmation_code
{
public:
    std::string brand;
    std::string client_id;
    std::string email;

    // Constructor with default values
    confirmation_code(const std::string& email) :
        brand("GRUBHUB"),
        client_id("beta_UmWlpstzQSFmocLy3h1UieYcVST"),
        email(email)
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"brand", brand},
            {"client_id", client_id},
            {"email", email}
        };
    }

    // Static Deserialize method
    static confirmation_code deserialize(const json& root)
    {
        const std::string email = root.at("email").get<std::string>();
        return confirmation_code{email};
    }
};

#endif /* CONFIRMATION_CODE */
