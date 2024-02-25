
#include "main_menu.hpp"

#define MAX_BUF_LEN 64
char email_address[MAX_BUF_LEN];
char password[MAX_BUF_LEN];
char visible_password[MAX_BUF_LEN];

bool main_menu::initialized = false;

void main_menu::initialize()
{
	memset(email_address, '\0', MAX_BUF_LEN);
	memset(password, '\0', MAX_BUF_LEN);
	memset(visible_password, '\0', MAX_BUF_LEN);
	main_menu::initialized = true;
}

menus::state main_menu::update()
{
	if(!main_menu::initialized)
	{
		main_menu::initialize();
	}

	menus::state menu = menus::state::MENU_NONE;

	gui_window w(screen_width, screen_height);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data btnSmall(button_small_png);
	gui_image_data btnSmallHover(button_small_hover_png);
	gui_image_data input_box(input_box_png);
	gui_image_data input_box_hover(input_box_hover_png);
	gui_image_data circle_btn(circle_button_png);
	gui_image_data circle_btn_hover(circle_button_hover_png);

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
	gui_image exit_btn_img_hover(&exit_btn_hover_img_data);
	gui_button exit_btn(exit_btn.get_width(), exit_btn.get_height());
	exit_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	exit_btn.set_position(-35, -35);
	exit_btn.set_image(&exit_btn_img);
	exit_btn.set_image_hover(&exit_btn_img_hover);
	exit_btn.set_sound_hover(&btn_sound_hover);
	exit_btn.set_trigger(&trig_a);
	exit_btn.set_scale(0.75f);
	w.append(&exit_btn);

	gui_text login_btnText("Login", 22, (GXColor){0, 0, 0, 255});
	gui_image login_btnImg(&btnSmall);
	gui_image login_btnImgOver(&btnSmallHover);
	gui_button login_btn(btnSmall.get_width(), btnSmall.get_height());
	login_btn.set_alignment(ALIGN_CENTER, ALIGN_BOTTOM);
	login_btn.set_position(0, -35);
	login_btn.set_label(&login_btnText);
	login_btn.set_image(&login_btnImg);
	login_btn.set_image_hover(&login_btnImgOver);
	login_btn.set_sound_hover(&btn_sound_hover);
	login_btn.set_trigger(&trig_a);
	login_btn.set_effect_grow();
	w.append(&login_btn);

	gui_text email_text("Email:", 24, (GXColor){0, 0, 0, 255});
	email_text.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	email_text.set_position(-150, 190);
	w.append(&email_text);

	gui_text email_prompt_text(email_address, 18, (GXColor){0, 0, 0, 255});
	email_prompt_text.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	email_prompt_text.set_position(45, 0);
	email_prompt_text.set_max_width(15 * 20);
	gui_image email_prompt_img(&input_box);
	gui_image email_prompt_img_hover(&input_box_hover);
	email_prompt_img.set_scale_x(1.5f);
	email_prompt_img_hover.set_scale_x(1.5f);
	gui_button email_prompt(input_box.get_width(), input_box.get_height());
	email_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	email_prompt.set_position(20, 150);
	email_prompt.set_label(&email_prompt_text);
	email_prompt.set_image(&email_prompt_img);
	email_prompt.set_image_hover(&email_prompt_img_hover);
	email_prompt.set_sound_hover(&btn_sound_hover);
	email_prompt.set_trigger(&trig_a);
	w.append(&email_prompt);

	gui_text password_txt("Password:", 24, (GXColor){0, 0, 0, 255});
	password_txt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	password_txt.set_position(-175, 265);
	w.append(&password_txt);

	gui_text password_prompt_txt(visible_password, 18, (GXColor){0, 0, 0, 255});
	password_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	password_prompt_txt.set_position(45, 0);
	password_prompt_txt.set_max_width(15 * 20);
	gui_image password_promptImg(&input_box);
	gui_image password_prompt_img_hover(&input_box_hover);
	password_promptImg.set_scale_x(1.5f);
	password_prompt_img_hover.set_scale_x(1.5f);
	gui_button password_prompt(input_box.get_width(), input_box.get_height());
	password_prompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	password_prompt.set_position(20, 225);
	password_prompt.set_label(&password_prompt_txt);
	password_prompt.set_image(&password_promptImg);
	password_prompt.set_image_hover(&password_prompt_img_hover);
	password_prompt.set_sound_hover(&btn_sound_hover);
	password_prompt.set_trigger(&trig_a);
	w.append(&password_prompt);

	
#ifdef DEBUG
	gui_text console_btnText("</>", 20, (GXColor){0, 0, 0, 255});
	gui_image console_btnImg(&circle_btn);
	gui_image console_btnImgOver(&circle_btn_hover);
	gui_button console_btn(circle_btn.get_width(), circle_btn.get_height());
	console_btn.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
	console_btn.set_position(35, -35);
	console_btn.set_label(&console_btnText);
	console_btn.set_image(&console_btnImg);
	console_btn.set_image_hover(&console_btnImgOver);
	console_btn.set_sound_hover(&btn_sound_hover);
	console_btn.set_trigger(&trig_a);
	console_btn.set_scale(0.75f);
	w.append(&console_btn);
#endif


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
		else if(exit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_EXIT);
		}
		else if(login_btn.get_state() == STATE_CLICKED)
		{
			auto err = api::auth_request(email_address, password);
			if(err != api::error::NONE && err != api::error::EMAIL_2FA && err != api::error::UNAUTHORIZED)
			{
				menus::window_prompt
				(
					"Invalid Login",
					"Unable to login to the Grubhub account using the provided credentials.",
					"Ok"
				);
			}
			else if(err == api::error::UNAUTHORIZED)
			{
				menus::window_prompt
				(
					"Unauthorized",
					"Your account might have been temporarily blocked. Please try to access grubhub.com and login to unblock your account.",
					"Ok"
				);
			}
			else if(err == api::error::EMAIL_2FA)
			{
				auto choice = menus::window_prompt
				(
					"Email Verification",
					"GrubHub is requiring you verify your email to login.",
					"Send",
					"Back"
				);

				if(choice == 1)
				{
					api::confirmation_code_request(email_address);

				retry:
					char code[7];
					memset(code, '\0', 7);
					menus::num_keyboard(code, 7);

					err = api::auth_code_request(email_address, code);
					if(err == api::error::NONE)
					{
						menu = menus::next(menus::state::MENU_ADDRESS);

						w.set_effect(EFFECT_FADE, -25);
						while(w.get_effect() > 0) usleep(100);
					}
					else
					{
						choice = menus::window_prompt
						(
							"Email Verification Failed",
							"The code you entered was not correct.",
							"Retry",
							"Back"
						);

						if(choice == 1)
						{
							goto retry;
						}
					}
				}
				else
				{
					login_btn.reset_state();
					continue;
				}
			}
			else
			{
				menu = menus::next(menus::state::MENU_ADDRESS);

				w.set_effect(EFFECT_FADE, -25);
				while(w.get_effect() > 0) usleep(100);
			}
		}
		else if(console_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_CONSOLE);
		}
		else if(email_prompt.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);
			
			menus::keyboard(email_address, MAX_BUF_LEN);
			menu = menus::current_menu;
		}
		else if(password_prompt.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);
			
			menus::keyboard(password, MAX_BUF_LEN);

			size_t len = strlen(password);
			for (size_t i = 0; i < len; i++)
			{
				visible_password[i] = '*';
			}
			visible_password[len] = '\0';

			menu = menus::current_menu;
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;
}