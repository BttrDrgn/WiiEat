/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * menu.h
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#include <ogcsys.h>
#include "main.hpp"
#include "../menus.hpp"
#include "../console_menu/console_menu.hpp"


class main_menu
{
    public:
        static void initialize(menus::state menu);
};

#endif
