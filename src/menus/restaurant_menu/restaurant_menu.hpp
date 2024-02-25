#ifndef RESTAURANT_MENU
#define RESTAURANT_MENU

#include <menus/menus.hpp>
#include <main.hpp>

class restaurant
{
    public:
        std::string name;
        std::string id;
        gui_text* txt = 0;
        gui_image* btn_img = 0;
        gui_image* btn_hover_img = 0;
        gui_button* btn = 0;

        restaurant(const std::string& name, const std::string& id)
        {
            this->name = name;
        }
};

class restaurant_menu
{
    public:
        static menus::state update();
};

#endif /* RESTAURANT_MENU */
