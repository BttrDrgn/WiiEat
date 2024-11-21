#include "cart_menu.hpp"

void cart_menu::load_cart(const std::string& cart_id)
{
	try
	{
		json json = 0;
		auto err = api::get_cart_request(cart_id, json);
		console_menu::write_line(format::va("Items in cart: %i", json["charges"]["lines"]["line_items"].size()).c_str());
	}
	catch(const std::exception& e)
	{
		console_menu::write_line(e.what());
	}
}

menus::state cart_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

	gui_window w(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data input_box(input_box_png);
	gui_image_data input_box_hover(input_box_hover_png);

	gui_image_data btn(main::dark_mode ? button_dark_png : button_png);
	gui_image_data btn_hover(main::dark_mode ? button_dark_hover_png : button_hover_png);
	gui_image_data btn_small(button_small_png);
	gui_image_data btn_small_hover(button_small_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_image_data logoImage(wiieat_logo_png);
	gui_image logo(&logoImage);
	logo.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	logo.set_position(50, 50);
	w.append(&logo);

	gui_text powered_by_txt("Powered by Grubhub", 12, (GXColor){0x50, 0x50, 0x50, 255});
	powered_by_txt.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	powered_by_txt.set_position(60, 100);
	w.append(&powered_by_txt);

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

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

	w.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = home_menu::update();
			if(menu == menus::state::MENU_CANCEL) menu = menus::state::MENU_NONE;
		}
		else if (exit_btn.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);

			if(store_menu::load_store(api::locked_store_name, api::locked_store_id))
			{
				menu = menus::state::MENU_STORE;
			}
			else
			{
				store_menu::unload_store();
			}
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;   
}