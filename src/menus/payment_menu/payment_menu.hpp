#ifndef PAYMENT_MENU
#define PAYMENT_MENU

#include <menus/menus.hpp>
#include <main.hpp>

class payment_menu
{
    public:
        static menus::state update();
        static void load_payments(const std::string& uuId);
};

#endif /* PAYMENT_MENU */
