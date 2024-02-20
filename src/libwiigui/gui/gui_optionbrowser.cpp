/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_optionbrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

/**
 * Constructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::GuiOptionBrowser(int w, int h, OptionList * l)
{
	width = w;
	height = h;
	options = l;
	selectable = true;
	listOffset = this->FindMenuItem(-1, 1);
	listChanged = true; // trigger an initial list update
	selectedItem = 0;
	focus = 0; // allow focus

	trig_a = new gui_trigger;
	trig_a->set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trig2 = new gui_trigger;
	trig2->set_simple_trigger(-1, WPAD_BUTTON_2, 0);

	btn_sound_hover = new gui_sound(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	btnsound_click = new gui_sound(button_click_pcm, button_click_pcm_size, SOUND_PCM);

	bgOptions = new gui_image_data(bg_options_png);
	bgOptionsImg = new gui_image(bgOptions);
	bgOptionsImg->set_parent(this);
	bgOptionsImg->set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgOptionsEntry = new gui_image_data(bg_options_entry_png);

	scrollbar = new gui_image_data(scrollbar_png);
	scrollbarImg = new gui_image(scrollbar);
	scrollbarImg->set_parent(this);
	scrollbarImg->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarImg->set_position(0, 30);

	arrowDown = new gui_image_data(scrollbar_arrowdown_png);
	arrowDownImg = new gui_image(arrowDown);
	arrowDownOver = new gui_image_data(scrollbar_arrowdown_over_png);
	arrowDownOverImg = new gui_image(arrowDownOver);
	arrowUp = new gui_image_data(scrollbar_arrowup_png);
	arrowUpImg = new gui_image(arrowUp);
	arrowUpOver = new gui_image_data(scrollbar_arrowup_over_png);
	arrowUpOverImg = new gui_image(arrowUpOver);

	arrowUpBtn = new gui_button(arrowUpImg->get_width(), arrowUpImg->get_height());
	arrowUpBtn->set_parent(this);
	arrowUpBtn->set_image(arrowUpImg);
	arrowUpBtn->set_image_hover(arrowUpOverImg);
	arrowUpBtn->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->set_trigger(trig_a);
	arrowUpBtn->set_sound_hover(btn_sound_hover);
	arrowUpBtn->set_sound_click(btnsound_click);

	arrowDownBtn = new gui_button(arrowDownImg->get_width(), arrowDownImg->get_height());
	arrowDownBtn->set_parent(this);
	arrowDownBtn->set_image(arrowDownImg);
	arrowDownBtn->set_image_hover(arrowDownOverImg);
	arrowDownBtn->set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->set_trigger(trig_a);
	arrowDownBtn->set_sound_hover(btn_sound_hover);
	arrowDownBtn->set_sound_click(btnsound_click);

	for(int i=0; i<PAGESIZE; i++)
	{
		optionTxt[i] = new gui_text(NULL, 20, (GXColor){0, 0, 0, 0xff});
		optionTxt[i]->set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionTxt[i]->set_position(8,0);

		optionVal[i] = new gui_text(NULL, 20, (GXColor){0, 0, 0, 0xff});
		optionVal[i]->set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionVal[i]->set_position(250,0);

		optionBg[i] = new gui_image(bgOptionsEntry);

		optionBtn[i] = new gui_button(512,30);
		optionBtn[i]->set_parent(this);
		optionBtn[i]->set_label(optionTxt[i], 0);
		optionBtn[i]->set_label(optionVal[i], 1);
		optionBtn[i]->set_image_hover(optionBg[i]);
		optionBtn[i]->set_position(2,30*i+3);
		optionBtn[i]->set_trigger(trig_a);
		optionBtn[i]->set_trigger(trig2);
		optionBtn[i]->set_sound_click(btnsound_click);
	}
}

/**
 * Destructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::~GuiOptionBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;

	delete bgOptionsImg;
	delete scrollbarImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;

	delete bgOptions;
	delete bgOptionsEntry;
	delete scrollbar;
	delete arrowDown;
	delete arrowDownOver;
	delete arrowUp;
	delete arrowUpOver;

	delete trig_a;
	delete trig2;
	delete btn_sound_hover;
	delete btnsound_click;

	for(int i=0; i<PAGESIZE; i++)
	{
		delete optionTxt[i];
		delete optionVal[i];
		delete optionBg[i];
		delete optionBtn[i];
	}
}

void GuiOptionBrowser::SetCol1Position(int x)
{
	for(int i=0; i<PAGESIZE; i++)
		optionTxt[i]->set_position(x,0);
}

void GuiOptionBrowser::SetCol2Position(int x)
{
	for(int i=0; i<PAGESIZE; i++)
		optionVal[i]->set_position(x,0);
}

void GuiOptionBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<PAGESIZE; i++)
		optionBtn[i]->reset_state();

	if(f == 1)
		optionBtn[selectedItem]->set_state(STATE_SELECTED);
}

void GuiOptionBrowser::reset_state()
{
	if(state != STATE_DISABLED)
	{
		state = STATE_DEFAULT;
		stateChan = -1;
	}

	for(int i=0; i<PAGESIZE; i++)
	{
		optionBtn[i]->reset_state();
	}
}

int GuiOptionBrowser::GetClickedOption()
{
	int found = -1;
	for(int i=0; i<PAGESIZE; i++)
	{
		if(optionBtn[i]->get_state() == STATE_CLICKED)
		{
			optionBtn[i]->set_state(STATE_SELECTED);
			found = optionIndex[i];
			break;
		}
	}
	return found;
}

/****************************************************************************
 * FindMenuItem
 *
 * Help function to find the next visible menu item on the list
 ***************************************************************************/

int GuiOptionBrowser::FindMenuItem(int currentItem, int direction)
{
	int nextItem = currentItem + direction;

	if(nextItem < 0 || nextItem >= options->length)
		return -1;

	if(strlen(options->name[nextItem]) > 0)
		return nextItem;
	else
		return FindMenuItem(nextItem, direction);
}

/**
 * Draw the button on screen
 */
void GuiOptionBrowser::Draw()
{
	if(!this->is_visible())
		return;

	bgOptionsImg->Draw();

	int next = listOffset;

	for(int i=0; i<PAGESIZE; ++i)
	{
		if(next >= 0)
		{
			optionBtn[i]->Draw();
			next = this->FindMenuItem(next, 1);
		}
		else
			break;
	}

	scrollbarImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();

	this->update_effects();
}

void GuiOptionBrowser::TriggerUpdate()
{
	listChanged = true;
}

void GuiOptionBrowser::reset_text()
{
	int next = listOffset;

	for(int i=0; i<PAGESIZE; i++)
	{
		if(next >= 0)
		{
			optionBtn[i]->reset_text();
			next = this->FindMenuItem(next, 1);
		}
		else
			break;
	}
}

void GuiOptionBrowser::Update(gui_trigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	int next, prev;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);

	next = listOffset;

	if(listChanged)
	{
		listChanged = false;
		for(int i=0; i<PAGESIZE; ++i)
		{
			if(next >= 0)
			{
				if(optionBtn[i]->get_state() == STATE_DISABLED)
				{
					optionBtn[i]->SetVisible(true);
					optionBtn[i]->set_state(STATE_DEFAULT);
				}

				optionTxt[i]->set_text(options->name[next]);
				optionVal[i]->set_text(options->value[next]);
				optionIndex[i] = next;
				next = this->FindMenuItem(next, 1);
			}
			else
			{
				optionBtn[i]->SetVisible(false);
				optionBtn[i]->set_state(STATE_DISABLED);
			}
		}
	}

	for(int i=0; i<PAGESIZE; ++i)
	{
		if(i != selectedItem && optionBtn[i]->get_state() == STATE_SELECTED)
			optionBtn[i]->reset_state();
		else if(focus && i == selectedItem && optionBtn[i]->get_state() == STATE_DEFAULT)
			optionBtn[selectedItem]->set_state(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !optionBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		optionBtn[i]->Update(t);
		t->chan = currChan;

		if(optionBtn[i]->get_state() == STATE_SELECTED)
			selectedItem = i;
	}

	// pad/joystick navigation
	if(!focus)
		return; // skip navigation

	if(t->Down() || arrowDownBtn->get_state() == STATE_CLICKED)
	{
		next = this->FindMenuItem(optionIndex[selectedItem], 1);

		if(next >= 0)
		{
			if(selectedItem == PAGESIZE-1)
			{
				// move list down by 1
				listOffset = this->FindMenuItem(listOffset, 1);
				listChanged = true;
			}
			else if(optionBtn[selectedItem+1]->is_visible())
			{
				optionBtn[selectedItem]->reset_state();
				optionBtn[selectedItem+1]->set_state(STATE_SELECTED, t->chan);
				++selectedItem;
			}
		}
		arrowDownBtn->reset_state();
	}
	else if(t->Up() || arrowUpBtn->get_state() == STATE_CLICKED)
	{
		prev = this->FindMenuItem(optionIndex[selectedItem], -1);

		if(prev >= 0)
		{
			if(selectedItem == 0)
			{
				// move list up by 1
				listOffset = prev;
				listChanged = true;
			}
			else
			{
				optionBtn[selectedItem]->reset_state();
				optionBtn[selectedItem-1]->set_state(STATE_SELECTED, t->chan);
				--selectedItem;
			}
		}
		arrowUpBtn->reset_state();
	}

	if(update_cb)
		update_cb(this);
}
