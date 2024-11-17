/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * demo.h
 ***************************************************************************/

#ifndef MAIN
#define MAIN

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
#include <time.h>

#include <io/io.hpp>
#include "menus/main_menu/main_menu.hpp"
#include "menus/restaurant_menu/restaurant_menu.hpp"
#include "menus/store_menu/store_menu.hpp"
#include "menus/console_menu/console_menu.hpp"
#include "menus/address_menu/address_menu.hpp"
#include "menus/home_menu/home_menu.hpp"
#include "menus/cart_menu/cart_menu.hpp"
#include "menus/payment_menu/payment_menu.hpp"
#include "libwiigui/FreeTypeGX.h"
#include "libwiigui/video.h"
#include "libwiigui/audio.h"
#include "libwiigui/input.h"

class main
{
    public:
        static bool dark_mode;
        static bool shutdown;
        static void shutdown_app();
};

extern FreeTypeGX *fontSystem[];

#endif /* MAIN */
