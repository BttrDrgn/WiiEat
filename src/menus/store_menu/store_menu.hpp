#ifndef STORE_MENU
#define STORE_MENU

#include <menus/menus.hpp>
#include <api/restaurant.hpp>
#include <api/category.hpp>
#include <api/menu_item.hpp>
#include <api/choice.hpp>
#include <main.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class store_menu
{
    public:
        enum view
        {
            VIEW_EXIT = -1,
            VIEW_NONE,
            VIEW_CATEGORIES,
            VIEW_ITEMS,
            VIEW_SELECTION,
        };

        static bool load_store(const std::string& store_name, const std::string& id);
        static void unload_store();
        static menus::state update();
        static view current_view;
        static std::string store_id;
        static std::string brand_uuid;
        static std::string store_name;
};

class store_selection
{
    public:
        static store_menu::view update(menus::state& menu);
        static void load_choices(const std::string& store_id, const std::string& item_id);
        static void unload_choices();
        static void update_buttons();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<choice*> choices;
        static std::vector<std::string> selected_choices;

        static int current_page;
        static int max_page;
};

#endif /* STORE_MENU */
