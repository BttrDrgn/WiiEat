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
        static bool load_store(const std::string& id);
        static void unload_store();
        static menus::state update();
        static bool loaded;

    private:
        static json json;
};

#endif /* STORE_MENU */
