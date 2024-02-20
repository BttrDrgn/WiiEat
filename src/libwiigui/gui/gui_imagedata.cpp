/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_imagedata.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../gui.h"

/**
 * Constructor for the gui_image_data class.
 */
gui_image_data::gui_image_data(const u8 * i, int maxw, int maxh)
{
	data = NULL;
	width = 0;
	height = 0;

	if(i)
		data = DecodePNG(i, &width, &height, maxw, maxh);
}

/**
 * Destructor for the gui_image_data class.
 */
gui_image_data::~gui_image_data()
{
	if(data)
	{
		free(data);
		data = NULL;
	}
}

u8 * gui_image_data::get_image()
{
	return data;
}

int gui_image_data::get_width()
{
	return width;
}

int gui_image_data::get_height()
{
	return height;
}
