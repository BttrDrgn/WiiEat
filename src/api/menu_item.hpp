#ifndef MENU_ITEM
#define MENU_ITEM

#include <string>

class menu_item
{
    public:
        std::string name;
        std::string id;
        std::string img_id;
        double price;

        menu_item(const std::string& name, const std::string& id, const std::string& img_id, double price)
        {
            this->name = name;
            this->id = id;
            this->img_id = img_id;
            this->price = price;
        }
};

#endif /* MENU_ITEM */