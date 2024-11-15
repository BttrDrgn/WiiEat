#ifndef CART_LINES
#define CART_LINES

#include <string>
#include <nlohmann/json.hpp>
#include <api/line_options.hpp>

using json = nlohmann::json;

class cart_lines
{
public:
    std::string menu_item_id;
    std::string brand;
    std::vector<std::string> experiments;
    int quantity;
    std::string special_instructions;
    std::vector<json> options;
    double cost;
    std::string restaurant_id;
    bool popular;
    bool is_badged;
    std::string source;

    // Constructor with default values
    cart_lines(const std::string& store_id, const std::string& menu_item_id, int count, double cost) :
        menu_item_id(menu_item_id),
        brand("GRUBHUB"),
        experiments({"LINEOPTION_ENHANCEMENTS"}),
        quantity(count),
        special_instructions(""),
        options({}),
        cost(cost),
        restaurant_id(store_id),
        popular(false),
        is_badged(false),
        source("restaurant menu section_other menu categories")
    {}

    void add_option(std::uint32_t id, int quantity)
    {
        line_options* opt = new line_options(id, quantity);
        options.emplace_back(opt->serialize());
        delete opt; 
    }

    // Serialize method
    json serialize() const
    {
        json j = {
            {"menu_item_id", menu_item_id},
            {"brand", brand},
            {"experiments", experiments},
            {"quantity", quantity},
            {"special_instructions", special_instructions},
            {"options", options},
            {"restaurant_id", restaurant_id},
            {"popular", popular},
            {"isBadged", is_badged},
            {"source", source}
        };

        if (cost == 0.0) j["cost"] = 0;
        else j["cost"] = cost;

        return j;
    }

    // Static Deserialize method
    static cart_lines deserialize(const json& root)
    {
        const std::string store_id = root.at("store_id").get<std::string>();
        const std::string menu_item_id = root.at("menu_item_id").get<std::string>();
        const int count = root.at("count").get<int>();
        const double cost = root.at("cost").get<double>();
        return cart_lines{store_id, menu_item_id, count, cost};
    }
};

#endif /* CART_LINES */
