#include "restaurant_menu.hpp"

std::vector<restaurant*> restaurants;
int page = 0;

menus::state restaurant_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

	if( !api::is_address_complete() )
	{
		menu = menus::next(menus::state::MENU_ADDRESS);
		menus::halt_gui();
		return menu; 
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
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

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
	w.append(&address_btn);

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
	w.append(&basket_btn);

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
	w.append(&exit_btn);

	int col = 0;
	int row = 0;

	for(int i = 0; i < 10; ++i)
	{
		if(i + 1 > restaurants.size()) break;

		int index = i + (10 * page);
		restaurants[index]->txt = new gui_text(restaurants[i]->name.c_str(), 24, (GXColor){0x0, 0x0, 0x0, 255});
		restaurants[index]->btn_img = new gui_image(&btn);
		restaurants[index]->btn_hover_img = new gui_image(&btn_hover);
		restaurants[index]->btn = new gui_button(restaurants[index]->btn_img->get_width(), restaurants[index]->btn_img->get_height());
		
		restaurants[index]->btn->set_alignment(ALIGN_LEFT, ALIGN_CENTER);
		restaurants[index]->btn->set_position(32 + col * 320, 76 + row * 64);
		restaurants[index]->btn->set_label(restaurants[index]->txt);
		restaurants[index]->btn->set_image(restaurants[index]->btn_img);
		restaurants[index]->btn->set_image_hover(restaurants[index]->btn_hover_img);
		restaurants[index]->btn->set_sound_hover(&btn_sound_hover);
		restaurants[index]->btn->set_trigger(&trig_a);

		w.append(restaurants[index]->btn);
		row++;

		if(i == 4)
		{
			++col;
			row = 0;
		}
	}

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
		else if(address_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_ADDRESS);

			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu; 
}