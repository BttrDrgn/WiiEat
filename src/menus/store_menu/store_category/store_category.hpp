#ifndef STORE_CATEGORY
#define STORE_CATEGORY

#include <string>
#include <vector>
#include "../store_menu.hpp"
#include <main.hpp>
#include <menus/menus.hpp>

class store_category
{
    public:
        static store_menu::view update(menus::state& menu);
        static void update_buttons();
        static void load_category();
        static void unload_category();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<category*> categories;
        static std::string category_name;   

        static int current_page;
        static int max_page;
};

#endif /* STORE_CATEGORY */
