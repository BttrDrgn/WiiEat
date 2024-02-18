#ifndef _MENUS_H_
#define _MENUS_H_

#include "libwiigui/gui.h"
#include "libwiigui/input.h"
#include "libwiigui/filebrowser.h"
#include "filelist.h"

class menus
{
    public:
        enum state
        {
            MENU_EXIT = -1,
            MENU_NONE,
            MENU_MAIN,
            MENU_CONSOLE,
        };

        static bool gui_halt;
        static lwp_t gui_thread;
        static menus::state current_menu;
        static menus::state prev_menu;
        static gui_window* main_window;
        static gui_image_data* pointer[4];
        static bool shutdown;


        static void resume_gui()
        {
            menus::gui_halt = false;
            LWP_ResumeThread (menus::gui_thread);
        }

        static void halt_gui()
        {
            menus::gui_halt = true;

            // wait for thread to finish
            while(!LWP_ThreadIsSuspended(menus::gui_thread))
            {
                usleep(100);
            }
        }

        static void* update_gui (void *arg);

        static void init_gui_threads()
        {
            LWP_CreateThread(&menus::gui_thread, menus::update_gui, NULL, NULL, 0, 70);
        }
};

#endif