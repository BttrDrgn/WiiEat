#include "menus.hpp"
#include "../main.hpp"
#include <format/format.hpp>

bool menus::gui_halt = true;
lwp_t menus::gui_thread = LWP_THREAD_NULL;
menus::state menus::current_menu = MENU_NONE;
menus::state menus::prev_menu = MENU_NONE;
gui_window* menus::main_window = NULL;
gui_image_data* menus::pointer[4];
gui_image* menus::bg_img = NULL;
bool menus::shutdown = false;

const gui_image_data* bg_img = NULL;
const gui_image* bg = NULL;

void menus::initialize(state menu)
{
    menus::init_gui_threads();
    
    menus::current_menu = menu;

	#ifdef HW_RVL
	menus::pointer[0] = new gui_image_data(player1_point_png);
	menus::pointer[1] = new gui_image_data(player2_point_png);
	menus::pointer[2] = new gui_image_data(player3_point_png);
	menus::pointer[3] = new gui_image_data(player4_point_png);
	#endif

	menus::main_window = new gui_window(screen_width, screen_height);

	menus::bg_img = new gui_image(screen_width, screen_height, (GXColor){0xE1, 0xE1, 0xE1, 0xFF});
	menus::main_window->append(menus::bg_img);

	menus::update();
    menus::shutdown_app();
}

void menus::update()
{
	menus::resume_gui();

    while(menus::current_menu != menus::state::MENU_EXIT)
	{
		switch (menus::current_menu)
		{
            case menus::state::MENU_CONSOLE:
                menus::current_menu = console_menu::update();
                break;

            case menus::state::MENU_ADDRESS:
                menus::current_menu = address_menu::update();
                break;

            case menus::state::MENU_RESTAURANT:
                menus::current_menu = restaurant_menu::update();
                break;

			case menus::state::MENU_MAIN:
			default:
				menus::current_menu = main_menu::update();
				break;
		}
	}
}

void menus::shutdown_app()
{
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

void* menus::update_gui(void *arg)
{
    int i;

    while(1)
    {
        if(menus::gui_halt)
        {
            LWP_SuspendThread(menus::gui_thread);
        }
        else
        {
            update_pads();
            menus::main_window->Draw();

            for(i=3; i >= 0; i--) // so that player 1's cursor appears on top!
            {
                if(userInput[i].wpad->ir.valid)
                {
                    menu_draw_img(userInput[i].wpad->ir.x-48, userInput[i].wpad->ir.y-48,
                        96, 96, pointer[i]->get_image(), userInput[i].wpad->ir.angle, 1, 1, 255);
                }
                do_rumble(i);
            }

            menu_render();

            for(i=0; i < 4; i++)
            {
                menus::main_window->Update(&userInput[i]);
            }

            if(shutdown)
            {
                for(i = 0; i <= 255; i += 10)
                {
                    menus::main_window->Draw();
                    menu_draw_rect(0,0,screen_width,screen_height,(GXColor){0, 0, 0, i},1);
                    menu_render();
                }
                main::shutdown_app();
            }
        }
    }
    return NULL;
}

void menus::keyboard(char* var, u16 maxlen)
{
    gui_image_data bg(keyboard_background_png);
    gui_image bg_img(&bg);
    bg_img.set_alignment(ALIGN_MIDDLE, ALIGN_MIDDLE);
    bg_img.set_scale(3.0f);
    gui_keyboard keyboard(var, maxlen, &bg_img);

    gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
    gui_image_data button(button_small_png);
    gui_image_data button_hover(button_small_hover_png);
    gui_trigger trig_a;
    trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

    gui_text cancelBtnTxt("Quit", 22, (GXColor){0, 0, 0, 255});
    gui_image cancelBtnImg(&button);
    gui_image cancelBtnImgOver(&button_hover);
    gui_button cancelBtn(button.get_width(), button.get_height());
    cancelBtn.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
    cancelBtn.set_position(25, -25);
    cancelBtn.set_label(&cancelBtnTxt);
    cancelBtn.set_image(&cancelBtnImg);
    cancelBtn.set_image_hover(&cancelBtnImgOver);
    cancelBtn.set_sound_hover(&btn_sound_hover);
    cancelBtn.set_trigger(&trig_a);
    cancelBtn.set_effect_grow();
    keyboard.append(&cancelBtn);

    gui_text okBtnTxt("OK", 22, (GXColor){0, 0, 0, 255});
    gui_image okBtnImg(&button);
    gui_image okBtnImgOver(&button_hover);
    gui_button okBtn(button.get_width(), button.get_height());
    okBtn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
    okBtn.set_position(-25, -25);
    okBtn.set_label(&okBtnTxt);
    okBtn.set_image(&okBtnImg);
    okBtn.set_image_hover(&okBtnImgOver);
    okBtn.set_sound_hover(&btn_sound_hover);
    okBtn.set_trigger(&trig_a);
    okBtn.set_effect_grow();
    keyboard.append(&okBtn);

	keyboard.set_effect(EFFECT_FADE, 30);
    menus::halt_gui();
    menus::main_window->set_state(STATE_DISABLED);
    menus::main_window->append(&keyboard);
    menus::main_window->change_focus(&keyboard);
    menus::resume_gui();

    int save = -1;
    while(save == -1)
    {
        usleep(100);

        if(okBtn.get_state() == STATE_CLICKED)
        {
            save = 1;
        }
        else if(cancelBtn.get_state() == STATE_CLICKED)
        {
            save = 0;
        }
    }

    if(save)
    {
        snprintf(var, maxlen, "%s", keyboard.kbtextstr);
    }

    keyboard.set_effect(EFFECT_FADE, -30);
	while(keyboard.get_effect() > 0) usleep(100);

    menus::halt_gui();
    menus::main_window->remove(&keyboard);
    menus::main_window->set_state(STATE_DEFAULT);
    menus::resume_gui();
}

void menus::num_keyboard(char* var, u16 maxlen)
{
    gui_image_data bg(keyboard_background_png);
    gui_image bg_img(&bg);
    bg_img.set_alignment(ALIGN_MIDDLE, ALIGN_MIDDLE);
    bg_img.set_scale(3.0f);
    gui_num_keyboard keyboard(var, maxlen, &bg_img);

    gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
    gui_image_data button(button_small_png);
    gui_image_data button_hover(button_small_hover_png);
    gui_trigger trig_a;
    trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

    gui_text cancelBtnTxt("Quit", 22, (GXColor){0, 0, 0, 255});
    gui_image cancelBtnImg(&button);
    gui_image cancelBtnImgOver(&button_hover);
    gui_button cancelBtn(button.get_width(), button.get_height());
    cancelBtn.set_alignment(ALIGN_LEFT, ALIGN_BOTTOM);
    cancelBtn.set_position(25, -25);
    cancelBtn.set_label(&cancelBtnTxt);
    cancelBtn.set_image(&cancelBtnImg);
    cancelBtn.set_image_hover(&cancelBtnImgOver);
    cancelBtn.set_sound_hover(&btn_sound_hover);
    cancelBtn.set_trigger(&trig_a);
    cancelBtn.set_effect_grow();
    keyboard.append(&cancelBtn);

    gui_text okBtnTxt("OK", 22, (GXColor){0, 0, 0, 255});
    gui_image okBtnImg(&button);
    gui_image okBtnImgOver(&button_hover);
    gui_button okBtn(button.get_width(), button.get_height());
    okBtn.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
    okBtn.set_position(-25, -25);
    okBtn.set_label(&okBtnTxt);
    okBtn.set_image(&okBtnImg);
    okBtn.set_image_hover(&okBtnImgOver);
    okBtn.set_sound_hover(&btn_sound_hover);
    okBtn.set_trigger(&trig_a);
    okBtn.set_effect_grow();
    keyboard.append(&okBtn);

	keyboard.set_effect(EFFECT_FADE, 30);
    menus::halt_gui();
    menus::main_window->set_state(STATE_DISABLED);
    menus::main_window->append(&keyboard);
    menus::main_window->change_focus(&keyboard);
    menus::resume_gui();

    int save = -1;
    while(save == -1)
    {
        usleep(100);

        if(okBtn.get_state() == STATE_CLICKED)
        {
            save = 1;
        }
        else if(cancelBtn.get_state() == STATE_CLICKED)
        {
            save = 0;
        }
    }

    if(save)
    {
        snprintf(var, maxlen, "%s", keyboard.kbtextstr);
    }

    keyboard.set_effect(EFFECT_FADE, -30);
	while(keyboard.get_effect() > 0) usleep(100);

    menus::halt_gui();
    menus::main_window->remove(&keyboard);
    menus::main_window->set_state(STATE_DEFAULT);
    menus::resume_gui();
}

int menus::window_prompt(const char *title, const char *msg, const char *btn1Label, const char* btn2Label)
{
	int choice = -1;
	bool has_btn_2 = strcmp(btn2Label, "");

	gui_window prompt_window(448,288);
	prompt_window.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	prompt_window.set_position(0, -10);
	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	gui_image_data button(button_small_png);
	gui_image_data button_hover(button_small_hover_png);
	gui_trigger trig_a;
	trig_a.set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	gui_image_data dialogBox(dialogue_box_png);
	gui_image dialogBoxImg(&dialogBox);
	prompt_window.append(&dialogBoxImg);

	gui_text titleTxt(title, 26, (GXColor){0, 0, 0, 255});
	titleTxt.set_alignment(ALIGN_CENTER, ALIGN_TOP);
	titleTxt.set_position(0,40);
	prompt_window.append(&titleTxt);

	gui_text msgTxt(msg, 22, (GXColor){0, 0, 0, 255});
	msgTxt.set_alignment(ALIGN_CENTER, ALIGN_MIDDLE);
	msgTxt.set_position(0,-20);
	msgTxt.SetWrap(true, 400);
	prompt_window.append(&msgTxt);

	gui_text btn1Txt(btn1Label, 22, (GXColor){0, 0, 0, 255});
	gui_image btn1Img(&button);
	gui_image btn1ImgOver(&button_hover);
	gui_button btn1(button.get_width(), button.get_height());
	btn1.set_label(&btn1Txt);
	btn1.set_image(&btn1Img);
	btn1.set_image_hover(&btn1ImgOver);
	btn1.set_sound_hover(&btn_sound_hover);
	btn1.set_trigger(&trig_a);
	btn1.set_state(STATE_SELECTED);
	btn1.set_effect_grow();

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
	prompt_window.append(&btn1);

	gui_button btn2(button.get_width(), button.get_height());
	gui_text btn2Txt(btn2Label, 22, (GXColor){0, 0, 0, 255});

	if(has_btn_2)
	{
		gui_image btn2Img(&button);
		gui_image btn2ImgOver(&button_hover);
		btn2.set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		btn2.set_position(-20, -25);
		btn2.set_label(&btn2Txt);
		btn2.set_image(&btn2Img);
		btn2.set_image_hover(&btn2ImgOver);
		btn2.set_sound_hover(&btn_sound_hover);
		btn2.set_trigger(&trig_a);
		btn2.set_state(STATE_SELECTED);
		btn2.set_effect_grow();
		prompt_window.append(&btn2);
	}

	prompt_window.set_effect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	menus::halt_gui();
	menus::main_window->set_state(STATE_DISABLED);
	menus::main_window->append(&prompt_window);
	menus::main_window->change_focus(&prompt_window);
	menus::resume_gui();

	while(choice == -1)
	{
		usleep(100);

		if(btn1.get_state() == STATE_CLICKED)
		{
			choice = 1;
		}
		else if(btn2Label && btn2.get_state() == STATE_CLICKED)
		{
			choice = 0;
		}
	}

	prompt_window.set_effect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(prompt_window.get_effect() > 0) usleep(100);
	menus::halt_gui();
	menus::main_window->remove(&prompt_window);
	menus::main_window->set_state(STATE_DEFAULT);
	menus::resume_gui();
	return choice;
}