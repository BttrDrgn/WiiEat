#ifndef CONFIRMATION_CODE_RESP
#define CONFIRMATION_CODE_RESP

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class confirmation_code_resp
{
public:
    std::string status;
    std::string crsf_token;
    int code_length;
    int expiration_time;

    // Constructor with default values
    confirmation_code_resp(const std::string& status, const std::string& crsf_token, int code_length, int expiration_time) :
        status(status),
        crsf_token(crsf_token),
        code_length(code_length),
        expiration_time(expiration_time)
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"status", status},
            {"crsf_token", crsf_token},
            {"code_length", code_length},
            {"expiration_time", expiration_time}
        };
    }

    // Static Deserialize method
    static confirmation_code_resp deserialize(const json& root)
    {
        const std::string status = root.at("status").get<std::string>();
        const std::string crsf_token = root.at("crsf_token").get<std::string>();
        int code_length = root.at("code_length").get<int>();
        int expiration_time = root.at("expiration_time").get<int>();
        return confirmation_code_resp{status, crsf_token, code_length, expiration_time};
    }
};

#endif /* CONFIRMATION_CODE_RESP */
