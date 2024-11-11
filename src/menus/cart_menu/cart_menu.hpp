#ifndef CART_MENU
#define CART_MENU

#include <menus/menus.hpp>
#include <main.hpp>

class cart_menu
{
    public:
        static menus::state update();
        static void load_cart(const std::string& cart_id);
};

#endif /* CART_MENU */
