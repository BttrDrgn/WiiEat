
#include "main_menu.hpp"

static gui_image* bg_img = NULL;
char email_address[64];
char password[64];

int WindowPrompt(const char *title, const char *msg, const char *btn1Label, const char *btn2Label)
{
	int choice = -1;

	gui_window promptWindow(448,288);
	promptWindow.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	promptWindow.set_position(0, -10);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data btnOutline(button_png);
	gui_image_data btnOutlineOver(button_hover_png);
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
	gui_image btn1Img(&btnOutline);
	gui_image btn1ImgOver(&btnOutlineOver);
	gui_button btn1(btnOutline.GetWidth(), btnOutline.GetHeight());

	if(btn2Label)
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
	btn1.SetState(STATE_SELECTED);
	btn1.SetEffectGrow();

	gui_text btn2Txt(btn2Label, 22, (GXColor){0, 0, 0, 255});
	gui_image btn2Img(&btnOutline);
	gui_image btn2ImgOver(&btnOutlineOver);
	gui_button btn2(btnOutline.GetWidth(), btnOutline.GetHeight());
	btn2.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	btn2.set_position(-20, -25);
	btn2.set_label(&btn2Txt);
	btn2.set_image(&btn2Img);
	btn2.set_image_hover(&btn2ImgOver);
	btn2.set_sound_hover(&btn_sound_hover);
	btn2.set_trigger(&trig_a);
	btn2.SetEffectGrow();

	promptWindow.append(&dialogBoxImg);
	promptWindow.append(&titleTxt);
	promptWindow.append(&msgTxt);
	promptWindow.append(&btn1);

	if(btn2Label)
		promptWindow.append(&btn2);

	promptWindow.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	menus::halt_gui();
	menus::main_window->SetState(STATE_DISABLED);
	menus::main_window->append(&promptWindow);
	menus::main_window->ChangeFocus(&promptWindow);
	menus::resume_gui();

	while(choice == -1)
	{
		usleep(100);

		if(btn1.get_state() == STATE_CLICKED)
			choice = 1;
		else if(btn2.get_state() == STATE_CLICKED)
			choice = 0;
	}

	promptWindow.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(promptWindow.GetEffect() > 0) usleep(100);
	menus::halt_gui();
	menus::main_window->remove(&promptWindow);
	menus::main_window->SetState(STATE_DEFAULT);
	menus::resume_gui();
	return choice;
}

static void OnScreenKeyboard(char * var, u16 maxlen)
{
	int save = -1;

	GuiKeyboard keyboard(var, maxlen);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data btnOutline(button_png);
	gui_image_data btnOutlineOver(button_hover_png);
	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_text okBtnTxt("OK", 22, (GXColor){0, 0, 0, 255});
	gui_image okBtnImg(&btnOutline);
	gui_image okBtnImgOver(&btnOutlineOver);
	gui_button okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());

	okBtn.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.set_position(25, -25);

	okBtn.set_label(&okBtnTxt);
	okBtn.set_image(&okBtnImg);
	okBtn.set_image_hover(&okBtnImgOver);
	okBtn.set_sound_hover(&btn_sound_hover);
	okBtn.set_trigger(&trig_a);
	okBtn.SetEffectGrow();

	gui_text cancelBtnTxt("Cancel", 22, (GXColor){0, 0, 0, 255});
	gui_image cancelBtnImg(&btnOutline);
	gui_image cancelBtnImgOver(&btnOutlineOver);
	gui_button cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.set_position(-25, -25);
	cancelBtn.set_label(&cancelBtnTxt);
	cancelBtn.set_image(&cancelBtnImg);
	cancelBtn.set_image_hover(&cancelBtnImgOver);
	cancelBtn.set_sound_hover(&btn_sound_hover);
	cancelBtn.set_trigger(&trig_a);
	cancelBtn.SetEffectGrow();

	keyboard.append(&okBtn);
	keyboard.append(&cancelBtn);

	menus::halt_gui();
	menus::main_window->SetState(STATE_DISABLED);
	menus::main_window->append(&keyboard);
	menus::main_window->ChangeFocus(&keyboard);
	menus::resume_gui();

	while(save == -1)
	{
		usleep(100);

		if(okBtn.get_state() == STATE_CLICKED)
			save = 1;
		else if(cancelBtn.get_state() == STATE_CLICKED)
			save = 0;
	}

	if(save)
	{
		snprintf(var, maxlen, "%s", keyboard.kbtextstr);
	}

	menus::halt_gui();
	menus::main_window->remove(&keyboard);
	menus::main_window->SetState(STATE_DEFAULT);
	menus::resume_gui();
}

menus::state main_menu::update()
{
	menus::state menu = menus::state::MENU_NONE;

	gui_window w(screenwidth, screenheight);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data btnSmall(button_small_png);
	gui_image_data inputBox(input_box_png);
	gui_image_data inputBoxHover(input_box_hover_png);
	gui_image_data btnSmallHover(button_small_over_png);
	gui_image_data circle_btn(circle_button_png);
	gui_image_data circle_btn_hover(circle_button_hover_png);
	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);

	gui_image_data logoImage(wiieat_logo_png);
	gui_image logo(&logoImage);
	logo.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	logo.set_position(50, 50);
	w.append(&logo);

	gui_text poweredByTxt("Powered by Grubhub", 12, (GXColor){0x50, 0x50, 0x50, 255});
	poweredByTxt.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	poweredByTxt.set_position(60, 100);
	w.append(&poweredByTxt);

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

#ifdef DEBUG
	gui_text consoleBtnText("</>", 20, (GXColor){0, 0, 0, 255});
	gui_image consoleBtnImg(&circle_btn);
	gui_image consoleBtnImgOver(&circle_btn_hover);
	gui_button consoleBtn(circle_btn.GetWidth(), circle_btn.GetHeight());
	consoleBtn.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
	consoleBtn.set_position(35, -35);
	consoleBtn.set_label(&consoleBtnText);
	consoleBtn.set_image(&consoleBtnImg);
	consoleBtn.set_image_hover(&consoleBtnImgOver);
	consoleBtn.set_sound_hover(&btn_sound_hover);
	consoleBtn.set_trigger(&trig_a);
	consoleBtn.set_trigger(&trig_home);
	consoleBtn.set_scale(0.75f);
	w.append(&consoleBtn);
#endif

	gui_text loginBtnText("Login", 22, (GXColor){0, 0, 0, 255});
	gui_image loginBtnImg(&btnSmall);
	gui_image loginBtnImgOver(&btnSmallHover);
	gui_button loginBtn(btnSmall.GetWidth(), btnSmall.GetHeight());
	loginBtn.set_alignment(ALIGN_CENTER, ALIGN_BOTTOM);
	loginBtn.set_position(0, -35);
	loginBtn.set_label(&loginBtnText);
	loginBtn.set_image(&loginBtnImg);
	loginBtn.set_image_hover(&loginBtnImgOver);
	loginBtn.set_sound_hover(&btn_sound_hover);
	loginBtn.set_trigger(&trig_a);
	loginBtn.SetEffectGrow();
	w.append(&loginBtn);

	gui_text emailText("Email:", 24, (GXColor){0, 0, 0, 255});
	emailText.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	emailText.set_position(-125, 175);
	w.append(&emailText);

	gui_text emailPromptText(email_address, 12, (GXColor){0, 0, 0, 255});
	gui_image emailPromptImg(&inputBox);
	gui_image emailPromptImgOver(&inputBoxHover);
	emailPromptImg.set_scale_x(1.5f);
	emailPromptImgOver.set_scale_x(1.5f);
	gui_button emailPrompt(inputBox.GetWidth(), inputBox.GetHeight());
	emailPrompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	emailPrompt.set_position(35, 150);
	emailPrompt.set_label(&emailPromptText);
	emailPrompt.set_image(&emailPromptImg);
	emailPrompt.set_image_hover(&emailPromptImgOver);
	emailPrompt.set_sound_hover(&btn_sound_hover);
	emailPrompt.set_trigger(&trig_a);
	w.append(&emailPrompt);

	gui_text passwordText("Password:", 24, (GXColor){0, 0, 0, 255});
	passwordText.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	passwordText.set_position(-150, 250);
	w.append(&passwordText);

	gui_text passwordPromptText(password, 12, (GXColor){0, 0, 0, 255});
	gui_image passwordPromptImg(&inputBox);
	gui_image passwordPromptImgOver(&inputBoxHover);
	passwordPromptImg.set_scale_x(1.5f);
	passwordPromptImgOver.set_scale_x(1.5f);
	gui_button passwordPrompt(inputBox.GetWidth(), inputBox.GetHeight());
	passwordPrompt.set_alignment(ALIGN_CENTER, ALIGN_CENTER);
	passwordPrompt.set_position(35, 225);
	passwordPrompt.set_label(&passwordPromptText);
	passwordPrompt.set_image(&passwordPromptImg);
	passwordPrompt.set_image_hover(&passwordPromptImgOver);
	passwordPrompt.set_sound_hover(&btn_sound_hover);
	passwordPrompt.set_trigger(&trig_a);
	w.append(&passwordPrompt);

	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		if(exit_btn.get_state() == STATE_CLICKED)
		{
			menu = menus::state::MENU_EXIT;
		}
		else if(consoleBtn.get_state() == STATE_CLICKED)
		{
			menus::prev_menu = menus::state::MENU_MAIN;
			menu = menus::state::MENU_CONSOLE;
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;
}

void main_menu::initialize(menus::state menu)
{
	menus::current_menu = menu;

	#ifdef HW_RVL
	menus::pointer[0] = new gui_image_data(player1_point_png);
	menus::pointer[1] = new gui_image_data(player2_point_png);
	menus::pointer[2] = new gui_image_data(player3_point_png);
	menus::pointer[3] = new gui_image_data(player4_point_png);
	#endif

	menus::main_window = new gui_window(screenwidth, screenheight);

	bg_img = new gui_image(screenwidth, screenheight, (GXColor){0xE1, 0xE1, 0xE1, 0xFF});
	menus::main_window->append(bg_img);

	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	menus::resume_gui();

	while(menus::current_menu != menus::state::MENU_EXIT)
	{
		switch (menus::current_menu)
		{
			case menus::state::MENU_CONSOLE:
				menus::current_menu = console_menu::update();
				break;

			case menus::state::MENU_MAIN:
			default:
				menus::current_menu = main_menu::update();
				break;
		}
	}

	menus::resume_gui();
	menus::shutdown = true;
	while(true) 
	{
		usleep(100);
	}

	menus::halt_gui();

	delete bg_img;
	delete menus::main_window;

	delete menus::pointer[0];
	delete menus::pointer[1];
	delete menus::pointer[2];
	delete menus::pointer[3];

	menus::main_window = NULL;
}
