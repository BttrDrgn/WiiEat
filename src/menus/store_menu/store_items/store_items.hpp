#ifndef STORE_ITEMS
#define STORE_ITEMS

#include <string>
#include <vector>
#include "../store_menu.hpp"
#include <main.hpp>
#include <menus/menus.hpp>

class store_items
{
    public:
        static store_menu::view update(menus::state& menu);
        static void load_items(const std::string& store_id, const std::string& cat_id);
        static void unload_items();
        static void update_buttons();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<menu_item*> items;

        static int current_page;
        static int max_page;
};

#endif /* STORE_ITEMS */
