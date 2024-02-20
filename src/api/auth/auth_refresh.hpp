#ifndef AUTH_REFRESH
#define AUTH_REFRESH

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
        cJSON* serialize() const
        {
            cJSON* root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "brand", brand.c_str());
            cJSON_AddStringToObject(root, "client_id", client_id.c_str());
            cJSON_AddStringToObject(root, "context_id", context_id.c_str());
            cJSON_AddNumberToObject(root, "device_id", device_id);
            cJSON_AddStringToObject(root, "refresh_token", refresh_token.c_str());
            return root;
        }

        // Static Deserialize method
        static auth_refresh deserialize(const cJSON* root)
        {
            if (!root)
            {
                // Handle error
                throw std::runtime_error("Invalid JSON object");
            }
            const char* refresh_token = cJSON_GetObjectItem(root, "refresh_token")->valuestring;
            return auth_refresh{ refresh_token };
        }
};

#endif /* AUTH_REFRESH */
