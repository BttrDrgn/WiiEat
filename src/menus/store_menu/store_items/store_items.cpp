#include "store_items.hpp"
#include "../store_category/store_category.hpp"

std::vector<gui_button*> store_items::buttons;
std::vector<menu_item*> store_items::items;
int store_items::current_page = 0;
int store_items::max_page = 0;

void store_items::update_buttons()
{
	int i = 0;
	int col = 0;
	constexpr int anim_speed = 15;

	for(i = 0; i < 10; ++i)
	{
		store_items::buttons[i]->set_effect(EFFECT_FADE, anim_speed);

		int index = i + (10 * store_items::current_page);
		if(index + 1 > items.size()) break;

		if(!store_items::buttons[i]->is_visible()) store_items::buttons[i]->set_visible(true);

		auto text = new gui_text(format::remove_non_ascii(store_items::items[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		store_items::buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(store_items::items[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		store_items::buttons[i]->set_label_hover(text_hover);

		store_items::buttons[i]->set_effect(EFFECT_FADE, anim_speed);

		if(i == 4)
		{
			++col;
		}
	}

	if(i < 10)
	{
		i = 10 - i;
		for(i = 10 - i; i < 10; ++i)
		{
			store_items::buttons[i]->set_visible(false);
		}
	}
}

void store_items::next_page()
{
	if(store_items::max_page <= 1) return;

	++store_items::current_page;
	if(store_items::current_page > store_items::max_page - 1)
	{
		store_items::current_page = 0;
	}

	store_items::update_buttons();
}

void store_items::prev_page()
{
	if(store_items::max_page <= 1) return;

	--store_items::current_page;
	if(store_items::current_page < 0)
	{
		store_items::current_page = store_items::max_page - 1;
	}

	store_items::update_buttons();
}

void store_items::load_items(const std::string& store_id, const std::string& cat_id)
{
	try
	{
		json json = 0;
		auto err = api::category_items_request(store_id, cat_id, api::operation_id, json);
		if(err == api::error::NONE)
		{
			auto menu_items = json["object"]["data"]["content"];
			for(int i = 0; i < menu_items.size(); ++i)
			{
				auto item = menu_items[i];
				store_items::items.emplace_back(new menu_item(
					format::remove_non_ascii(item["entity"]["item_name"].get<std::string>()),
					item["entity"]["item_id"].get<std::string>())
				);
			}

			console_menu::write_line(format::va("%i", store_items::items.size()));
			store_items::max_page = (int)floor(store_items::items.size() / 10.f);
		}
		else if(err == api::error::UNAUTHORIZED)
		{
			menus::unauthorized_prompt();
		}
	}
	catch(const std::exception& e)
	{
		console_menu::write_line(e.what());
	}
}

store_menu::view store_items::update(menus::state& menu)
{
	auto view = store_menu::view::VIEW_NONE;

	gui_window w(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data input_box(input_box_png);
	gui_image_data input_box_hover(input_box_hover_png);

	gui_image_data btn(button_png);
	gui_image_data btn_hover(button_hover_png);
	gui_image_data btn_small(button_small_png);
	gui_image_data btn_small_hover(button_small_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, 0);

	gui_trigger trig_minus;
	trig_minus.set_button_only_trigger(-1, WPAD_BUTTON_MINUS, 0);

	gui_trigger trig_plus;
	trig_plus.set_button_only_trigger(-1, WPAD_BUTTON_PLUS, 0);

	gui_image_data logoImage(wiieat_logo_png);
	gui_image logo(&logoImage);
	logo.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	logo.set_position(25, 25);
	logo.set_scale(0.75f);
	w.append(&logo);

	gui_image_data basket_btn_img_data(basket_button_png);
	gui_image_data basket_btn_hover_img_data(basket_button_hover_png);
	gui_image basket_btn_img(&basket_btn_img_data);
	gui_image basket_btn_hover_img(&basket_btn_hover_img_data);
	gui_button basket_btn(basket_btn_img_data.get_width(), basket_btn_img_data.get_height());
	basket_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	basket_btn.set_position(234, 15);
	basket_btn.set_image(&basket_btn_img);
	basket_btn.set_image_hover(&basket_btn_hover_img);
	basket_btn.set_sound_hover(&btn_sound_hover);
	basket_btn.set_trigger(&trig_a);
	basket_btn.set_scale(0.75f);
	basket_btn.set_effect_grow();
	w.append(&basket_btn);

	gui_image_data left_btn_img_data(left_button_png);
	gui_image_data left_btn_hover_img_data(left_button_hover_png);
	gui_image left_btn_img(&left_btn_img_data);
	gui_image left_btn_hover_img(&left_btn_hover_img_data);
	gui_button left_btn(left_btn_img_data.get_width(), left_btn_img_data.get_height());
	left_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	left_btn.set_position(382 - 37, 15);
	left_btn.set_image(&left_btn_img);
	left_btn.set_image_hover(&left_btn_hover_img);
	left_btn.set_sound_hover(&btn_sound_hover);
	left_btn.set_trigger(&trig_a);
	left_btn.set_trigger(&trig_plus);
	left_btn.set_scale(0.75f);
	left_btn.set_effect_grow();
	if(store_items::max_page > 1) w.append(&left_btn);

	gui_image_data right_btn_img_data(right_button_png);
	gui_image_data right_btn_hover_img_data(right_button_hover_png);
	gui_image right_btn_img(&right_btn_img_data);
	gui_image right_btn_hover_img(&right_btn_hover_img_data);
	gui_button right_btn(right_btn_img_data.get_width(), right_btn_img_data.get_height());
	right_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	right_btn.set_position(456 - 37, 15);
	right_btn.set_image(&right_btn_img);
	right_btn.set_image_hover(&right_btn_hover_img);
	right_btn.set_sound_hover(&btn_sound_hover);
	right_btn.set_trigger(&trig_a);
	right_btn.set_trigger(&trig_minus);
	right_btn.set_scale(0.75f);
	right_btn.set_effect_grow();
	if(store_items::max_page > 1) w.append(&right_btn);

	gui_image_data exit_btn_img_data(exit_button_png);
	gui_image_data exit_btn_hover_img_data(exit_button_hover_png);
	gui_image exit_btn_img(&exit_btn_img_data);
	gui_image exit_btn_hover_img(&exit_btn_hover_img_data);
	gui_button exit_btn(exit_btn_img_data.get_width(), exit_btn_img_data.get_height());
	exit_btn.set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	exit_btn.set_position(-25, 15);
	exit_btn.set_image(&exit_btn_img);
	exit_btn.set_image_hover(&exit_btn_hover_img);
	exit_btn.set_sound_hover(&btn_sound_hover);
	exit_btn.set_trigger(&trig_a);
	exit_btn.set_scale(0.75f);
	exit_btn.set_effect_grow();
	w.append(&exit_btn);

	gui_text info_text(format::va("%s - %s", store_menu::store_name.c_str(), store_category::category_name.c_str()).c_str(), 20, (GXColor){0, 0, 0, 255});
	info_text.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	info_text.set_position(32, 80);
	w.append(&info_text);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

	if(store_items::buttons.size() == 0)
	{
		int col = 0;
		int row = 0;

		for(int i = 0; i < 10; ++i)
		{
			gui_image* new_img = new gui_image(&btn);
			gui_image* new_img_hover = new gui_image(&btn_hover);
			gui_button* new_btn = new gui_button(new_img->get_width(), new_img->get_height());

			float y_pos = 100 + row * 68;
			if(col == 0)
			{
				new_btn->set_alignment(ALIGN_LEFT, ALIGN_CENTER);
				new_btn->set_position(48, y_pos);
			}
			else
			{
				new_btn->set_alignment(ALIGN_RIGHT, ALIGN_CENTER);
				new_btn->set_position(-48, y_pos);
			}
			new_btn->set_image(new_img);
			new_btn->set_image_hover(new_img_hover);
			new_btn->set_sound_hover(&btn_sound_hover);
			new_btn->set_trigger(&trig_a);
			new_btn->set_visible(false);
			new_btn->set_effect_grow();

			store_items::buttons.emplace_back(new_btn);
			w.append(store_items::buttons[i]);

			row++;

			if(i == 4)
			{
				++col;
				row = 0;
			}
		}
	}

	for(int i = 0; i < 10; ++i)
	{
		int index = i + (10 * store_items::current_page);
		if(index + 1 > store_items::items.size()) break;

		auto text = new gui_text(format::remove_non_ascii(store_items::items[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		store_items::buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(store_items::items[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		store_items::buttons[i]->set_label_hover(text_hover);

		store_items::buttons[i]->set_visible(true);
	}

	//w.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(view == store_menu::view::VIEW_NONE)
	{
		usleep(100);

		for(int i = 0; i < buttons.size(); ++i)
		{
			if(store_items::buttons[i]->get_state() == STATE_CLICKED)
			{
				int index = i + (10 * current_page);
				store_selection::load_choices(store_menu::store_id, store_items::items[index]->id);
				view = store_menu::view::VIEW_SELECTION;
				store_items::buttons[i]->reset_state();
				break;
			}
		}

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = home_menu::update();
			if(menu == menus::state::MENU_CANCEL) view = store_menu::view::VIEW_NONE;
			else if(menu != menus::state::MENU_CANCEL)  view = store_menu::view::VIEW_EXIT;
		}
		else if(exit_btn.get_state() == STATE_CLICKED)
		{
			view = store_menu::view::VIEW_CATEGORIES;
			store_items::unload_items();
		}
		else if(left_btn.get_state() == STATE_CLICKED)
		{
			store_items::prev_page();
			left_btn.reset_state();
		}
		else if(right_btn.get_state() == STATE_CLICKED)
		{
			store_items::next_page();
			right_btn.reset_state();
		}
	}

	store_items::buttons.clear();

	menus::halt_gui();
	menus::main_window->remove(&w);

	return view;
}

void store_items::unload_items()
{
	store_items::items.clear();
	store_items::current_page = 0;
	store_items::max_page = 0;
}
