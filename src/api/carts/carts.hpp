#ifndef CARTS
#define CARTS

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class carts
{
public:
    std::string brand;
    std::vector<std::string> experiments;
    std::vector<std::string> cart_attributes;

    // Constructor with default values
    carts() :
        brand("GRUBHUB"),
        experiments({"IGNORE_MINIMUM_TIP_REQUIREMENT","LINEOPTION_ENHANCEMENTS"}),
        cart_attributes({})
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"brand", brand},
            {"experiments", experiments},
            {"cart_attributes", cart_attributes},
        };
    }

    // Static Deserialize method
    static carts deserialize(const json& root)
    {
        return carts{ };
    }
};

#endif /* CARTS */
