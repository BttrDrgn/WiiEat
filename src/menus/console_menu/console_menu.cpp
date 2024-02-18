#include "console_menu.hpp"

std::vector<std::string> console_menu::console_out{};
int console_menu::scroll = 0;
bool console_menu::needs_update = true;
gui_text* lines[LINES];

menus::state console_menu::update()
{
	needs_update = true;
    menus::state menu = menus::state::MENU_NONE;

	gui_window w(screen_width, screen_height);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data circle_btn(circle_button_png);
	gui_image_data circle_btn_hover(circle_button_hover_png);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);

	for(int i = 0; i < LINES; ++i)
    {
		lines[i] = new gui_text("", 22, (GXColor){0, 0, 0, 255});
		lines[i]->set_alignment(ALIGN_LEFT, ALIGN_CENTER);
		lines[i]->set_position(32, 32 + i * 25);
		w.append(lines[i]);
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

	gui_text up_btn_txt("^", 22, (GXColor){0, 0, 0, 255});
	gui_image up_btn_img(&circle_btn);
	gui_image up_btn_img_hover(&circle_btn_hover);
	gui_button up_btn(circle_btn.GetWidth(), circle_btn.GetHeight());
	up_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	up_btn.set_position(-35, -200);
	up_btn.set_label(&up_btn_txt);
	up_btn.set_image(&up_btn_img);
	up_btn.set_image_hover(&up_btn_img_hover);
	up_btn.set_sound_hover(&btn_sound_hover);
	up_btn.set_trigger(&trig_a);
	up_btn.set_scale(0.75f);
	w.append(&up_btn);

	gui_text down_btn_txt("V", 22, (GXColor){0, 0, 0, 255});
	gui_image down_btn_img(&circle_btn);
	gui_image down_btn_img_hover(&circle_btn_hover);
	gui_button down_btn(circle_btn.GetWidth(), circle_btn.GetHeight());
	down_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	down_btn.set_position(-35, -145);
	down_btn.set_label(&down_btn_txt);
	down_btn.set_image(&down_btn_img);
	down_btn.set_image_hover(&down_btn_img_hover);
	down_btn.set_sound_hover(&btn_sound_hover);
	down_btn.set_trigger(&trig_a);
	down_btn.set_scale(0.75f);
	w.append(&down_btn);

	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if (needs_update)
		{
			for (int i = 0; i < LINES; ++i)
			{
				int line_index = i + scroll;
				if (line_index >= console_out.size())
					break;

				lines[i]->set_text(console_out[line_index].data());
			}
			needs_update = false;
		}

		if(exit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::prev_menu;
			menus::prev_menu = menus::state::MENU_CONSOLE;
		}
		else if(up_btn.get_state() == STATE_CLICKED)
		{
			scroll_dir(-1);
			up_btn.reset_state();
		}
		else if(down_btn.get_state() == STATE_CLICKED)
		{
			scroll_dir(1);
			down_btn.reset_state();
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;
}

void console_menu::write_line(const std::string& msg)
{
	needs_update = true;
	console_out.emplace_back(msg);
	if (console_out.size() > LINES)
	{
        scroll = console_out.size() - LINES;
    }
	else
	{
        scroll = 0;
    }
}

void console_menu::scroll_dir(int dir)
{
	int new_scroll = scroll + dir;
	new_scroll = std::min(std::max(new_scroll, 0), std::max(0, static_cast<int>(console_out.size()) - LINES));

	if (scroll != new_scroll)
	{
		scroll = new_scroll;
		needs_update = true;
	}
}