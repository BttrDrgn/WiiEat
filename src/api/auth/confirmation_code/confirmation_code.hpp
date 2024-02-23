#ifndef CONFIRMATION_CODE
#define CONFIRMATION_CODE

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

        cJSON* serialize() const
        {
            cJSON* root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "brand", brand.c_str());
            cJSON_AddStringToObject(root, "client_id", client_id.c_str());
            cJSON_AddStringToObject(root, "email", email.c_str());
            return root;
        }

        // Static Deserialize method
        static confirmation_code deserialize(const cJSON* root)
        {
            if (!root)
            {
                // Handle error
                throw std::runtime_error("Invalid JSON object");
            }
            const char* email = cJSON_GetObjectItem(root, "email")->valuestring;
            return confirmation_code{ email };
        }
};

#endif /* CONFIRMATION_CODE */
