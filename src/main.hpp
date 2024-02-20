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
#include <ogcsys.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <curl/curl.h>
#include <mbedtls/sha1.h>
#include <gctypes.h>
#include <wiisocket.h>
#include <fat.h>
#include <net/net.hpp>
#include <api/api.hpp>
#include <vector>
#include <string>
#include <filelist.h>

#include <fs/fs.hpp>
#include "menus/main_menu/main_menu.hpp"
#include "menus/restaurant_menu/restaurant_menu.hpp"
#include "menus/console_menu/console_menu.hpp"
#include "menus/address_menu/address_menu.hpp"
#include "menus/home_menu/home_menu.hpp"
#include "libwiigui/FreeTypeGX.h"
#include "libwiigui/video.h"
#include "libwiigui/audio.h"
#include "libwiigui/input.h"


class main
{
    public:
        static bool shutdown;
        static void shutdown_app();
};

extern FreeTypeGX *fontSystem[];

#endif
