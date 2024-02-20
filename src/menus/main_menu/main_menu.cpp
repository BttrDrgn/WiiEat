
#include "main_menu.hpp"

#define MAX_BUF_LEN 64
char email_address[MAX_BUF_LEN];
char password[MAX_BUF_LEN];
char visible_password[MAX_BUF_LEN];

int WindowPrompt(const char *title, const char *msg, const char *btn1Label, const char* btn2Label = "")
{
	int choice = -1;
	bool has_btn_2 = strcmp(btn2Label, "");

	gui_window promptWindow(448,288);
	promptWindow.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	promptWindow.set_position(0, -10);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data button(button_small_png);
	gui_image_data button_hover(button_small_hover_png);
	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_image_data dialogBox(dialogue_box_png);
	gui_image dialogBoxImg(&dialogBox);

	gui_text titleTxt(title, 26, (GXColor){0, 0, 0, 255});
	titleTxt.set_alignment(ALIGN_CENTER, ALIGN_TOP);
	titleTxt.set_position(0,40);
	gui_text msgTxt(msg, 22, (GXColor){0, 0, 0, 255});
	msgTxt.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	msgTxt.set_position(0,-20);
	msgTxt.SetWrap(true, 400);

	gui_text btn1Txt(btn1Label, 22, (GXColor){0, 0, 0, 255});
	gui_image btn1Img(&button);
	gui_image btn1ImgOver(&button_hover);
	gui_button btn1(button.get_width(), button.get_height());

	if(has_btn_2)
	{
		btn1.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
		btn1.set_position(20, -25);
	}
	else
	{
		btn1.set_alignment(ALIGN_CENTER, ALIGN_BOTTOM);
		btn1.set_position(0, -25);
	}

	btn1.set_label(&btn1Txt);
	btn1.set_image(&btn1Img);
	btn1.set_image_hover(&btn1ImgOver);
	btn1.set_sound_hover(&btn_sound_hover);
	btn1.set_trigger(&trig_a);
	btn1.set_state(STATE_SELECTED);
	btn1.set_effect_grow();

	gui_button btn2;
	if(has_btn_2)
	{
		gui_text btn2Txt(btn2Label, 22, (GXColor){0, 0, 0, 255});
		gui_image btn2Img(&button);
		gui_image btn2ImgOver(&button_hover);
		gui_button btn2(button.get_width(), button.get_height());
		btn2.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		btn2.set_position(-20, -25);
		btn2.set_label(&btn2Txt);
		btn2.set_image(&btn2Img);
		btn2.set_image_hover(&btn2ImgOver);
		btn2.set_sound_hover(&btn_sound_hover);
		btn2.set_trigger(&trig_a);
		btn2.set_effect_grow();
		promptWindow.append(&btn2);
	}

	promptWindow.append(&dialogBoxImg);
	promptWindow.append(&titleTxt);
	promptWindow.append(&msgTxt);
	promptWindow.append(&btn1);

	promptWindow.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	menus::halt_gui();
	menus::main_window->set_state(STATE_DISABLED);
	menus::main_window->append(&promptWindow);
	menus::main_window->change_focus(&promptWindow);
	menus::resume_gui();

	while(choice == -1)
	{
		usleep(100);

		if(btn1.get_state() == STATE_CLICKED)
		{
			choice = 1;
		}
		else if(has_btn_2 && btn2.get_state() == STATE_CLICKED)
		{
			choice = 0;
		}
	}

	promptWindow.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(promptWindow.GetEffect() > 0) usleep(100);
	menus::halt_gui();
	menus::main_window->remove(&promptWindow);
	menus::main_window->set_state(STATE_DEFAULT);
	menus::resume_gui();
	return choice;
}

menus::state main_menu::update()
{
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

	gui_text exit_btn_txt("X", 22, (GXColor){0, 0, 0, 255});
	gui_image exit_btn_img(&circle_btn);
	gui_image exit_btn_img_hover(&circle_btn_hover);
	gui_button exit_btn(circle_btn.get_width(), circle_btn.get_height());
	exit_btn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	exit_btn.set_position(-35, -35);
	exit_btn.set_label(&exit_btn_txt);
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
	email_text.set_position(-150, 175);
	w.append(&email_text);

	gui_text email_prompt_text(email_address, 18, (GXColor){0, 0, 0, 255});
	email_prompt_text.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	email_prompt_text.set_position(25, 0);
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
	password_txt.set_position(-175, 250);
	w.append(&password_txt);

	gui_text password_prompt_txt(visible_password, 18, (GXColor){0, 0, 0, 255});
	password_prompt_txt.set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
	password_prompt_txt.set_position(25, 0);
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
			if(!api::auth_request(email_address, password))
			{
				WindowPrompt
				(
					"Invalid Login",
					"Unable to login to the Grubhub account using the provided credentials.",
					"Ok"
				);
			}
			else
			{
				menu = menus::next(menus::state::MENU_ADDRESS);
			}
		}
		else if(console_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::next(menus::state::MENU_CONSOLE);
		}
		else if(email_prompt.get_state() == STATE_CLICKED)
		{
			menus::keyboard(email_address, MAX_BUF_LEN);
			menu = menus::current_menu;
		}
		else if(password_prompt.get_state() == STATE_CLICKED)
		{
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