/****************************************************************************
 * libwiigui
 *
 * BttrDrgn 2024
 *
 * gui_num_keyboard.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

static char tmptxt[MAX_KEYBOARD_DISPLAY];

static char * GetDisplayText(char * t)
{
	if(!t)
		return NULL;

	snprintf(tmptxt, MAX_KEYBOARD_DISPLAY, "%s", t);
	return &tmptxt[0];
}

/**
 * Constructor for the gui_num_keyboard class.
 */

gui_num_keyboard::gui_num_keyboard(char* t, u32 max, gui_image* bg_image)
{
	const int y_offset = 135;
	width = 540;
	height = 400;
	selectable = true;
	focus = 0; // allow focus
	alignmentHor = ALIGN_CENTER;
	alignmentVert = ALIGN_MIDDLE;
	memset(kbtextstr, '\0', 255);
	snprintf(kbtextstr, 255, "%s", t);
	kbtextmaxlen = max;

	Key thekeys[4][3] = {
		{
			{'1','\0'},
			{'2','\0'},
			{'3','\0'},
		},
		{
			{'4','\0'},
			{'5','\0'},
			{'6','\0'},
		},
		{
			{'7','\0'},
			{'8','\0'},
			{'9','\0'},
		},
		{
			{'\0','\0'},
			{'0','\0'},
			{'\0','\0'},
		}
	};
	memcpy(keys, thekeys, sizeof(thekeys));

	if(bg_image != NULL)
	{
    	this->append(bg_image);
	}

	keyTextbox = new gui_image_data(keyboard_textbox_png);
	keyTextboxImg = new gui_image(keyTextbox);
	keyTextboxImg->set_alignment(ALIGN_CENTER, ALIGN_TOP);
	keyTextboxImg->set_position(0, 0);
	this->append(keyTextboxImg);

	kbText = new gui_text(GetDisplayText(kbtextstr), 20, (GXColor){0, 0, 0, 0xff});
	kbText->set_alignment(ALIGN_CENTER, ALIGN_TOP);
	kbText->set_position(0, 13);
	this->append(kbText);

	key = new gui_image_data(keyboard_key_png);
	keyOver = new gui_image_data(keyboard_key_over_png);
	keyMedium = new gui_image_data(keyboard_mediumkey_png);
	keyMediumOver = new gui_image_data(keyboard_mediumkey_over_png);
	keyLarge = new gui_image_data(keyboard_largekey_png);
	keyLargeOver = new gui_image_data(keyboard_largekey_over_png);

	keysound_hover = new gui_sound(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	keysound_click = new gui_sound(button_click_pcm, button_click_pcm_size, SOUND_PCM);

	trig_a = new gui_trigger;
	trig_a->set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trig2 = new gui_trigger;
	trig2->set_simple_trigger(-1, WPAD_BUTTON_2, 0);

	keyBackImg = new gui_image(keyMedium);
	keyBackOverImg = new gui_image(keyMediumOver);
	keyBackText = new gui_text("Back", 20, (GXColor){0, 0, 0, 0xff});
	keyBack = new gui_button(keyMedium->get_width(), keyMedium->get_height());
	keyBack->set_image(keyBackImg);
	keyBack->set_image_hover(keyBackOverImg);
	keyBack->set_label(keyBackText);
	keyBack->set_sound_hover(keysound_hover);
	keyBack->set_sound_click(keysound_click);
	keyBack->set_trigger(trig_a);
	keyBack->set_trigger(trig2);
	keyBack->set_position(10 * 42 + 40, 0 * 42 + y_offset);
	keyBack->set_effect_grow();
	this->append(keyBack);

	char txt[2] = { 0, 0 };
	constexpr float scale = 1.25f;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j].ch != '\0')
			{
				txt[0] = keys[i][j].ch;
				keyImg[i][j] = new gui_image(key);
				keyImgOver[i][j] = new gui_image(keyOver);
				keyTxt[i][j] = new gui_text(txt, 20, (GXColor){0, 0, 0, 0xff});
				keyTxt[i][j]->set_alignment(ALIGN_CENTER, ALIGN_BOTTOM);
				keyTxt[i][j]->set_position(0, -10);

				keyBtn[i][j] = new gui_button(key->get_width(), key->get_height());
				keyBtn[i][j]->set_image(keyImg[i][j]);
				keyBtn[i][j]->set_image_hover(keyImgOver[i][j]);
				keyBtn[i][j]->set_sound_hover(keysound_hover);
				keyBtn[i][j]->set_sound_click(keysound_click);
				keyBtn[i][j]->set_trigger(trig_a);
				keyBtn[i][j]->set_trigger(trig2);
				keyBtn[i][j]->set_label(keyTxt[i][j]);
				keyBtn[i][j]->set_position(j * 42 * scale + 195, i * 42 * scale + y_offset);
				keyBtn[i][j]->set_effect_grow();
				keyBtn[i][j]->set_scale(scale);
				this->append(keyBtn[i][j]);
			}
		}
	}
}

/**
 * Destructor for the gui_num_keyboard class.
 */
gui_num_keyboard::~gui_num_keyboard()
{
	delete kbText;
	delete keyTextbox;
	delete keyTextboxImg;
	delete keyBackText;
	delete keyBackImg;
	delete keyBackOverImg;
	delete keyBack;
	delete key;
	delete keyOver;
	delete keyMedium;
	delete keyMediumOver;
	delete keyLarge;
	delete keyLargeOver;
	delete keysound_hover;
	delete keysound_click;
	delete trig_a;
	delete trig2;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j].ch != '\0')
			{
				delete keyImg[i][j];
				delete keyImgOver[i][j];
				delete keyTxt[i][j];
				delete keyBtn[i][j];
			}
		}
	}
}

void gui_num_keyboard::Update(gui_trigger * t)
{
	if(_elements.size() == 0 || (state == STATE_DISABLED && parentElement))
		return;

	for (u8 i = 0; i < _elements.size(); i++)
	{
		try	{ _elements.at(i)->Update(t); }
		catch (const std::exception& e) { }
	}

	bool update = false;

	if(keyBack->get_state() == STATE_CLICKED)
	{
		if(strlen(kbtextstr) > 0)
		{
			kbtextstr[strlen(kbtextstr)-1] = 0;
			kbText->set_text(GetDisplayText(kbtextstr));
		}
		keyBack->set_state(STATE_SELECTED, t->chan);
	}

	char txt[2] = { 0, 0 };

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j].ch != '\0')
			{
				if(update)
				{
					txt[0] = keys[i][j].ch;
					keyTxt[i][j]->set_text(txt);
				}

				if(keyBtn[i][j]->get_state() == STATE_CLICKED)
				{
					int len = strlen(kbtextstr);

					if(len < kbtextmaxlen-1)
					{
						kbtextstr[len] = keys[i][j].ch;
						kbtextstr[len+1] = '\0';
					}
					kbText->set_text(GetDisplayText(kbtextstr));
					keyBtn[i][j]->set_state(STATE_SELECTED, t->chan);
				}
			}
		}
	}

	this->ToggleFocus(t);

	if(focus) // only send actions to this window if it's in focus
	{
		// pad/joystick navigation
		if(t->Right())
			this->MoveSelectionHor(1);
		else if(t->Left())
			this->MoveSelectionHor(-1);
		else if(t->Down())
			this->MoveSelectionVert(1);
		else if(t->Up())
			this->MoveSelectionVert(-1);
	}
}
