#ifndef PAYMENT_MENU
#define PAYMENT_MENU

#include <menus/menus.hpp>
#include <main.hpp>
#include <api/api.hpp>
#include <api/credit_card.hpp>

class payment_menu
{
    public:
        static menus::state update();
        static void load_payments(const std::string& uu_id);
        static std::vector<credit_card*> credit_cards;
        static std::vector<gui_button*> buttons;
};

#endif /* PAYMENT_MENU */
