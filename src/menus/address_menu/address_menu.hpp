#ifndef ADDRESS_MENU
#define ADDRESS_MENU

#include <menus/menus.hpp>
#include <main.hpp>

#define ADDRESS_LEN 50
#define CITY_LEN 50
#define STATE_LEN 3
#define ZIP_LEN 50

class address_menu
{
    public:
        static char address[ADDRESS_LEN];
        static char city[CITY_LEN];
        static char state[STATE_LEN];
        static char zip[ZIP_LEN];

        static menus::state update();
};

#endif /* ADDRESS_MENU */
