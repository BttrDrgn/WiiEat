#include "restaurant_menu.hpp"

std::vector<restaurant*> restaurants;

menus::state restaurant_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

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
	logo.set_scale(0.5f);
	w.append(&logo);

	for(int i = 0; i < 4; ++i)
	{
		restaurants.emplace_back(new restaurant(format::va("Restaurant %i", i + 1)));
		restaurants[i]->txt = new gui_text(restaurants[i]->name.c_str(), 24, (GXColor){0x0, 0x0, 0x0, 255});
		restaurants[i]->btn_img = new gui_image(&btn);
		restaurants[i]->btn_hover_img = new gui_image(&btn_hover);
		restaurants[i]->btn = new gui_button(restaurants[i]->btn_img->get_width(), restaurants[i]->btn_img->get_height());
		
		restaurants[i]->btn->set_alignment(ALIGN_LEFT, ALIGN_CENTER);
		restaurants[i]->btn->set_position(32, 76 + i * 64);
		restaurants[i]->btn->set_label(restaurants[i]->txt);
		restaurants[i]->btn->set_image(restaurants[i]->btn_img);
		restaurants[i]->btn->set_image_hover(restaurants[i]->btn_hover_img);
		restaurants[i]->btn->set_sound_hover(&btn_sound_hover);
		restaurants[i]->btn->set_trigger(&trig_a);


		w.append(restaurants[i]->btn);
	}

	// auto test_img = api::download_image("https://resize.sardo.work/?imageUrl=https://media-cdn.grubhub.com/image/upload/v1687457248/rkgnpcgpzhjpu8trbeze.png&width=96");
	// fs::write_file("sd://test.png", test_img.data, test_img.size);

	// if(test_img.data)
	// {
	// 	gui_image_data test_img_data(test_img.data);
	// 	gui_image test(&test_img_data);
	// 	test.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	// 	test.set_position(25, 25);
	// 	test.set_scale(0.5f);
	// 	w.append(&test);
	// }

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

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
	}

	restaurants.clear();
	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu; 
}