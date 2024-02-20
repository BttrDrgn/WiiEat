#include "address_menu.hpp"

char address_menu::address[ADDRESS_LEN];
char address_menu::city[CITY_LEN];
char address_menu::state[STATE_LEN];
char address_menu::zip[ZIP_LEN];

menus::state address_menu::update()
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
	logo.set_position(50, 50);
	w.append(&logo);

	gui_text powered_by_txt("Powered by Grubhub", 12, (GXColor){0x50, 0x50, 0x50, 255});
	powered_by_txt.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	powered_by_txt.set_position(60, 100);
	w.append(&powered_by_txt);

	gui_text address_txt("Address:", 24, (GXColor){0, 0, 0, 255});
	address_txt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	address_txt.set_position(-150, 175);
	w.append(&address_txt);

	gui_text address_prompt_txt(address_menu::address, 18, (GXColor){0, 0, 0, 255});
	address_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	address_prompt_txt.set_position(25, 0);
	address_prompt_txt.set_max_width(15 * 20);
	gui_image address_prompt_img(&input_box);
	gui_image address_prompt_img_hover(&input_box_hover);
	address_prompt_img.set_scale_x(1.5f);
	address_prompt_img_hover.set_scale_x(1.5f);
	gui_button address_prompt(input_box.get_width(), input_box.get_height());
	address_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	address_prompt.set_position(20, 150);
	address_prompt.set_label(&address_prompt_txt);
	address_prompt.set_image(&address_prompt_img);
	address_prompt.set_image_hover(&address_prompt_img_hover);
	address_prompt.set_sound_hover(&btn_sound_hover);
	address_prompt.set_trigger(&trig_a);
	w.append(&address_prompt);

	gui_text city_txt("City:", 24, (GXColor){0, 0, 0, 255});
	city_txt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	city_txt.set_position(-125, 250);
	w.append(&city_txt);

	gui_text city_prompt_txt(address_menu::city, 18, (GXColor){0, 0, 0, 255});
	city_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	city_prompt_txt.set_position(25, 0);
	city_prompt_txt.set_max_width(15 * 20);
	gui_image city_prompt_img(&input_box);
	gui_image city_prompt_img_hover(&input_box_hover);
	city_prompt_img.set_scale_x(0.75);
	city_prompt_img_hover.set_scale_x(0.75f);
	gui_button city_prompt(input_box.get_width(), input_box.get_height());
	city_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	city_prompt.set_position(20, 225);
	city_prompt.set_label(&city_prompt_txt);
	city_prompt.set_image(&city_prompt_img);
	city_prompt.set_image_hover(&city_prompt_img_hover);
	city_prompt.set_sound_hover(&btn_sound_hover);
	city_prompt.set_trigger(&trig_a);
	w.append(&city_prompt);

	gui_text state_txt("State:", 24, (GXColor){0, 0, 0, 255});
	state_txt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	state_txt.set_position(125, 250);
	w.append(&state_txt);

	gui_text state_prompt_txt(address_menu::state, 18, (GXColor){0, 0, 0, 255});
	state_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	state_prompt_txt.set_position(25, 0);
	state_prompt_txt.set_max_width(15 * 20);
	gui_image state_prompt_img(&input_box);
	gui_image state_prompt_img_hover(&input_box_hover);
	state_prompt_img.set_scale_x(0.35);
	state_prompt_img_hover.set_scale_x(0.35f);
	gui_button state_prompt(input_box.get_width(), input_box.get_height());
	state_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	state_prompt.set_position(280, 225);
	state_prompt.set_label(&state_prompt_txt);
	state_prompt.set_image(&state_prompt_img);
	state_prompt.set_image_hover(&state_prompt_img_hover);
	state_prompt.set_sound_hover(&btn_sound_hover);
	state_prompt.set_trigger(&trig_a);
	w.append(&state_prompt);

	gui_text zip_txt("Zip:", 24, (GXColor){0, 0, 0, 255});
	zip_txt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	zip_txt.set_position(-125, 325);
	w.append(&zip_txt);

	gui_text zip_prompt_txt(address_menu::zip, 18, (GXColor){0, 0, 0, 255});
	zip_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	zip_prompt_txt.set_position(25, 0);
	zip_prompt_txt.set_max_width(15 * 20);
	gui_image zip_prompt_img(&input_box);
	gui_image zip_prompt_img_hover(&input_box_hover);
	zip_prompt_img.set_scale_x(0.75);
	zip_prompt_img_hover.set_scale_x(0.75f);
	gui_button zip_prompt(input_box.get_width(), input_box.get_height());
	zip_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	zip_prompt.set_position(20, 300);
	zip_prompt.set_label(&zip_prompt_txt);
	zip_prompt.set_image(&zip_prompt_img);
	zip_prompt.set_image_hover(&zip_prompt_img_hover);
	zip_prompt.set_sound_hover(&btn_sound_hover);
	zip_prompt.set_trigger(&trig_a);
	w.append(&zip_prompt);

	gui_text save_btn_txt("Save", 22, (GXColor){0, 0, 0, 255});
	gui_image save_btn_img(&btn_small);
	gui_image save_btn_img_over(&btn_small_hover);
	gui_button save_btn(btn_small.get_width(), btn_small.get_height());
	save_btn.set_alignment(ALIGN_CENTER, ALIGN_BOTTOM);
	save_btn.set_position(0, -35);
	save_btn.set_label(&save_btn_txt);
	save_btn.set_image(&save_btn_img);
	save_btn.set_image_hover(&save_btn_img_over);
	save_btn.set_sound_hover(&btn_sound_hover);
	save_btn.set_trigger(&trig_a);
	save_btn.set_effect_grow();
	w.append(&save_btn);

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
		else if(address_prompt.get_state() == STATE_CLICKED)
		{
			menus::keyboard(address_menu::address, ADDRESS_LEN);
			menu = menus::current_menu;
		}
		else if(city_prompt.get_state() == STATE_CLICKED)
		{
			menus::keyboard(address_menu::city, CITY_LEN);
			menu = menus::current_menu;
		}
		else if(state_prompt.get_state() == STATE_CLICKED)
		{
			menus::keyboard(address_menu::state, STATE_LEN);
			menu = menus::current_menu;
		}
		else if(zip_prompt.get_state() == STATE_CLICKED)
		{
			menus::keyboard(address_menu::zip, ZIP_LEN);
			menu = menus::current_menu;
		}
		else if(save_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::state::MENU_RESTAURANT;
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;   
}