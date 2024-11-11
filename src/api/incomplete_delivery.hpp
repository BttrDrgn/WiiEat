#ifndef INCOMPLETE_DELIVERY
#define INCOMPLETE_DELIVERY
#include <string>
#include <nlohmann/json.hpp>

class incomplete_delivery
{
    public:
        std::string latitude;
        std::string longitude;

        // Constructor with default values
        incomplete_delivery(double latitude, double longitude) :
            latitude(std::to_string(latitude)),
            longitude(std::to_string(longitude))
        {}

        incomplete_delivery(const std::string& latitude, const std::string& longitude) :
            latitude(latitude),
            longitude(longitude)
        {}

        // Serialize method
        json serialize() const
        {
            return {
                {"latitude", latitude},
                {"longitude", longitude},
            };
        }

        // Static Deserialize method
        static incomplete_delivery deserialize(const json& root)
        {
            const std::string lat = root.at("latitude").get<std::string>();
            const std::string lng = root.at("longitude").get<std::string>();
            return incomplete_delivery{lat, lng};
        }
};

#endif /* INCOMPLETE_DELIVERY */
