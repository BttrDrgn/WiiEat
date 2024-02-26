/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_element.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

/**
 * Constructor for the Object class.
 */
gui_element::gui_element()
{
	xoffset = 0;
	yoffset = 0;
	xmin = 0;
	xmax = 0;
	ymin = 0;
	ymax = 0;
	width = 0;
	height = 0;
	alpha = 255;
	xscale = 1;
	yscale = 1;
	state = STATE_DEFAULT;
	stateChan = -1;
	trigger[0] = NULL;
	trigger[1] = NULL;
	trigger[2] = NULL;
	parentElement = NULL;
	rumble = true;
	selectable = false;
	clickable = false;
	holdable = false;
	visible = true;
	focus = -1; // cannot be focused
	update_cb = NULL;
	yoffsetDyn = 0;
	xoffsetDyn = 0;
	alphaDyn = -1;
	scaleDyn = 1;
	effects = 0;
	effectAmount = 0;
	effectTarget = 0;
	effectsOver = 0;
	effectAmountOver = 0;
	effectTargetOver = 0;

	// default alignment - align to top left
	alignmentVert = ALIGN_TOP;
	alignmentHor = ALIGN_LEFT;
}

/**
 * Destructor for the gui_element class.
 */
gui_element::~gui_element()
{
}

void gui_element::set_parent(gui_element * e)
{
	parentElement = e;
}

gui_element * gui_element::GetParent()
{
	return parentElement;
}

int gui_element::get_left()
{
	int x = 0;
	int pWidth = 0;
	int pLeft = 0;

	if(parentElement)
	{
		pWidth = parentElement->get_width();
		pLeft = parentElement->get_left();
	}

	if(effects & (EFFECT_SLIDE_IN | EFFECT_SLIDE_OUT))
		pLeft += xoffsetDyn;

	switch(alignmentHor)
	{
		case ALIGN_LEFT:
			x = pLeft;
			break;
		case ALIGN_CENTER:
			x = pLeft + pWidth/2.0 - (width*xscale)/2.0;
			break;
		case ALIGN_RIGHT:
			x = pLeft + pWidth - width*xscale;
			break;
	}
	x += (width*(xscale - 1))/2.0; // correct offset for scaled images
	return x + xoffset;
}

int gui_element::get_top()
{
	int y = 0;
	int pHeight = 0;
	int pTop = 0;

	if(parentElement)
	{
		pHeight = parentElement->get_height();
		pTop = parentElement->get_top();
	}

	if(effects & (EFFECT_SLIDE_IN | EFFECT_SLIDE_OUT))
		pTop += yoffsetDyn;

	switch(alignmentVert)
	{
		case ALIGN_TOP:
			y = pTop;
			break;
		case ALIGN_MIDDLE:
			y = pTop + pHeight/2.0 - (height*yscale)/2.0;
			break;
		case ALIGN_BOTTOM:
			y = pTop + pHeight - height*yscale;
			break;
	}
	y += (height*(yscale - 1))/2.0; // correct offset for scaled images
	return y + yoffset;
}

void gui_element::SetMinX(int x)
{
	xmin = x;
}

int gui_element::GetMinX()
{
	return xmin;
}

void gui_element::SetMaxX(int x)
{
	xmax = x;
}

int gui_element::GetMaxX()
{
	return xmax;
}

void gui_element::SetMinY(int y)
{
	ymin = y;
}

int gui_element::GetMinY()
{
	return ymin;
}

void gui_element::SetMaxY(int y)
{
	ymax = y;
}

int gui_element::GetMaxY()
{
	return ymax;
}

int gui_element::get_width()
{
	return width;
}

int gui_element::get_height()
{
	return height;
}

void gui_element::SetSize(int w, int h)
{

	width = w;
	height = h;
}

bool gui_element::is_visible()
{
	return visible;
}

void gui_element::set_visible(bool v)
{
	visible = v;
}

void gui_element::set_alpha(int a)
{
	alpha = a;
}

int gui_element::get_alpha()
{
	int a = alpha;

	if(alphaDyn >= 0)
		a = alphaDyn;

	if(parentElement)
		a *= float(parentElement->get_alpha())/255.0f;

	return a;
}

void gui_element::set_scale(float s)
{
	xscale = s;
	yscale = s;
}

void gui_element::set_scale_x(float s)
{
	xscale = s;
}

void gui_element::set_scale_y(float s)
{
	yscale = s;
}

void gui_element::set_scale(int mw, int mh)
{
	xscale = 1.0f;
	if(width > mw || height > mh)
	{
		if(width/(height*1.0) > mw/(mh*1.0))
			xscale = mw/(width*1.0);
		else
			xscale = mh/(height*1.0);
	}
	yscale = xscale;
}

float gui_element::get_scale()
{
	float s = xscale * scaleDyn;

	if(parentElement)
		s *= parentElement->get_scale();

	return s;
}

float gui_element::get_scaleX()
{
	float s = xscale * scaleDyn;

	if(parentElement)
		s *= parentElement->get_scale();

	return s;
}

float gui_element::get_scaleY()
{
	float s = yscale * scaleDyn;

	if(parentElement)
		s *= parentElement->get_scaleY();

	return s;
}

int gui_element::get_state()
{
	return state;
}

int gui_element::get_stateChan()
{
	return stateChan;
}

void gui_element::set_state(int s, int c)
{
	state = s;
	stateChan = c;
}

void gui_element::reset_state()
{
	if(state != STATE_DISABLED)
	{
		state = STATE_DEFAULT;
		stateChan = -1;
	}
}

void gui_element::SetClickable(bool c)
{
	clickable = c;
}

void gui_element::SetSelectable(bool s)
{
	selectable = s;
}

void gui_element::SetHoldable(bool d)
{
	holdable = d;
}

bool gui_element::IsSelectable()
{
	if(state == STATE_DISABLED || state == STATE_CLICKED)
		return false;
	else
		return selectable;
}

bool gui_element::IsClickable()
{
	if(state == STATE_DISABLED ||
		state == STATE_CLICKED ||
		state == STATE_HELD)
		return false;
	else
		return clickable;
}

bool gui_element::IsHoldable()
{
	if(state == STATE_DISABLED)
		return false;
	else
		return holdable;
}

void gui_element::SetFocus(int f)
{
	focus = f;
}

int gui_element::IsFocused()
{
	return focus;
}

void gui_element::set_trigger(gui_trigger * t)
{
	if(!trigger[0])
		trigger[0] = t;
	else if(!trigger[1])
		trigger[1] = t;
	else if(!trigger[2])
		trigger[2] = t;
	else // all were assigned, so we'll just overwrite the first one
		trigger[0] = t;
}

void gui_element::set_trigger(u8 i, gui_trigger * t)
{
	trigger[i] = t;
}

bool gui_element::Rumble()
{
	return rumble;
}

void gui_element::set_rumble(bool r)
{
	rumble = r;
}

int gui_element::get_effect()
{
	return effects;
}

void gui_element::set_effect(int eff, int amount, int target)
{
	if(eff & EFFECT_SLIDE_IN)
	{
		// these calculations overcompensate a little
		if(eff & EFFECT_SLIDE_TOP)
			yoffsetDyn = -screen_height;
		else if(eff & EFFECT_SLIDE_LEFT)
			xoffsetDyn = -screen_width;
		else if(eff & EFFECT_SLIDE_BOTTOM)
			yoffsetDyn = screen_height;
		else if(eff & EFFECT_SLIDE_RIGHT)
			xoffsetDyn = screen_width;
	}
	if(eff & EFFECT_FADE)
	{
		if(amount > 0)
			alphaDyn = 0;
		else if(amount < 0)
			alphaDyn = alpha;
	}

	effects |= eff;
	effectAmount = amount;
	effectTarget = target;
}

void gui_element::set_effect_on_hover(int eff, int amount, int target)
{
	effectsOver |= eff;
	effectAmountOver = amount;
	effectTargetOver = target;
}

void gui_element::set_effect_grow()
{
	set_effect_on_hover(EFFECT_SCALE, 4, 110);
}

void gui_element::set_effect_shrink()
{
	set_effect_on_hover(EFFECT_SCALE, -4, 110);
}

void gui_element::update_effects()
{
	if(effects & (EFFECT_SLIDE_IN | EFFECT_SLIDE_OUT))
	{
		if(effects & EFFECT_SLIDE_IN)
		{
			if(effects & EFFECT_SLIDE_LEFT)
			{
				xoffsetDyn += effectAmount;

				if(xoffsetDyn >= 0)
				{
					xoffsetDyn = 0;
					effects = 0;
				}
			}
			else if(effects & EFFECT_SLIDE_RIGHT)
			{
				xoffsetDyn -= effectAmount;

				if(xoffsetDyn <= 0)
				{
					xoffsetDyn = 0;
					effects = 0;
				}
			}
			else if(effects & EFFECT_SLIDE_TOP)
			{
				yoffsetDyn += effectAmount;

				if(yoffsetDyn >= 0)
				{
					yoffsetDyn = 0;
					effects = 0;
				}
			}
			else if(effects & EFFECT_SLIDE_BOTTOM)
			{
				yoffsetDyn -= effectAmount;

				if(yoffsetDyn <= 0)
				{
					yoffsetDyn = 0;
					effects = 0;
				}
			}
		}
		else
		{
			if(effects & EFFECT_SLIDE_LEFT)
			{
				xoffsetDyn -= effectAmount;

				if(xoffsetDyn <= -screen_width)
					effects = 0; // shut off effect
			}
			else if(effects & EFFECT_SLIDE_RIGHT)
			{
				xoffsetDyn += effectAmount;

				if(xoffsetDyn >= screen_width)
					effects = 0; // shut off effect
			}
			else if(effects & EFFECT_SLIDE_TOP)
			{
				yoffsetDyn -= effectAmount;

				if(yoffsetDyn <= -screen_height)
					effects = 0; // shut off effect
			}
			else if(effects & EFFECT_SLIDE_BOTTOM)
			{
				yoffsetDyn += effectAmount;

				if(yoffsetDyn >= screen_height)
					effects = 0; // shut off effect
			}
		}
	}
	if(effects & EFFECT_FADE)
	{
		alphaDyn += effectAmount;

		if(effectAmount < 0 && alphaDyn <= 0)
		{
			alphaDyn = 0;
			effects = 0; // shut off effect
		}
		else if(effectAmount > 0 && alphaDyn >= alpha)
		{
			alphaDyn = alpha;
			effects = 0; // shut off effect
		}
	}
	if(effects & EFFECT_SCALE)
	{
		scaleDyn += f32(effectAmount)*0.01f;
		f32 effTar100 = f32(effectTarget)*0.01f;

		if((effectAmount < 0 && scaleDyn <= effTar100)
			|| (effectAmount > 0 && scaleDyn >= effTar100))
		{
			scaleDyn = effTar100;
			effects = 0; // shut off effect
		}
	}
}

void gui_element::Update(gui_trigger * t)
{
	if(update_cb)
		update_cb(this);
}

void gui_element::set_update_callback(update_callback u)
{
	update_cb = u;
}

void gui_element::set_position(int xoff, int yoff)
{
	xoffset = xoff;
	yoffset = yoff;
}

void gui_element::set_alignment(int hor, int vert)
{
	alignmentHor = hor;
	alignmentVert = vert;
}

int gui_element::GetSelected()
{
	return -1;
}

void gui_element::reset_text()
{
}

void gui_element::Draw()
{
}

void gui_element::draw_tool_tip()
{
}

bool gui_element::IsInside(int x, int y)
{
	if(unsigned(x - this->get_left())  < unsigned(width)
	&& unsigned(y - this->get_top())  < unsigned(height))
		return true;
	return false;
}
