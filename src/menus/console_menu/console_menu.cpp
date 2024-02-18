#include "console_menu.hpp"

std::vector<std::string> console_menu::console_out{};

menus::state console_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

	gui_window w(screenwidth, screenheight);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data circle_btn(circle_button_png);
	gui_image_data circle_btn_hover(circle_button_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);

	for(int i = 0; i < console_menu::console_out.size(); ++i)
    {

    }

	gui_text exit_btn_txt("X", 22, (GXColor){0, 0, 0, 255});
	gui_image exit_btn_img(&circle_btn);
	gui_image exit_btn_img_hover(&circle_btn_hover);
	gui_button exit_btn(circle_btn.GetWidth(), circle_btn.GetHeight());
	exit_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	exit_btn.set_position(-35, -35);
	exit_btn.set_label(&exit_btn_txt);
	exit_btn.set_image(&exit_btn_img);
	exit_btn.set_image_hover(&exit_btn_img_hover);
	exit_btn.set_sound_hover(&btn_sound_hover);
	exit_btn.set_trigger(&trig_a);
	exit_btn.set_trigger(&trig_home);
	exit_btn.set_scale(0.75f);
	w.append(&exit_btn);

	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if(exit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::prev_menu;
			menus::prev_menu = menus::state::MENU_CONSOLE;
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;
}