#ifndef CONFIRMATION_CODE_RESP
#define CONFIRMATION_CODE_RESP

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

    cJSON* serialize() const
    {
        cJSON* root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "status", status.c_str());
        cJSON_AddStringToObject(root, "crsf_token", crsf_token.c_str());
        cJSON_AddNumberToObject(root, "code_length", code_length);
        cJSON_AddNumberToObject(root, "expiration_time", expiration_time);
        return root;
    }

    // Static Deserialize method
    static confirmation_code_resp deserialize(const cJSON* root)
    {
        if (!root)
        {
            // Handle error
            throw std::runtime_error("Invalid JSON object");
        }
        const char* status = cJSON_GetObjectItem(root, "status")->valuestring;
        const char* crsf_token = cJSON_GetObjectItem(root, "crsf_token")->valuestring;
        int code_length = cJSON_GetObjectItem(root, "code_length")->valueint;
        int expiration_time = cJSON_GetObjectItem(root, "expiration_time")->valueint;
        return confirmation_code_resp{ status, crsf_token, code_length, expiration_time };
    }
};

#endif /* CONFIRMATION_CODE_RESP */
