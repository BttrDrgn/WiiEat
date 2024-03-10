#ifndef RESTAURANT
#define RESTAURANT

#include <string>

class restaurant
{
    public:
        std::string name;
        std::string id;

        restaurant(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

#endif /* RESTAURANT */
