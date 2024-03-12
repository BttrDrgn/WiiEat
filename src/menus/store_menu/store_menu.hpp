#ifndef STORE_MENU
#define STORE_MENU

#include <menus/menus.hpp>
#include <api/restaurant.hpp>
#include <api/category.hpp>
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

        static bool load_store(const std::string& id);
        static void unload_store();
        static menus::state update();
        static bool loaded;
        static view current_view;
        static view prev_view;

    private:
        static json json;
};

class store_category
{
    public:
        static store_menu::view update(menus::state& menu);
        static void update_buttons();
        static void next_page();
        static void prev_page();
        static std::vector<gui_button*> buttons;
        static std::vector<category*> categories;

        static int current_page;
        static int max_page;
};

#endif /* STORE_MENU */
