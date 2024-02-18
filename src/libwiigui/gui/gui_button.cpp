/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_button.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"
/**
 * Constructor for the gui_button class.
 */

gui_button::gui_button(int w, int h)
{
	width = w;
	height = h;
	image = NULL;
	image_hover = NULL;
	image_hold = NULL;
	image_click = NULL;
	icon = NULL;
	icon_hover = NULL;
	icon_hold = NULL;
	icon_click = NULL;

	for(int i=0; i < 3; i++)
	{
		label[i] = NULL;
		label_hover[i] = NULL;
		label_hold[i] = NULL;
		label_click[i] = NULL;
	}

	sound_hover = NULL;
	sound_hold = NULL;
	sound_click = NULL;
	tooltip = NULL;
	selectable = true;
	holdable = false;
	clickable = true;
}

/**
 * Destructor for the gui_button class.
 */
gui_button::~gui_button()
{
}

void gui_button::set_image(gui_image* img)
{
	image = img;
	if(img) img->set_parent(this);
}
void gui_button::set_image_hover(gui_image* img)
{
	image_hover = img;
	if(img) img->set_parent(this);
}
void gui_button::set_image_hold(gui_image* img)
{
	image_hold = img;
	if(img) img->set_parent(this);
}
void gui_button::set_image_click(gui_image* img)
{
	image_click = img;
	if(img) img->set_parent(this);
}
void gui_button::set_icon(gui_image* img)
{
	icon = img;
	if(img) img->set_parent(this);
}
void gui_button::set_icon_hover(gui_image* img)
{
	icon_hover = img;
	if(img) img->set_parent(this);
}
void gui_button::set_icon_hold(gui_image* img)
{
	icon_hold = img;
	if(img) img->set_parent(this);
}
void gui_button::set_icon_click(gui_image* img)
{
	icon_click = img;
	if(img) img->set_parent(this);
}
void gui_button::set_label(gui_text* txt, int n)
{
	label[n] = txt;
	if(txt) txt->set_parent(this);
}
void gui_button::set_label_hover(gui_text* txt, int n)
{
	label_hover[n] = txt;
	if(txt) txt->set_parent(this);
}
void gui_button::set_label_hold(gui_text* txt, int n)
{
	label_hold[n] = txt;
	if(txt) txt->set_parent(this);
}
void gui_button::set_label_click(gui_text* txt, int n)
{
	label_click[n] = txt;
	if(txt) txt->set_parent(this);
}
void gui_button::set_sound_hover(gui_sound * snd)
{
	sound_hover = snd;
}
void gui_button::set_sound_hold(gui_sound * snd)
{
	sound_hold = snd;
}
void gui_button::set_sound_click(gui_sound * snd)
{
	sound_click = snd;
}
void gui_button::set_tooltip(gui_tooltip* t)
{
	tooltip = t;
	if(t)
		tooltip->set_parent(this);
}

/**
 * Draw the button on screen
 */
void gui_button::Draw()
{
	if(!this->is_visible())
		return;

	if(state == STATE_SELECTED || state == STATE_HELD)
	{
		if(image_hover)
			image_hover->Draw();
		else if(image) // draw image
			image->Draw();

		if(icon_hover)
			icon_hover->Draw();
		else if(icon) // draw icon
			icon->Draw();

		// draw text
		if(label_hover[0])
			label_hover[0]->Draw();
		else if(label[0])
			label[0]->Draw();
			
		if(label_hover[1])
			label_hover[1]->Draw();	
		else if(label[1])
			label[1]->Draw();
			
		if(label_hover[2])
			label_hover[2]->Draw();
		else if(label[2])
			label[2]->Draw();
	}
	else
	{
		if(image) // draw image
			image->Draw();
		if(icon) // draw icon
			icon->Draw();

		// draw text
		if(label[0])
			label[0]->Draw();
		if(label[1])
			label[1]->Draw();
		if(label[2])
			label[2]->Draw();
	}

	this->update_effects();
}

void gui_button::draw_tool_tip()
{
	if(tooltip)
		tooltip->draw_tool_tip();
}

void gui_button::reset_text()
{
	for(int i=0; i<3; i++)
	{
		if(label[i])
			label[i]->reset_text();
		if(label_hover[i])
			label_hover[i]->reset_text();
	}
	if(tooltip)
		tooltip->reset_text();
}

void gui_button::Update(gui_trigger * t)
{
	if(state == STATE_CLICKED || state == STATE_DISABLED || !t)
		return;
	else if(parentElement && parentElement->get_state() == STATE_DISABLED)
		return;

	#ifdef HW_RVL
	// cursor
	if(t->wpad->ir.valid && t->chan >= 0)
	{
		if(this->IsInside(t->wpad->ir.x, t->wpad->ir.y))
		{
			if(state == STATE_DEFAULT) // we weren't on the button before!
			{
				this->set_state(STATE_SELECTED, t->chan);

				if(this->Rumble())
					rumbleRequest[t->chan] = 1;

				if(sound_hover)
					sound_hover->Play();

				if(effectsOver && !effects)
				{
					// initiate effects
					effects = effectsOver;
					effectAmount = effectAmountOver;
					effectTarget = effectTargetOver;
				}
			}
		}
		else
		{
			if(state == STATE_SELECTED && (stateChan == t->chan || stateChan == -1))
				this->reset_state();

			if(effectTarget == effectTargetOver && effectAmount == effectAmountOver)
			{
				// initiate effects (in reverse)
				effects = effectsOver;
				effectAmount = -effectAmountOver;
				effectTarget = 100;
			}
		}
	}
	#endif

	// button triggers
	if(this->IsClickable())
	{
		s32 wm_btns, wm_btns_trig, cc_btns, cc_btns_trig;
		for(int i=0; i<3; i++)
		{
			if(trigger[i] && (trigger[i]->chan == -1 || trigger[i]->chan == t->chan))
			{
				// higher 16 bits only (wiimote)
				wm_btns = t->wpad->btns_d << 16;
				wm_btns_trig = trigger[i]->wpad->btns_d << 16;

				// lower 16 bits only (classic controller)
				cc_btns = t->wpad->btns_d >> 16;
				cc_btns_trig = trigger[i]->wpad->btns_d >> 16;

				if(
					(t->wpad->btns_d > 0 &&
					(wm_btns == wm_btns_trig ||
					(cc_btns == cc_btns_trig && t->wpad->exp.type == EXP_CLASSIC))) ||
					(t->pad.btns_d == trigger[i]->pad.btns_d && t->pad.btns_d > 0))
				{
					if(t->chan == stateChan || stateChan == -1)
					{
						if(state == STATE_SELECTED)
						{
							if(!t->wpad->ir.valid ||	this->IsInside(t->wpad->ir.x, t->wpad->ir.y))
							{
								this->set_state(STATE_CLICKED, t->chan);

								if(sound_click)
									sound_click->Play();
							}
						}
						else if(trigger[i]->type == TRIGGER_BUTTON_ONLY)
						{
							this->set_state(STATE_CLICKED, t->chan);
						}
						else if(trigger[i]->type == TRIGGER_BUTTON_ONLY_IN_FOCUS &&
								parentElement->IsFocused())
						{
							this->set_state(STATE_CLICKED, t->chan);
						}
					}
				}
			}
		}
	}

	if(this->IsHoldable())
	{
		bool held = false;
		s32 wm_btns, wm_btns_h, wm_btns_trig, cc_btns, cc_btns_h, cc_btns_trig;

		for(int i=0; i<3; i++)
		{
			if(trigger[i] && (trigger[i]->chan == -1 || trigger[i]->chan == t->chan))
			{
				// higher 16 bits only (wiimote)
				wm_btns = t->wpad->btns_d << 16;
				wm_btns_h = t->wpad->btns_h << 16;
				wm_btns_trig = trigger[i]->wpad->btns_h << 16;

				// lower 16 bits only (classic controller)
				cc_btns = t->wpad->btns_d >> 16;
				cc_btns_h = t->wpad->btns_h >> 16;
				cc_btns_trig = trigger[i]->wpad->btns_h >> 16;

				if(
					(t->wpad->btns_d > 0 &&
					(wm_btns == wm_btns_trig ||
					(cc_btns == cc_btns_trig && t->wpad->exp.type == EXP_CLASSIC))) ||
					(t->pad.btns_d == trigger[i]->pad.btns_h && t->pad.btns_d > 0))
				{
					if(trigger[i]->type == TRIGGER_HELD && state == STATE_SELECTED &&
						(t->chan == stateChan || stateChan == -1))
						this->set_state(STATE_CLICKED, t->chan);
				}

				if(
					(t->wpad->btns_h > 0 &&
					(wm_btns_h == wm_btns_trig ||
					(cc_btns_h == cc_btns_trig && t->wpad->exp.type == EXP_CLASSIC))) ||
					(t->pad.btns_h == trigger[i]->pad.btns_h && t->pad.btns_h > 0))
				{
					if(trigger[i]->type == TRIGGER_HELD)
						held = true;
				}

				if(!held && state == STATE_HELD && stateChan == t->chan)
				{
					this->reset_state();
				}
				else if(held && state == STATE_CLICKED && stateChan == t->chan)
				{
					this->set_state(STATE_HELD, t->chan);
				}
			}
		}
	}

	if(update_cb)
		update_cb(this);
}
