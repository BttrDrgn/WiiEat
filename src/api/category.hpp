#ifndef CATEGORY
#define CATEGORY

#include <string>
#include "menu_item.hpp"

class category
{
    public:
        std::string name;
        std::string id;

        category(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

#endif /* CATEGORY */
