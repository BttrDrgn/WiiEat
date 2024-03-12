#ifndef RESTAURANT_MENU
#define RESTAURANT_MENU

#include <menus/menus.hpp>
#include <api/restaurant.hpp>
#include <main.hpp>

class restaurant_menu
{
    public:
        static menus::state update();
        static std::vector<restaurant*> restaurants;
        static void refresh();
};

#endif /* RESTAURANT_MENU */
