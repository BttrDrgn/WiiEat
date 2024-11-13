#include "restaurant_menu.hpp"

std::vector<restaurant*> restaurant_menu::restaurants;
std::vector<gui_button*> buttons;

int current_page = 0;
int max_page = 0;
bool updating = false;
static gui_text* page_text;

void update_buttons()
{
	updating = true;
	int i = 0;
	int col = 0;
	constexpr int anim_speed = 15;

	for(i = 0; i < 10; ++i)
	{
		// if(col == 0) buttons[i]->set_effect(EFFECT_SLIDE_OUT | EFFECT_SLIDE_LEFT, anim_speed);
		// else if(col == 1) buttons[i]->set_effect(EFFECT_SLIDE_OUT | EFFECT_SLIDE_RIGHT, anim_speed);
		buttons[i]->set_effect(EFFECT_FADE, anim_speed);

		int index = i + (10 * current_page);
		if(index + 1 > restaurant_menu::restaurants.size()) break;

		if(!buttons[i]->is_visible()) buttons[i]->set_visible(true);

		auto text = new gui_text(format::remove_non_ascii(restaurant_menu::restaurants[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(restaurant_menu::restaurants[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		buttons[i]->set_label_hover(text_hover);

		buttons[i]->set_effect(EFFECT_FADE, anim_speed);

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
			buttons[i]->set_visible(false);
		}
	}
	updating = false;
}

void next_page()
{
	if(max_page <= 1) return;

	++current_page;
	if(current_page > max_page - 1)
	{
		current_page = 0;
	}

	page_text->set_text(format::va("%i/%i", current_page + 1, max_page).c_str());

	update_buttons();
}

void prev_page()
{
	if(max_page <= 1) return;

	--current_page;
	if(current_page < 0)
	{
		current_page = max_page - 1;
	}

	page_text->set_text(format::va("%i/%i", current_page + 1, max_page).c_str());

	update_buttons();
}

void restaurant_menu::refresh()
{
	current_page = 0;
	buttons.clear();
	restaurant_menu::restaurants.clear();

	json json = 0;
	auto err = api::restaurants_request(json);
	if(err != api::error::NONE)
	{
		if(err == api::error::UNAUTHORIZED)
		{
			menus::unauthorized_prompt();
		}
		return;
	}

	try
	{
		auto results = json["search_result"]["results"];
		for(int i = 0; i < results.size(); ++i)
		{
			auto r = results[i];
			restaurants.emplace_back(new restaurant(r["name"].get<std::string>(), r["restaurant_id"]));
		}		
	}
	catch(const std::exception& e)
	{
		
	}
	max_page = (int)ceil(restaurant_menu::restaurants.size() / 10.f);
}

menus::state restaurant_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

	if( !api::is_address_complete() )
	{
		menu = menus::next(menus::state::MENU_ADDRESS);
		menus::halt_gui();
		return menu; 
	}

	if(restaurant_menu::restaurants.size() == 0)
	{
		restaurant_menu::refresh();
	}

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

	gui_image_data address_btn_img_data(address_button_png);
	gui_image_data address_btn_hover_img_data(address_button_hover_png);
	gui_image address_btn_img(&address_btn_img_data);
	gui_image address_btn_hover_img(&address_btn_hover_img_data);
	gui_button address_btn(address_btn_img_data.get_width(), address_btn_img_data.get_height());
	address_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	address_btn.set_position(160, 15);
	address_btn.set_image(&address_btn_img);
	address_btn.set_image_hover(&address_btn_hover_img);
	address_btn.set_sound_hover(&btn_sound_hover);
	address_btn.set_trigger(&trig_a);
	address_btn.set_scale(0.75f);
	address_btn.set_effect_grow();
	w.append(&address_btn);

	gui_image_data credit_btn_img_data(credit_button_png);
	gui_image_data credit_btn_hover_img_data(credit_button_hover_png);
	gui_image credit_btn_img(&credit_btn_img_data);
	gui_image credit_btn_hover_img(&credit_btn_hover_img_data);
	gui_button credit_btn(credit_btn_img_data.get_width(), credit_btn_img_data.get_height());
	credit_btn.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	credit_btn.set_position(225, 15);
	credit_btn.set_image(&credit_btn_img);
	credit_btn.set_image_hover(&credit_btn_hover_img);
	credit_btn.set_sound_hover(&btn_sound_hover);
	credit_btn.set_trigger(&trig_a);
	credit_btn.set_scale(0.75f);
	credit_btn.set_effect_grow();
	w.append(&credit_btn);

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
	left_btn.set_scale(0.75f);
	left_btn.set_effect_grow();
	if(max_page > 1) w.append(&left_btn);

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
	right_btn.set_scale(0.75f);
	right_btn.set_effect_grow();
	if(max_page > 1) w.append(&right_btn);

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

	page_text = new gui_text(format::va("1/%i", max_page).c_str(), 15, (GXColor){0, 0, 0, 255});
	page_text->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	page_text->set_position(-112, 40);
	if(max_page > 1) w.append(page_text);

	gui_text info_text("What are Wii eating today?", 20, (GXColor){0, 0, 0, 255});
	info_text.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	info_text.set_position(32, 80);
	w.append(&info_text);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

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

		buttons.emplace_back(new_btn);
		w.append(buttons[i]);

		row++;

		if(i == 4)
		{
			++col;
			row = 0;
		}
	}

	for(int i = 0; i < 10; ++i)
	{
		int index = i + (10 * current_page);
		if(index + 1 > restaurant_menu::restaurants.size()) break;
		auto text = new gui_text(format::remove_non_ascii(restaurant_menu::restaurants[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::remove_non_ascii(restaurant_menu::restaurants[index]->name).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		buttons[i]->set_label_hover(text_hover);

		buttons[i]->set_label(text);
		buttons[i]->set_visible(true);
	}

	w.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);
		if(updating) continue;

		for(int i = 0; i < buttons.size(); ++i)
		{
			if(buttons[i]->get_state() == STATE_CLICKED)
			{
				int index = i + (10 * current_page);

				if(store_menu::load_store(restaurant_menu::restaurants[index]->name, restaurant_menu::restaurants[index]->id))
				{
					w.set_effect(EFFECT_FADE, -25);
					while(w.get_effect() > 0) usleep(100);
					
					menu = menus::next(menus::state::MENU_STORE);
				}
				else
				{
					store_menu::unload_store();
				}
				buttons[i]->reset_state();
				break;
			}
		}

		if(menu != menus::state::MENU_NONE) break;

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = home_menu::update();
			if(menu == menus::state::MENU_CANCEL) menu = menus::state::MENU_NONE;
		}
		else if(address_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_ADDRESS);

			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);
		}
		else if(credit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_PAYMENT);

			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);

			payment_menu::load_payments(api::ud_id);
		}
		else if(left_btn.get_state() == STATE_CLICKED)
		{
			prev_page();
			left_btn.reset_state();
		}
		else if(right_btn.get_state() == STATE_CLICKED)
		{
			next_page();
			right_btn.reset_state();
		}
		else if(exit_btn.get_state() == STATE_CLICKED)
		{
			auto choice = menus::window_prompt
			(
				"Return",
				"Are you sure you want to return to the Homebrew Channel?",
				"Yes",
				"No"
			);

			if(choice == 1)
			{
				menu = menus::state::MENU_EXIT;
			}

			exit_btn.reset_state();
		}
	}

	buttons.clear();
	
	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu; 
}