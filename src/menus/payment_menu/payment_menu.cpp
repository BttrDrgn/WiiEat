#include "payment_menu.hpp"

std::vector<credit_card*> payment_menu::credit_cards {};
std::vector<gui_button*> payment_menu::buttons;

void payment_menu::load_payments(const std::string& uu_id)
{
	for (auto cc_ptr : credit_cards)
	{
		delete cc_ptr;
	}
	
	credit_cards.clear();

	json json = 0;
	auto resp = api::get_payments(uu_id, json);
	if (resp != api::error::NONE)
	{
		console_menu::write_line("Error loading payments");
		return;
	}

	auto ccs = json["credit_cards"];
	for(int i = 0; i < ccs.size(); ++i)
	{
		auto cc = ccs[i];
		credit_cards.emplace_back(new credit_card(
			cc["id"].get<std::string>(),
			cc["diner_id"].get<std::string>(),
			cc["credit_card_type"].get<std::string>(),
			cc["credit_card_last4"].get<std::string>()
		));

		console_menu::write_line(format::va("Added card id %s", credit_cards[i]->id.c_str()).c_str());
	}
}

menus::state payment_menu::update()
{
    menus::state menu = menus::state::MENU_NONE;

	gui_window w(screen_width, screen_height);

	gui_sound btn_sound_hover(button_over_pcm, button_over_pcm_size, SOUND_PCM);

	gui_image_data input_box(input_box_png);
	gui_image_data input_box_hover(input_box_hover_png);

	gui_image_data btn(main::dark_mode ? button_dark_png : button_png);
	gui_image_data btn_hover(main::dark_mode ? button_dark_hover_png : button_hover_png);
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

	gui_text active_card_text(format::va("Active Card: %s", api::active_card_last_4.c_str()).c_str(), 20, (GXColor){0, 0, 0, 255});
	active_card_text.set_alignment(ALIGN_LEFT, ALIGN_TOP);
	active_card_text.set_position(60, 150);
	w.append(&active_card_text);

	gui_image_data exit_btn_img_data(exit_button_png);
	gui_image_data exit_btn_hover_img_data(exit_button_hover_png);
	gui_image exit_btn_img(&exit_btn_img_data);
	gui_image exit_btn_hover_img(&exit_btn_hover_img_data);
	gui_button exit_btn(exit_btn_img_data.get_width(), exit_btn_img_data.get_height());
	exit_btn.set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	exit_btn.set_position(-25, 15);
	exit_btn.set_image(&exit_btn_img);
	exit_btn.set_image_hover(&exit_btn_hover_img);
	exit_btn.set_sound_hover(&btn_sound_hover);
	exit_btn.set_trigger(&trig_a);
	exit_btn.set_scale(0.75f);
	exit_btn.set_effect_grow();
	w.append(&exit_btn);

	gui_trigger trig_home;
	trig_home.set_button_only_trigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	gui_button home_btn(0, 0);
	home_btn.set_position(-1000, -1000);
	home_btn.set_trigger(&trig_home);
	w.append(&home_btn);

	for(int i = 0; i < 3; ++i)
	{
		gui_button* new_btn = new gui_button(btn.get_width(), btn.get_height());

		float y_pos = 200 + i * 68;
		new_btn->set_alignment(ALIGN_LEFT, ALIGN_CENTER);
		new_btn->set_position(48, y_pos);
		new_btn->set_sound_hover(&btn_sound_hover);
		new_btn->set_trigger(&trig_a);
		new_btn->set_visible(false);
		new_btn->set_effect_grow();

		payment_menu::buttons.emplace_back(new_btn);
		w.append(payment_menu::buttons[i]);
	}

	for(int i = 0; i < 3; ++i)
	{
		if(i + 1 > credit_cards.size()) break;

		auto text = new gui_text(format::va("%s %s", format::to_upper(payment_menu::credit_cards[i]->type).c_str(), payment_menu::credit_cards[i]->last_4.c_str()).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text->set_max_width(200);
		payment_menu::buttons[i]->set_label(text);

		auto text_hover = new gui_text(format::va("%s %s", format::to_upper(payment_menu::credit_cards[i]->type).c_str(), payment_menu::credit_cards[i]->last_4.c_str()).c_str(), 18, (GXColor){0x0, 0x0, 0x0, 255});
		text_hover->set_max_width(200);
		text_hover->set_scroll(true);
		payment_menu::buttons[i]->set_label_hover(text_hover);

		gui_image* new_img = new gui_image(&btn);
		gui_image* new_img_hover = new gui_image(&btn_hover);
		payment_menu::buttons[i]->set_image(new_img);
		payment_menu::buttons[i]->set_image_hover(new_img_hover);
		payment_menu::buttons[i]->set_visible(true);
	}

	w.set_effect(EFFECT_FADE, 25);
	menus::halt_gui();
	menus::main_window->append(&w);
	menus::resume_gui();

	while(menu == menus::state::MENU_NONE)
	{
		usleep(100);

		for(int i = 0; i < buttons.size(); ++i)
		{
			if(payment_menu::buttons[i]->get_state() == STATE_CLICKED)
			{
				w.set_effect(EFFECT_FADE, -25);
				while(w.get_effect() > 0) usleep(100);

				api::set_active_card(payment_menu::credit_cards[i]->last_4, payment_menu::credit_cards[i]->id);
				payment_menu::buttons[i]->reset_state();
				menu = menus::state::MENU_PAYMENT;
				break;
			}
		}


		if(home_btn.get_state() == STATE_CLICKED)
		{
			menu = home_menu::update();
			if(menu == menus::state::MENU_CANCEL) menu = menus::state::MENU_NONE;
		}
		else if (exit_btn.get_state() == STATE_CLICKED)
		{
			w.set_effect(EFFECT_FADE, -25);
			while(w.get_effect() > 0) usleep(100);
			
			menu = menus::state::MENU_RESTAURANT;
		}
	}

	menus::halt_gui();
	menus::main_window->remove(&w);
	return menu;   
}