#ifndef _CONSOLE_MENU_H_
#define _CONSOLE_MENU_H_

#include <string>
#include <vector>

#include "main.hpp"
#include "../menus.hpp"
#include "filelist.h"
#include "libwiigui/gui.h"
#include "libwiigui/input.h"

#define LINES 15

class console_menu
{
    public:
        static menus::state update(); 
        static std::vector<std::string> console_out;
        static bool needs_update;
        static int scroll;

        static void write_line(const std::string& msg);
        static void scroll_dir(int dir);
};

#endif