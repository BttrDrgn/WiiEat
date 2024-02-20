#ifndef _AUTH_H_
#define _AUTH_H_

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
        cJSON* serialize() const
        {
            cJSON* root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "brand", brand.c_str());
            cJSON_AddStringToObject(root, "client_id", client_id.c_str());
            cJSON_AddStringToObject(root, "context_id", context_id.c_str());
            cJSON_AddNumberToObject(root, "device_id", device_id);
            cJSON_AddStringToObject(root, "email", email.c_str());
            cJSON_AddStringToObject(root, "password", password.c_str());
            return root;
        }

        // Static Deserialize method
        static auth deserialize(const cJSON* root)
        {
            if (!root)
            {
                // Handle error
                throw std::runtime_error("Invalid JSON object");
            }
            const char* email = cJSON_GetObjectItem(root, "email")->valuestring;
            const char* password = cJSON_GetObjectItem(root, "password")->valuestring;
            return auth{ email, password };
        }
};
#endif