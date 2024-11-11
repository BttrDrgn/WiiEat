#include "store_menu.hpp"
#include "store_category/store_category.hpp"
#include "store_items/store_items.hpp"
#include "store_selection/store_selection.hpp"

store_menu::view store_menu::current_view = store_menu::view::VIEW_NONE;
std::string store_menu::store_id;
std::string store_menu::brand_uuid;
std::string store_menu::store_name = "";
std::initializer_list<std::string> cat_blacklist = 
{
	"Disclaimers",
	"Similar options nearby",
};

bool store_menu::load_store(const std::string& store_name, const std::string& store_id)
{
	json categories_json = 0;
	auto err = api::restaurant_info_request(store_id, categories_json);
	if(err != api::error::NONE)
	{
		if(err == api::error::UNAUTHORIZED)
		{
			menus::unauthorized_prompt();
		}
		return false;
	}

	store_menu::store_name = format::remove_non_ascii(store_name);
	store_menu::store_id = store_id;

	try
	{
		api::operation_id = categories_json["object"]["request_id"].get<std::string>();

		auto cats = categories_json["object"]["data"]["enhanced_feed"]; //meow
		//by starting at 5, we skip the default grubhub disclaimers, search, and other categories which don't actually exist as categories
		for(int c = 5; c < cats.size(); ++c)
		{
		    auto cat = cats[c];
			auto name = format::remove_non_ascii(cat["name"].get<std::string>());
			bool found = false;
			for(auto filter : cat_blacklist)
			{
				if(name == filter) 
				{
					found = true;
					break;
				}
			}
			if(found) continue;
			store_category::categories.emplace_back(new category(name, cat["id"].get<std::string>()));
		}
		store_category::max_page = (int)ceil(store_category::categories.size() / 10.f);
	}
	catch(std::exception e)
	{
		console_menu::write_line(e.what());
		return false;
	}

	return true;
}

void store_menu::unload_store()
{
	store_menu::current_view = store_menu::view::VIEW_NONE;
	store_category::buttons.clear();
	store_category::categories.clear();
	store_menu::store_id = "";
	store_menu::store_name = "";
}

menus::state store_menu::update()
{
	auto menu = menus::state::MENU_RESTAURANT;
	store_menu::current_view = store_menu::view::VIEW_CATEGORIES;

	while(store_menu::current_view != store_menu::view::VIEW_EXIT)
	{
		switch(store_menu::current_view)
		{
			case store_menu::view::VIEW_CATEGORIES:
				store_menu::current_view = store_category::update(menu);
			break;

			case store_menu::view::VIEW_ITEMS:
				store_menu::current_view = store_items::update(menu);
			break;

			case store_menu::view::VIEW_CART:
				menu = menus::state::MENU_CART;
				store_menu::current_view = store_menu::view::VIEW_EXIT;
				cart_menu::load_cart(api::cart_id);
			break;

			case store_menu::view::VIEW_SELECTION:
				store_menu::current_view = store_selection::update(menu);
			break;
		}
	}

	store_menu::unload_store();
	return menu; 
}