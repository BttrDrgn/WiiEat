/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_filebrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"
#include "../filebrowser.h"

/**
 * Constructor for the GuiFileBrowser class.
 */
GuiFileBrowser::GuiFileBrowser(int w, int h)
{
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = 0;
	selectable = true;
	listChanged = true; // trigger an initial list update
	focus = 0; // allow focus

	trig_a = new gui_trigger;
	trig_a->set_simple_trigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trig2 = new gui_trigger;
	trig2->set_simple_trigger(-1, WPAD_BUTTON_2, 0);

	trigHeldA = new gui_trigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btn_sound_hover = new gui_sound(button_over_pcm, button_over_pcm_size, SOUND_PCM);
	btnsound_click = new gui_sound(button_click_pcm, button_click_pcm_size, SOUND_PCM);

	bgFileSelection = new gui_image_data(bg_file_selection_png);
	bgFileSelectionImg = new gui_image(bgFileSelection);
	bgFileSelectionImg->set_parent(this);
	bgFileSelectionImg->set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgFileSelectionEntry = new gui_image_data(bg_file_selection_entry_png);
	fileFolder = new gui_image_data(folder_png);

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
	scrollbarBox = new gui_image_data(scrollbar_box_png);
	scrollbarBoxImg = new gui_image(scrollbarBox);
	scrollbarBoxOver = new gui_image_data(scrollbar_box_over_png);
	scrollbarBoxOverImg = new gui_image(scrollbarBoxOver);

	arrowUpBtn = new gui_button(arrowUpImg->get_width(), arrowUpImg->get_height());
	arrowUpBtn->set_parent(this);
	arrowUpBtn->set_image(arrowUpImg);
	arrowUpBtn->set_image_hover(arrowUpOverImg);
	arrowUpBtn->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->set_position(0, -2);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->SetClickable(false);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->set_trigger(trigHeldA);
	arrowUpBtn->set_sound_hover(btn_sound_hover);
	arrowUpBtn->set_sound_click(btnsound_click);

	arrowDownBtn = new gui_button(arrowDownImg->get_width(), arrowDownImg->get_height());
	arrowDownBtn->set_parent(this);
	arrowDownBtn->set_image(arrowDownImg);
	arrowDownBtn->set_image_hover(arrowDownOverImg);
	arrowDownBtn->set_alignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->SetClickable(false);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->set_trigger(trigHeldA);
	arrowDownBtn->set_sound_hover(btn_sound_hover);
	arrowDownBtn->set_sound_click(btnsound_click);

	scrollbarBoxBtn = new gui_button(scrollbarBoxImg->get_width(), scrollbarBoxImg->get_height());
	scrollbarBoxBtn->set_parent(this);
	scrollbarBoxBtn->set_image(scrollbarBoxImg);
	scrollbarBoxBtn->set_image_hover(scrollbarBoxOverImg);
	scrollbarBoxBtn->set_alignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarBoxBtn->SetMinY(0);
	scrollbarBoxBtn->SetMaxY(130);
	scrollbarBoxBtn->SetSelectable(false);
	scrollbarBoxBtn->SetClickable(false);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->set_trigger(trigHeldA);

	for(int i=0; i<FILE_PAGESIZE; ++i)
	{
		fileListText[i] = new gui_text(NULL, 20, (GXColor){0, 0, 0, 0xff});
		fileListText[i]->set_alignment(ALIGN_LEFT, ALIGN_MIDDLE);
		fileListText[i]->set_position(5,0);
		fileListText[i]->set_max_width(512);

		fileListBg[i] = new gui_image(bgFileSelectionEntry);
		fileListFolder[i] = new gui_image(fileFolder);

		fileList[i] = new gui_button(512, 30);
		fileList[i]->set_parent(this);
		fileList[i]->set_label(fileListText[i]);
		fileList[i]->set_image_hover(fileListBg[i]);
		fileList[i]->set_position(2,30*i+3);
		fileList[i]->set_trigger(trig_a);
		fileList[i]->set_trigger(trig2);
		fileList[i]->set_sound_click(btnsound_click);
	}
}

/**
 * Destructor for the GuiFileBrowser class.
 */
GuiFileBrowser::~GuiFileBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete bgFileSelectionImg;
	delete scrollbarImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	delete bgFileSelection;
	delete bgFileSelectionEntry;
	delete fileFolder;
	delete scrollbar;
	delete arrowDown;
	delete arrowDownOver;
	delete arrowUp;
	delete arrowUpOver;
	delete scrollbarBox;
	delete scrollbarBoxOver;

	delete btn_sound_hover;
	delete btnsound_click;
	delete trigHeldA;
	delete trig_a;
	delete trig2;

	for(int i=0; i<FILE_PAGESIZE; i++)
	{
		delete fileListText[i];
		delete fileList[i];
		delete fileListBg[i];
		delete fileListFolder[i];
	}
}

void GuiFileBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<FILE_PAGESIZE; i++)
		fileList[i]->reset_state();

	if(f == 1)
		fileList[selectedItem]->set_state(STATE_SELECTED);
}

void GuiFileBrowser::reset_state()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(int i=0; i<FILE_PAGESIZE; i++)
	{
		fileList[i]->reset_state();
	}
}

void GuiFileBrowser::TriggerUpdate()
{
	listChanged = true;
}

/**
 * Draw the button on screen
 */
void GuiFileBrowser::Draw()
{
	if(!this->is_visible())
		return;

	bgFileSelectionImg->Draw();

	for(u32 i=0; i<FILE_PAGESIZE; ++i)
	{
		fileList[i]->Draw();
	}

	scrollbarImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	this->update_effects();
}

void GuiFileBrowser::draw_tool_tip()
{
}

void GuiFileBrowser::Update(gui_trigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	int position = 0;
	int positionWiimote = 0;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);

	// move the file listing to respond to wiimote cursor movement
	if(scrollbarBoxBtn->get_state() == STATE_HELD &&
		scrollbarBoxBtn->get_stateChan() == t->chan &&
		t->wpad->ir.valid &&
		browser.numEntries > FILE_PAGESIZE
		)
	{
		scrollbarBoxBtn->set_position(0,0);
		positionWiimote = t->wpad->ir.y - 60 - scrollbarBoxBtn->get_top();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		browser.pageIndex = (positionWiimote * browser.numEntries)/130.0 - selectedItem;

		if(browser.pageIndex <= 0)
		{
			browser.pageIndex = 0;
		}
		else if(browser.pageIndex+FILE_PAGESIZE >= browser.numEntries)
		{
			browser.pageIndex = browser.numEntries-FILE_PAGESIZE;
		}
		listChanged = true;
		focus = false;
	}

	if(arrowDownBtn->get_state() == STATE_HELD && arrowDownBtn->get_stateChan() == t->chan)
	{
		t->wpad->btns_d |= WPAD_BUTTON_DOWN;
		if(!this->IsFocused())
			((gui_window *)this->GetParent())->change_focus(this);
	}
	else if(arrowUpBtn->get_state() == STATE_HELD && arrowUpBtn->get_stateChan() == t->chan)
	{
		t->wpad->btns_d |= WPAD_BUTTON_UP;
		if(!this->IsFocused())
			((gui_window *)this->GetParent())->change_focus(this);
	}

	// pad/joystick navigation
	if(!focus)
	{
		goto endNavigation; // skip navigation
		listChanged = false;
	}

	if(t->Right())
	{
		if(browser.pageIndex < browser.numEntries && browser.numEntries > FILE_PAGESIZE)
		{
			browser.pageIndex += FILE_PAGESIZE;
			if(browser.pageIndex+FILE_PAGESIZE >= browser.numEntries)
				browser.pageIndex = browser.numEntries-FILE_PAGESIZE;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(browser.pageIndex > 0)
		{
			browser.pageIndex -= FILE_PAGESIZE;
			if(browser.pageIndex < 0)
				browser.pageIndex = 0;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(browser.pageIndex + selectedItem + 1 < browser.numEntries)
		{
			if(selectedItem == FILE_PAGESIZE-1)
			{
				// move list down by 1
				++browser.pageIndex;
				listChanged = true;
			}
			else if(fileList[selectedItem+1]->is_visible())
			{
				fileList[selectedItem]->reset_state();
				fileList[++selectedItem]->set_state(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	browser.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			--browser.pageIndex;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			fileList[selectedItem]->reset_state();
			fileList[--selectedItem]->set_state(STATE_SELECTED, t->chan);
		}
	}

	endNavigation:

	for(int i=0; i<FILE_PAGESIZE; ++i)
	{
		if(listChanged || numEntries != browser.numEntries)
		{
			if(browser.pageIndex+i < browser.numEntries)
			{
				if(fileList[i]->get_state() == STATE_DISABLED)
					fileList[i]->set_state(STATE_DEFAULT);

				fileList[i]->SetVisible(true);

				fileListText[i]->set_text(browserList[browser.pageIndex+i].displayname);

				if(browserList[browser.pageIndex+i].isdir) // directory
				{
					fileList[i]->set_icon(fileListFolder[i]);
					fileListText[i]->set_position(30,0);
				}
				else
				{
					fileList[i]->set_icon(NULL);
					fileListText[i]->set_position(10,0);
				}
			}
			else
			{
				fileList[i]->SetVisible(false);
				fileList[i]->set_state(STATE_DISABLED);
			}
		}

		if(i != selectedItem && fileList[i]->get_state() == STATE_SELECTED)
			fileList[i]->reset_state();
		else if(focus && i == selectedItem && fileList[i]->get_state() == STATE_DEFAULT)
			fileList[selectedItem]->set_state(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !fileList[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		fileList[i]->Update(t);
		t->chan = currChan;

		if(fileList[i]->get_state() == STATE_SELECTED)
		{
			selectedItem = i;
			browser.selIndex = browser.pageIndex + i;
		}

		if(selectedItem == i)
			fileListText[i]->set_scroll(SCROLL_HORIZONTAL);
		else
			fileListText[i]->set_scroll(SCROLL_NONE);
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
		scrollbarBoxBtn->set_position(0,position+36);
	}
	else if(listChanged || numEntries != browser.numEntries)
	{
		if(float((browser.pageIndex<<1))/(float(FILE_PAGESIZE)) < 1.0)
		{
			position = 0;
		}
		else if(browser.pageIndex+FILE_PAGESIZE >= browser.numEntries)
		{
			position = 130;
		}
		else
		{
			position = 130 * (browser.pageIndex + FILE_PAGESIZE/2) / (float)browser.numEntries;
		}
		scrollbarBoxBtn->set_position(0,position+36);
	}

	listChanged = false;
	numEntries = browser.numEntries;

	if(update_cb)
		update_cb(this);
}
