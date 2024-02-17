/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * menu.h
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#ifndef _DEMO_MENU_H_
#define _DEMO_MENU_H_

#include <ogcsys.h>

void InitGUIThreads();
void DemoMenu (int menuitem);

enum
{
	MENU_EXIT = -1,
	MENU_NONE,
	MENU_SETTINGS,
	MENU_SETTINGS_FILE,
	MENU_BROWSE_DEVICE
};

#endif
