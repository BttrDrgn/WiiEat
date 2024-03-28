/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_text.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"
#include "../gettext.h"

static GXColor preset_color = (GXColor){255, 255, 255, 255};
static int currentSize = 0;
static int presetSize = 0;
static int preset_max_width = 0;
static int preset_alignmentHor = 0;
static int preset_alignmentVert = 0;
static u16 preset_style = 0;

#define TEXT_SCROLL_DELAY			6
#define	TEXT_SCROLL_INITIAL_DELAY	12

/**
 * Constructor for the gui_text class.
 */
gui_text::gui_text(const char * t, int s, GXColor c)
{
	origText = NULL;
	text = NULL;
	size = s;
	color = c;
	alpha = c.a;
	style = FTGX_JUSTIFY_CENTER | FTGX_ALIGN_MIDDLE;
	maxWidth = 0;
	wrap = false;
	textDynNum = 0;
	textScroll = SCROLL_NONE;
	textScrollPos = 0;
	textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
	textScrollDelay = TEXT_SCROLL_DELAY;

	alignmentHor = ALIGN_CENTER;
	alignmentVert = ALIGN_MIDDLE;

	if(t)
	{
		origText = strdup(t);
		text = charToWideChar(gettext(t));
	}

	for(int i=0; i < 20; i++)
		textDyn[i] = NULL;
}

/**
 * Constructor for the gui_text class, uses presets
 */
gui_text::gui_text(const char * t)
{
	origText = NULL;
	text = NULL;
	size = presetSize;
	color = preset_color;
	alpha = preset_color.a;
	style = preset_style;
	maxWidth = preset_max_width;
	wrap = false;
	textDynNum = 0;
	textScroll = SCROLL_NONE;
	textScrollPos = 0;
	textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
	textScrollDelay = TEXT_SCROLL_DELAY;

	alignmentHor = preset_alignmentHor;
	alignmentVert = preset_alignmentVert;

	if(t)
	{
		origText = strdup(t);
		text = charToWideChar(gettext(t));
	}

	for(int i=0; i < 20; i++)
		textDyn[i] = NULL;
}

/**
 * Destructor for the gui_text class.
 */
gui_text::~gui_text()
{
	if(origText)
		free(origText);
	if(text)
		delete[] text;

	if(textDynNum > 0)
	{
		for(int i=0; i < textDynNum; i++)
			if(textDyn[i])
				delete[] textDyn[i];
	}
}

void gui_text::set_text(const char * t)
{
	if(origText)
		free(origText);
	if(text)
		delete[] text;

	if(textDynNum > 0)
	{
		for(int i=0; i < textDynNum; i++)
			if(textDyn[i])
				delete[] textDyn[i];
	}

	origText = NULL;
	text = NULL;
	textDynNum = 0;
	textScrollPos = 0;
	textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;

	if(t)
	{
		origText = strdup(t);
		text = charToWideChar(gettext(t));
	}
}

void gui_text::SetWText(wchar_t * t)
{
	if(origText)
		free(origText);
	if(text)
		delete[] text;

	if(textDynNum > 0)
	{
		for(int i=0; i < textDynNum; i++)
			if(textDyn[i])
				delete[] textDyn[i];
	}

	origText = NULL;
	text = NULL;
	textDynNum = 0;
	textScrollPos = 0;
	textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;

	if(t)
		text = wcsdup(t);
}

int gui_text::get_length()
{
	if(!text)
		return 0;

	return wcslen(text);
}

void gui_text::SetPresets(int sz, GXColor c, int w, u16 s, int h, int v)
{
	presetSize = sz;
	preset_color = c;
	preset_style = s;
	preset_max_width = w;
	preset_alignmentHor = h;
	preset_alignmentVert = v;
}

void gui_text::SetFontSize(int s)
{
	size = s;
}

void gui_text::set_max_width(int width)
{
	maxWidth = width;

	for(int i=0; i < textDynNum; i++)
	{
		if(textDyn[i])
		{
			delete[] textDyn[i];
			textDyn[i] = NULL;
		}
	}

	textDynNum = 0;
}

int gui_text::get_text_width()
{
	if(!text)
		return 0;

	if(currentSize != size)
	{
		change_font_size(size);

		if(!fontSystem[size])
			fontSystem[size] = new FreeTypeGX(size);

		currentSize = size;
	}
	return fontSystem[size]->get_width(text);
}

void gui_text::SetWrap(bool w, int width)
{
	wrap = w;
	maxWidth = width;

	for(int i=0; i < textDynNum; i++)
	{
		if(textDyn[i])
		{
			delete[] textDyn[i];
			textDyn[i] = NULL;
		}
	}

	textDynNum = 0;
}

void gui_text::set_scroll(int s)
{
	if(textScroll == s)
		return;

	for(int i=0; i < textDynNum; i++)
	{
		if(textDyn[i])
		{
			delete[] textDyn[i];
			textDyn[i] = NULL;
		}
	}

	textDynNum = 0;

	textScroll = s;
	textScrollPos = 0;
	textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
	textScrollDelay = TEXT_SCROLL_DELAY;
}

void gui_text::set_color(GXColor c)
{
	color = c;
	alpha = c.a;
}

void gui_text::set_style(u16 s)
{
	style = s;
}

void gui_text::set_alignment(int hor, int vert)
{
	style = 0;

	switch(hor)
	{
		case ALIGN_LEFT:
			style |= FTGX_JUSTIFY_LEFT;
			break;
		case ALIGN_RIGHT:
			style |= FTGX_JUSTIFY_RIGHT;
			break;
		default:
			style |= FTGX_JUSTIFY_CENTER;
			break;
	}
	switch(vert)
	{
		case ALIGN_TOP:
			style |= FTGX_ALIGN_TOP;
			break;
		case ALIGN_BOTTOM:
			style |= FTGX_ALIGN_BOTTOM;
			break;
		default:
			style |= FTGX_ALIGN_MIDDLE;
			break;
	}

	alignmentHor = hor;
	alignmentVert = vert;
}

void gui_text::reset_text()
{
	if(!origText)
		return;
	if(text)
		delete[] text;

	text = charToWideChar(gettext(origText));

	for(int i=0; i < textDynNum; i++)
	{
		if(textDyn[i])
		{
			delete[] textDyn[i];
			textDyn[i] = NULL;
		}
	}

	textDynNum = 0;
	currentSize = 0;
}

/**
 * Draw the text on screen
 */
void gui_text::Draw()
{
	if(!text)
		return;

	if(!this->is_visible())
		return;

	GXColor c = color;
	c.a = this->get_alpha();

	int newSize = size*this->get_scale();

	if(newSize > MAX_FONT_SIZE)
		newSize = MAX_FONT_SIZE;

	if(newSize != currentSize)
	{
		change_font_size(newSize);
		if(!fontSystem[newSize])
			fontSystem[newSize] = new FreeTypeGX(newSize);
		currentSize = newSize;
	}

	if(maxWidth == 0)
	{
		fontSystem[currentSize]->draw_text(this->get_left(), this->get_top(), text, c, style);
		this->update_effects();
		return;
	}

	u32 textlen = wcslen(text);

	if(wrap)
	{
		if(textDynNum == 0)
		{
			u32 n = 0, ch = 0;
			int linenum = 0;
			int lastSpace = -1;
			int lastSpaceIndex = -1;

			while(ch < textlen && linenum < 20)
			{
				if(n == 0)
					textDyn[linenum] = new wchar_t[textlen + 1];

				textDyn[linenum][n] = text[ch];
				textDyn[linenum][n+1] = 0;

				if(text[ch] == ' ' || ch == textlen-1)
				{
					if(fontSystem[currentSize]->get_width(textDyn[linenum]) > maxWidth)
					{
						if(lastSpace >= 0)
						{
							textDyn[linenum][lastSpaceIndex] = 0; // discard space, and everything after
							ch = lastSpace; // go backwards to the last space
							lastSpace = -1; // we have used this space
							lastSpaceIndex = -1;
						}
						++linenum;
						n = -1;
					}
					else if(ch == textlen-1)
					{
						++linenum;
					}
				}
				if(text[ch] == ' ' && n >= 0)
				{
					lastSpace = ch;
					lastSpaceIndex = n;
				}
				++ch;
				++n;
			}
			textDynNum = linenum;
		}

		int lineheight = newSize + 6;
		int voffset = 0;

		if(alignmentVert == ALIGN_MIDDLE)
			voffset = (lineheight >> 1) * (1-textDynNum);

		int left = this->get_left();
		int top  = this->get_top() + voffset;

		for(int i=0; i < textDynNum; ++i)
			fontSystem[currentSize]->draw_text(left, top+i*lineheight, textDyn[i], c, style);
	}
	else
	{
		if(textDynNum == 0)
		{
			textDynNum = 1;
			textDyn[0] = wcsdup(text);
			int len = wcslen(textDyn[0]);

			while(fontSystem[currentSize]->get_width(textDyn[0]) > maxWidth)
				textDyn[0][--len] = 0;
		}

		if(textScroll == SCROLL_HORIZONTAL)
		{
			if(fontSystem[currentSize]->get_width(text) > maxWidth && (FrameTimer % textScrollDelay == 0))
			{
				if(textScrollInitialDelay)
				{
					--textScrollInitialDelay;
				}
				else
				{
					++textScrollPos;
					if((u32)textScrollPos > textlen)
					{
						textScrollPos = 0;
						textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
					}

					wcscpy(textDyn[0], &text[textScrollPos]);
					u32 dynlen = wcslen(textDyn[0]);

					if(dynlen+2 < textlen)
					{
						textDyn[0][dynlen] = ' ';
						textDyn[0][dynlen+1] = ' ';
						textDyn[0][dynlen+2] = 0;
						dynlen += 2;
					}

					if(fontSystem[currentSize]->get_width(textDyn[0]) > maxWidth)
					{
						while(fontSystem[currentSize]->get_width(textDyn[0]) > maxWidth)
							textDyn[0][--dynlen] = 0;
					}
					else
					{
						int i = 0;

						while(fontSystem[currentSize]->get_width(textDyn[0]) < maxWidth && dynlen+1 < textlen)
						{
							textDyn[0][dynlen] = text[i++];
							textDyn[0][++dynlen] = 0;
						}

						if(fontSystem[currentSize]->get_width(textDyn[0]) > maxWidth)
							textDyn[0][dynlen-2] = 0;
						else
							textDyn[0][dynlen-1] = 0;
					}
				}
			}
		}

		fontSystem[currentSize]->draw_text(this->get_left(), this->get_top(), textDyn[0], c, style);
	}
	this->update_effects();
}
