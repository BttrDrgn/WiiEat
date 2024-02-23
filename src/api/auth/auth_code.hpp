#ifndef _AUTH_CODE_H_
#define _AUTH_CODE_H_

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
        cJSON* serialize() const
        {
            cJSON* root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "brand", brand.c_str());
            cJSON_AddStringToObject(root, "client_id", client_id.c_str());
            cJSON_AddStringToObject(root, "confirmation_code", confirmation_code.c_str());
            cJSON_AddStringToObject(root, "csrf_token", csrf_token.c_str());
            cJSON_AddNumberToObject(root, "device_id", device_id);
            cJSON_AddStringToObject(root, "email", email.c_str());
            return root;
        }

        // Static Deserialize method
        static auth_code deserialize(const cJSON* root)
        {
            if (!root)
            {
                // Handle error
                throw std::runtime_error("Invalid JSON object");
            }
            const char* confirmation_code = cJSON_GetObjectItem(root, "confirmation_code")->valuestring;
            const char* csrf_token = cJSON_GetObjectItem(root, "csrf_token")->valuestring;
            const char* email = cJSON_GetObjectItem(root, "email")->valuestring;
            return auth_code{ confirmation_code, csrf_token, email };
        }
};
#endif