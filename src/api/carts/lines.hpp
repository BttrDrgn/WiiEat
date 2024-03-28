#ifndef LINES
#define LINES

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class lines
{
public:
    std::string menu_item_id;
    std::string brand;
    std::vector<std::string> experiments;
    int quantity;
    std::string special_instructions;
    std::vector<json> options;
    std::vector<int> sub_option_ids;
    double cost;
    std::string restaurant_id;
    bool popular;
    bool is_badged;
    std::string source;

    // Constructor with default values
    lines(const std::string& menu_item_id, int count, double cost, const std::string& store_id) :
        menu_item_id(menu_item_id),
        brand("GRUBHUB"),
        experiments({"LINEOPTION_ENHANCEMENTS"}),
        quantity(count),
        special_instructions(""),
        options({}),
        sub_option_ids({}),
        cost(cost),
        restaurant_id(store_id),
        popular(false),
        is_badged(false),
        source("restaurant menu section_other menu categories")
    {}

    // Serialize method
    json serialize() const
    {
        return {
            {"menu_item_id", menu_item_id},
            {"brand", brand},
            {"experiments", experiments},
            {"quantity", quantity},
            {"special_instructions", special_instructions},
            {"options", options},
            {"sub_option_ids", sub_option_ids},
            {"cost", cost},
            {"restaurant_id", restaurant_id},
            {"popular", popular},
            {"is_badged", is_badged},
            {"source", source}
        };
    }

    // Static Deserialize method
    static lines deserialize(const json& root)
    {
        return lines{ };
    }
};

#endif /* LINES */
