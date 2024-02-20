#include "home_menu.hpp"

menus::state home_menu::update()
{
	menus::state menu = menus::state::MENU_NONE;

	gui_window home(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data btn(button_png);
	gui_image_data btn_hover(button_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_text quit_btn_txt("Homebrew Menu", 22, (GXColor){0, 0, 0, 255});
	gui_image quit_btn_img(&btn);
	gui_image quit_btn_img_hover(&btn_hover);
	gui_button quit_btn(btn.get_width(), btn.get_height());
	quit_btn.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	quit_btn.set_position(0, 0);
	quit_btn.set_label(&quit_btn_txt);
	quit_btn.set_image(&quit_btn_img);
	quit_btn.set_image_hover(&quit_btn_img_hover);
	quit_btn.set_sound_hover(&btn_sound_hover);
	quit_btn.set_trigger(&trig_a);
	quit_btn.set_scale(1.25f);
	home.append(&quit_btn);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	home.append(&home_btn);

	home.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	menus::halt_gui();
	menus::main_window->set_state(STATE_DISABLED);
	menus::main_window->append(&home);
	menus::main_window->change_focus(&home);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::state::MENU_CANCEL;
		}
		else if(quit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::state::MENU_EXIT;
		}
	}

	home.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(home.GetEffect() > 0) usleep(100);

	menus::halt_gui();
	menus::main_window->remove(&home);
	menus::main_window->set_state(STATE_DEFAULT);
	menus::resume_gui();
	return menu;
}