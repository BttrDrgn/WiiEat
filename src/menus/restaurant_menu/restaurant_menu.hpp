#ifndef RESTAURANT_MENU
#define RESTAURANT_MENU

#include <menus/menus.hpp>
#include <main.hpp>

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

class restaurant_menu
{
    public:
        static menus::state update();
};

#endif /* RESTAURANT_MENU */
