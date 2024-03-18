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
            VIEW_CHOICE,
        };

        static bool load_store(const std::string& store_name, const std::string& id);
        static void unload_store();
        static menus::state update();
        static view current_view;
        static std::string store_id;
        static std::string brand_uuid;
        static std::string store_name;
};

#endif /* STORE_MENU */
