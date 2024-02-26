#include "home_menu.hpp"
#include <main.hpp>

menus::state home_menu::update()
{
	menus::state menu = menus::state::MENU_NONE;

	gui_window home(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data btn(button_png);
	gui_image_data btn_hover(button_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_image* bg_img = new gui_image(screen_width, screen_height, (GXColor){50, 50, 50, 255});
	bg_img->set_alpha(200);
	bg_img->color_stripe(18);
	home.append(bg_img);

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

#ifdef DEBUG
	gui_text console_btn_txt("Debug Console", 22, (GXColor){0, 0, 0, 255});
	gui_image console_btn_img(&btn);
	gui_image console_btn_img_hover(&btn_hover);
	gui_button console_btn(btn.get_width(), btn.get_height());
	console_btn.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	console_btn.set_position(0, 75);
	console_btn.set_label(&console_btn_txt);
	console_btn.set_image(&console_btn_img);
	console_btn.set_image_hover(&console_btn_img_hover);
	console_btn.set_sound_hover(&btn_sound_hover);
	console_btn.set_trigger(&trig_a);
	console_btn.set_scale(1.25f);
	home.append(&console_btn);
#endif

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	home.append(&home_btn);

	home.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->set_state(STATE_DISABLED);
	menus::main_window->append(&home);
	menus::main_window->change_focus(&home);
	menus::resume_gui();

	menus::current_song->Pause();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::state::MENU_CANCEL;
		}
		else if(quit_btn.get_state() == STATE_CLICKED)
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
			else
			{
				quit_btn.reset_state();
			}
		}
#ifdef DEBUG
		else if(console_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_CONSOLE);
		}
#endif
	}

	home.set_effect(EFFECT_FADE, -25);
	while(home.get_effect() > 0) usleep(100);

	menus::halt_gui();
	menus::main_window->remove(&home);
	menus::main_window->set_state(STATE_DEFAULT);
	menus::resume_gui();

	if(menu != menus::state::MENU_EXIT) menus::current_song->Resume();

	return menu;
}