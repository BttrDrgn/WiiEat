/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * demo.h
 ***************************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

#define DEBUG
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <network.h>

#include "fs.hpp"
#include "menus/main_menu/main_menu.hpp"
#include "libwiigui/FreeTypeGX.h"
#include "libwiigui/video.h"
#include "libwiigui/audio.h"
#include "libwiigui/input.h"
#include "filelist.h"

class main
{
    public:
        static bool shutdown;
        static void shutdown_app();
};

extern FreeTypeGX *fontSystem[];

#endif
