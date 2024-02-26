/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_window.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

gui_window::gui_window()
{
	width = 0;
	height = 0;
	focus = 0; // allow focus
}

gui_window::gui_window(int w, int h)
{
	width = w;
	height = h;
	focus = 0; // allow focus
}

gui_window::~gui_window()
{
}

void gui_window::append(gui_element* e)
{
	if (e == NULL)
		return;

	remove(e);
	_elements.push_back(e);
	e->set_parent(this);
}

void gui_window::Insert(gui_element* e, u32 index)
{
	if (e == NULL || index > (_elements.size() - 1))
		return;

	remove(e);
	_elements.insert(_elements.begin()+index, e);
	e->set_parent(this);
}

void gui_window::remove(gui_element* e)
{
	if (e == NULL)
		return;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		if(e == _elements.at(i))
		{
			_elements.erase(_elements.begin()+i);
			break;
		}
	}
}

void gui_window::removeAll()
{
	_elements.clear();
}

bool gui_window::Find(gui_element* e)
{
	if (e == NULL)
		return false;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
		if(e == _elements.at(i))
			return true;
	return false;
}

gui_element* gui_window::Getgui_elementAt(u32 index) const
{
	if (index >= _elements.size())
		return NULL;
	return _elements.at(index);
}

u32 gui_window::get_size()
{
	return _elements.size();
}

void gui_window::Draw()
{
	if(_elements.size() == 0 || !this->is_visible())
		return;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try	{ _elements.at(i)->Draw(); }
		catch (const std::exception& e) { }
	}

	this->update_effects();

	if(parentElement && state == STATE_DISABLED)
		menu_draw_rect(0,0,screen_width,screen_height,(GXColor){0xbe, 0xca, 0xd5, 0x70},1);
}

void gui_window::draw_tool_tip()
{
	if(_elements.size() == 0 || !this->is_visible())
		return;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; i++)
	{
		try	{ _elements.at(i)->draw_tool_tip(); }
		catch (const std::exception& e) { }
	}
}

void gui_window::reset_state()
{
	if(state != STATE_DISABLED)
		state = STATE_DEFAULT;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try { _elements.at(i)->reset_state(); }
		catch (const std::exception& e) { }
	}
}

void gui_window::set_state(int s)
{
	state = s;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try { _elements.at(i)->set_state(s); }
		catch (const std::exception& e) { }
	}
}

void gui_window::set_visible(bool v)
{
	visible = v;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try { _elements.at(i)->set_visible(v); }
		catch (const std::exception& e) { }
	}
}

void gui_window::SetFocus(int f)
{
	focus = f;

	if(f == 1)
		this->MoveSelectionVert(1);
	else
		this->reset_state();
}

void gui_window::change_focus(gui_element* e)
{
	if(parentElement)
		return; // this is only intended for the main window

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		if(e == _elements.at(i))
			_elements.at(i)->SetFocus(1);
		else if(_elements.at(i)->IsFocused() == 1)
			_elements.at(i)->SetFocus(0);
	}
}

void gui_window::ToggleFocus(gui_trigger * t)
{
	if(parentElement)
		return; // this is only intended for the main window

	int found = -1;
	int newfocus = -1;
	int i;

	int elemSize = _elements.size();

	// look for currently in focus element
	for (i = 0; i < elemSize; ++i)
	{
		try
		{
			if(_elements.at(i)->IsFocused() == 1)
			{
				found = i;
				break;
			}
		}
		catch (const std::exception& e) { }
	}

	// element with focus not found, try to give focus
	if(found == -1)
	{
		for (i = 0; i < elemSize; ++i)
		{
			try
			{
				if(_elements.at(i)->IsFocused() == 0 && _elements.at(i)->get_state() != STATE_DISABLED) // focus is possible (but not set)
				{
					_elements.at(i)->SetFocus(1); // give this element focus
					break;
				}
			}
			catch (const std::exception& e) { }
		}
	}
	// change focus
	else if(t->wpad->btns_d & (WPAD_BUTTON_1 | WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B)
		|| t->pad.btns_d & PAD_BUTTON_B)
	{
		for (i = found; i < elemSize; ++i)
		{
			try
			{
				if(_elements.at(i)->IsFocused() == 0 && _elements.at(i)->get_state() != STATE_DISABLED) // focus is possible (but not set)
				{
					newfocus = i;
					_elements.at(i)->SetFocus(1); // give this element focus
					_elements.at(found)->SetFocus(0); // disable focus on other element
					break;
				}
			}
			catch (const std::exception& e) { }
		}

		if(newfocus == -1)
		{
			for (i = 0; i < found; ++i)
			{
				try
				{
					if(_elements.at(i)->IsFocused() == 0 && _elements.at(i)->get_state() != STATE_DISABLED) // focus is possible (but not set)
					{
						_elements.at(i)->SetFocus(1); // give this element focus
						_elements.at(found)->SetFocus(0); // disable focus on other element
						break;
					}
				}
				catch (const std::exception& e) { }
			}
		}
	}
}

int gui_window::GetSelected()
{
	// find selected element
	int found = -1;
	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try
		{
			if(_elements.at(i)->get_state() == STATE_SELECTED)
			{
				found = int(i);
				break;
			}
		}
		catch (const std::exception& e) { }
	}
	return found;
}

// set element to left/right as selected
// there's probably a more clever way to do this, but this way works
void gui_window::MoveSelectionHor(int dir)
{
	int found = -1;
	u16 left = 0;
	u16 top = 0;
	u32 i;
	u32 elemSize = _elements.size();

	int selected = this->GetSelected();

	if(selected >= 0)
	{
		left = _elements.at(selected)->get_left();
		top = _elements.at(selected)->get_top();
	}

	
	// look for a button on the same row, to the left/right
	for (i = 0; i < elemSize; ++i)
	{
		try
		{
			if(_elements.at(i)->IsSelectable())
			{
				if(_elements.at(i)->get_left()*dir > left*dir && _elements.at(i)->get_top() == top)
				{
					if(found == -1)
						found = int(i);
					else if(_elements.at(i)->get_left()*dir < _elements.at(found)->get_left()*dir)
						found = int(i); // this is a better match
				}
			}
		}
		catch (const std::exception& e) { }
	}
	if(found >= 0)
		goto matchfound;

	// match still not found, let's try the first button in the next row
	for (i = 0; i < elemSize; ++i)
	{
		try
		{
			if(_elements.at(i)->IsSelectable())
			{
				if(_elements.at(i)->get_top()*dir > top*dir)
				{
					if(found == -1)
						found = i;
					else if(_elements.at(i)->get_top()*dir < _elements.at(found)->get_top()*dir)
						found = i; // this is a better match
					else if(_elements.at(i)->get_top()*dir == _elements.at(found)->get_top()*dir
						&&
						_elements.at(i)->get_left()*dir < _elements.at(found)->get_left()*dir)
						found = i; // this is a better match
				}
			}
		}
		catch (const std::exception& e) { }
	}

	// match found
	matchfound:
	if(found >= 0)
	{
		_elements.at(found)->set_state(STATE_SELECTED);
		if(selected >= 0)
			_elements.at(selected)->reset_state();
	}
}

void gui_window::MoveSelectionVert(int dir)
{
	int found = -1;
	u16 left = 0;
	u16 top = 0;

	int selected = this->GetSelected();

	if(selected >= 0)
	{
		left = _elements.at(selected)->get_left();
		top = _elements.at(selected)->get_top();
	}

	// look for a button above/below, with the least horizontal difference
	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try
		{
			if(_elements.at(i)->IsSelectable())
			{
				if(_elements.at(i)->get_top()*dir > top*dir)
				{
					if(found == -1)
						found = i;
					else if(_elements.at(i)->get_top()*dir < _elements.at(found)->get_top()*dir)
						found = i; // this is a better match
					else if(_elements.at(i)->get_top()*dir == _elements.at(found)->get_top()*dir
							&&
							abs(_elements.at(i)->get_left() - left) <
							abs(_elements.at(found)->get_left() - left))
						found = i;
				}
			}
		}
		catch (const std::exception& e) { }
	}
	if(found >= 0)
		goto matchfound;

	// match found
	matchfound:
	if(found >= 0)
	{
		_elements.at(found)->set_state(STATE_SELECTED);
		if(selected >= 0)
			_elements.at(selected)->reset_state();
	}
}

void gui_window::reset_text()
{
	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; i++)
	{
		try { _elements.at(i)->reset_text(); }
		catch (const std::exception& e) { }
	}
}

void gui_window::Update(gui_trigger * t)
{
	if(_elements.size() == 0 || (state == STATE_DISABLED && parentElement))
		return;

	u32 elemSize = _elements.size();
	for (u32 i = 0; i < elemSize; ++i)
	{
		try	{ _elements.at(i)->Update(t); }
		catch (const std::exception& e) { }
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

	if(update_cb)
		update_cb(this);
}
