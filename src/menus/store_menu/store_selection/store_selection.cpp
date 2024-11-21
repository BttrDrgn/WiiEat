#include "store_selection.hpp"
#include "../store_category/store_category.hpp"
#include "../store_items/store_items.hpp"

std::vector<gui_button*> store_selection::buttons;
std::vector<choice*> store_selection::choices;
int store_selection::current_page = 0;
int store_selection::max_page = 0;
std::string store_selection::current_item = "";
std::string store_selection::current_item_id = "";
std::unordered_map<std::string, std::vector<std::string>> store_selection::selected_choices;
double store_selection::current_item_cost = 0;
static gui_text* page_text;
static gui_text* price_text;
std::string store_selection::img_id;

bool store_selection::load_choices(const std::string& item_name, const std::string& store_id, const std::string& item_id, const std::string& img_id, double item_cost)
{
	store_selection::img_id = img_id;
	json json = 0;
	auto err = api::item_info_request(store_id, item_id, json);
	if(err == api::error::NONE)
	{
		try
		{
			auto choices = json["choice_category_list"];
			for(int i = 0; i < choices.size(); ++i)
			{
				auto name = format::remove_non_ascii(choices[i]["name"].get<std::string>());
				auto choice_option_list = choices[i]["choice_option_list"];

				auto new_choice = new choice(
					name, choices[i]["id"].get<std::string>(),
					choices[i]["max_choice_options"].get<int>(),
					choices[i]["min_choice_options"].get<int>()
				);

				std::string defaulted;
				for(int c = 0; c < choice_option_list.size(); ++c)
				{
					if(defaulted.size() <= 0)
					{
						if(choice_option_list[c]["defaulted"].get<bool>())
						{
							defaulted = choice_option_list[c]["id"].get<std::string>();
						}
					}

					new_choice->add_option(
						format::remove_non_ascii(choice_option_list[c]["description"].get<std::string>()),
						choice_option_list[c]["id"].get<std::string>(),
						choice_option_list[c]["delivery_price"]["amount"].get<double>()
					);
				}

				new_choice->required = defaulted.size() <= 0 && new_choice->min_options > 0;

				store_selection::choices.emplace_back(new_choice);
			}

			store_selection::max_page = (int)ceil(store_selection::choices.size() / 10.f);
			store_selection::current_item = item_name;
			store_selection::current_item_id = item_id;
			store_selection::current_item_cost = item_cost;
            return true;
		}
		catch(const std::exception& e)
		{
			console_menu::write_line(e.what());
		}
	}
	else if(err == api::error::UNAUTHORIZED)
	{
		menus::unauthorized_prompt();
	}

    return false;
}

void store_selection::update_buttons(bool animate = true)
{
	int i = 0;
	int col = 0;
	constexpr int anim_speed = 15;

	for(i = 0; i < 10; ++i)
	{
		int index = i + (10 * store_selection::current_page);
		if(index + 1 > choices.size()) break;

		if(!store_selection::buttons[i]->is_visible()) store_selection::buttons[i]->set_visible(true);

		auto text = new gui_text(format::remove_non_ascii(store_selection::choices[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		store_selection::buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(store_selection::choices[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		store_selection::buttons[i]->set_label_hover(text_hover);
		
		if (animate) store_selection::buttons[i]->set_effect(EFFECT_FADE, anim_speed);

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
			store_selection::buttons[i]->set_visible(false);
		}
	}
}

void store_selection::next_page()
{
	if(store_selection::max_page <= 1) return;

	++store_selection::current_page;
	if(store_selection::current_page > store_selection::max_page - 1)
	{
		store_selection::current_page = 0;
	}

	page_text->set_text(format::va("%i/%i", current_page + 1, max_page).c_str());

	store_selection::update_buttons();
}

void store_selection::prev_page()
{
	if(store_selection::max_page <= 1) return;

	--store_selection::current_page;
	if(store_selection::current_page < 0)
	{
		store_selection::current_page = store_selection::max_page - 1;
	}

	page_text->set_text(format::va("%i/%i", current_page + 1, max_page).c_str());

	store_selection::update_buttons();
}

store_menu::view store_selection::update(menus::state& menu)
{
	auto view = store_menu::view::VIEW_NONE;

	gui_window w(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data input_box(input_box_png);
	gui_image_data input_box_hover(input_box_hover_png);

	gui_image_data btn(main::dark_mode ? button_dark_png : button_png);
	gui_image_data btn_hover(main::dark_mode ? button_dark_hover_png : button_hover_png);
	gui_image_data btn_required(button_required_png);
	gui_image_data btn_required_hover(button_required_hover_png);
	gui_image_data btn_small(button_small_png);
	gui_image_data btn_small_hover(button_small_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, 0);

	gui_trigger trig_minus;
	trig_minus.set_button_only_trigger(-1, WPAD_BUTTON_MINUS, 0);

	gui_trigger trig_plus;
	trig_plus.set_button_only_trigger(-1, WPAD_BUTTON_PLUS, 0);

	auto img = api::download_image(img_id, 320, 320);
	gui_image_data background_image_img(img.data);
	gui_image background_image(&background_image_img);
	background_image.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	background_image.set_position(0, 0);
	background_image.set_scale(1.5f);
	background_image.set_visible(false);
	w.append(&background_image);

	gui_image_data logoImage(wiieat_logo_png);
	gui_image logo(&logoImage);
	logo.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	logo.set_position(25, 25);
	logo.set_scale(0.75f);
	w.append(&logo);

	gui_text add_btn_txt("Add To Cart", 22, (GXColor){0, 0, 0, 255});
	gui_image add_btn_img(&btn);
	gui_image add_btn_img_over(&btn_hover);
	gui_button add_btn(btn.get_width(), btn.get_height());
	add_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	add_btn.set_position(160, 17);
	add_btn.set_label(&add_btn_txt);
	add_btn.set_image(&add_btn_img);
	add_btn.set_image_hover(&add_btn_img_over);
	add_btn.set_sound_hover(&btn_sound_hover);
	add_btn.set_trigger(&trig_a);
	add_btn.set_effect_grow();
	add_btn.set_scale(0.75f);
	w.append(&add_btn);

	price_text = new gui_text(format::va("$%.2f", store_selection::current_item_cost / 100.0).c_str(), 20, (GXColor){0, 0, 0, 255});
	price_text->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	price_text->set_position(-185, 36);
	w.append(price_text);

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
	if(store_selection::max_page > 1) w.append(&left_btn);

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
	if(store_selection::max_page > 1) w.append(&right_btn);

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

	gui_image_data image_btn_img_data(image_button_png);
	gui_image_data image_btn_hover_img_data(image_button_hover_png);
	gui_image image_btn_img(&image_btn_img_data);
	gui_image image_btn_hover_img(&image_btn_hover_img_data);
	gui_button image_btn(image_btn_img_data.get_width(), image_btn_img_data.get_height());
	image_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	image_btn.set_position(-25, -15);
	image_btn.set_image(&image_btn_img);
	image_btn.set_image_hover(&image_btn_hover_img);
	image_btn.set_sound_hover(&btn_sound_hover);
	image_btn.set_trigger(&trig_a);
	image_btn.set_scale(0.75f);
	image_btn.set_effect_grow();
	if (img_id.size() > 0) w.append(&image_btn);

	page_text = new gui_text(format::va("1/%i", max_page).c_str(), 15, (GXColor){0, 0, 0, 255});
	page_text->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	page_text->set_position(-112, 40);
	if(max_page > 1) w.append(page_text);

	gui_text info_text(format::va("%s - ", store_menu::store_name.c_str()).c_str(), 20, (GXColor){0, 0, 0, 255});
	info_text.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	info_text.set_position(32, 80);
	w.append(&info_text);

	gui_text item_text(store_selection::current_item.c_str(), 20, (GXColor){0, 0, 0, 255});
	item_text.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	item_text.set_position(32 + info_text.get_text_width(), 80);
	item_text.set_max_width(500 - (store_menu::store_name.length() + 3) * 2.9);
	item_text.set_scroll(true);
	w.append(&item_text);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

	int col = 0;
	int row = 0;
	bool hide_ui = false;

	for(int i = 0; i < 10; ++i)
	{
		gui_button* new_btn = new gui_button(btn.get_width(), btn.get_height());

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
		new_btn->set_sound_hover(&btn_sound_hover);
		new_btn->set_trigger(&trig_a);
		new_btn->set_visible(false);
		new_btn->set_effect_grow();

		store_selection::buttons.emplace_back(new_btn);
		w.append(store_selection::buttons[i]);

		row++;

		if(i == 4)
		{
			++col;
			row = 0;
		}
	}

	for(int i = 0; i < 10; ++i)
	{
		int index = i + (10 * store_selection::current_page);
		if(index + 1 > store_selection::choices.size()) break;

		auto text = new gui_text(format::remove_non_ascii(store_selection::choices[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		store_selection::buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(store_selection::choices[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		store_selection::buttons[i]->set_label_hover(text_hover);

		gui_image* new_img = new gui_image(!store_selection::choices[index]->required ? &btn : &btn_required);
		gui_image* new_img_hover = new gui_image(!store_selection::choices[index]->required ? &btn_hover : &btn_required_hover);
		store_selection::buttons[i]->set_image(new_img);
		store_selection::buttons[i]->set_image_hover(new_img_hover);
		store_selection::buttons[i]->set_visible(true);
	}

	w.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(view == store_menu::view::VIEW_NONE)
	{
		usleep(100);

		for(int i = 0; i < buttons.size(); ++i)
		{
			if(store_selection::buttons[i]->get_state() == STATE_CLICKED)
			{
				int index = i + (10 * current_page);
				console_menu::write_line(store_selection::choices[index]->name.c_str());
				store_selection::buttons[i]->reset_state();
				break;
			}
		}

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = home_menu::update();
			if(menu == menus::state::MENU_CANCEL) view = store_menu::view::VIEW_NONE;
			else if(menu != menus::state::MENU_CANCEL)  view = store_menu::view::VIEW_EXIT;
		}
		else if(image_btn.get_state() == STATE_CLICKED)
		{
			hide_ui = !hide_ui;

			for(int i = 0; i < buttons.size(); ++i)
			{
				buttons[i]->set_visible(!hide_ui);
			}

			if (!hide_ui) update_buttons(false);

			background_image.set_visible(hide_ui);
			add_btn.set_visible(!hide_ui);
			price_text->set_visible(!hide_ui);
			page_text->set_visible(!hide_ui);
			item_text.set_visible(!hide_ui);
			info_text.set_visible(!hide_ui);
			exit_btn.set_visible(!hide_ui);

			image_btn.reset_state();
		}
		else if(exit_btn.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);

			view = store_menu::view::VIEW_ITEMS;
		}
		else if(add_btn.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);

			if(api::cart_id.length() <= 0)
			{
				api::create_cart_request();
				api::locked_store_id = store_menu::store_id;
				api::locked_store_name = store_menu::store_name;
			}
			
			std::vector<std::uint32_t> option_ids {};
			for(int i = 0; i < store_selection::choices.size(); ++i)
			{
				try
				{
					option_ids.emplace_back(std::stoul(store_selection::choices[i]->options[0]->id));
				}
				catch(std::exception e)
				{
					console_menu::write_line(e.what());
				}
			}

			if(api::add_item_request(api::cart_id, api::locked_store_id, store_selection::current_item_id, store_selection::current_item_cost / 100.0, option_ids) != api::error::NONE)
			{
				console_menu::write_line("adding item error!");
			}

			store_items::unload_items();
			view = store_menu::view::VIEW_CATEGORIES;
			add_btn.reset_state();
		}
		else if(left_btn.get_state() == STATE_CLICKED)
		{
			store_selection::prev_page();
			left_btn.reset_state();
		}
		else if(right_btn.get_state() == STATE_CLICKED)
		{
			store_selection::next_page();
			right_btn.reset_state();
		}
	}

	store_selection::unload_choices();

	store_selection::buttons.clear();
	delete[] img.data;

	menus::halt_gui();
	menus::main_window->remove(&w);

	return view;
}

void store_selection::unload_choices()
{
	for(auto ptr : store_selection::choices)
	{
		delete ptr;
	}
	
	store_selection::choices.clear();
	store_selection::current_page = 0;
	store_selection::max_page = 0;
	store_selection::current_item = "";
	store_selection::current_item_id = "";
}