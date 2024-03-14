#ifndef STORE_MENU
#define STORE_MENU

#include <menus/menus.hpp>
#include <api/restaurant.hpp>
#include <api/category.hpp>
#include <api/menu_item.hpp>
#include <api/selection.hpp>
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
        };

        static bool load_store(const std::string& store_name, const std::string& id);
        static void unload_store();
        static menus::state update();
        static view next_view(view next_view);
        static view current_view;
        static view prev_view;
        static std::string store_id;
        static std::string store_name;
};

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

class store_selection
{
    public:
        static store_menu::view update(menus::state& menu);
        static void load_selection(const std::string& store_id, const std::string& item_id);
        static void unload_selection();
        static void update_buttons();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<selection*> selections;

        static int current_page;
        static int max_page;
};

#endif /* STORE_MENU */
