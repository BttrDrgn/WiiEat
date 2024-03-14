#ifndef MENU_ITEM
#define MENU_ITEM

#include <string>

class menu_item
{
    public:
        std::string name;
        std::string id;

        menu_item(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

#endif /* MENU_ITEM */