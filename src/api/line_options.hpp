#ifndef OPTIONS
#define OPTIONS

#include <string>
#include <vector>
#include <nlohmann/json.hpp>


class line_options
{
    public:
        std::vector<json> child_options;
        std::uint32_t id = 0;
        int quantity = 0;
        std::vector<json> sub_option_ids;

        line_options(std::uint32_t _id, int _quantity) :
            child_options({}),
            id(_id),
            quantity(_quantity),
            sub_option_ids({})
        {}

        // Serialize method
        json serialize() const
        {
            return {
                {"child_options", child_options},
                {"id", id},
                {"quantity", quantity},
                {"sub_option_ids", sub_option_ids}
            };
        }

        // Static Deserialize method
        static line_options deserialize(const json& root)
        {
            const std::uint32_t id = root.at("id").get<std::uint32_t>();
            const int quantity = root.at("quantity").get<int>();
            return line_options(id, quantity);
        }
};

#endif /* OPTIONS */
