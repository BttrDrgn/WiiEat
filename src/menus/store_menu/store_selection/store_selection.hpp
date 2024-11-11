#ifndef STORE_SELECTION
#define STORE_SELECTION

#include <string>
#include <vector>
#include "../store_menu.hpp"
#include <main.hpp>
#include <menus/menus.hpp>

class store_selection
{
    public:
        static store_menu::view update(menus::state& menu);
        static bool load_choices(const std::string& item_name, const std::string& store_id, const std::string& item_id);
        static void unload_choices();
        static void update_buttons();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<choice*> choices;
        static std::vector<std::string> selected_choices;

        static std::string current_item;
        static std::string current_item_id;
        static double current_item_cost;
        static int current_page;
        static int max_page;
};

#endif /* STORE_SELECTION */
