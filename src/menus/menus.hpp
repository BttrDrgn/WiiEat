#ifndef MENUS
#define MENUS

#include "libwiigui/gui.h"
#include "libwiigui/input.h"
#include "libwiigui/filebrowser.h"
#include "filelist.h"
#include <unordered_map>

class menus
{
    public:
        enum state
        {
            MENU_EXIT = -1,
            MENU_NONE,
            MENU_CANCEL,
            MENU_MAIN,
            MENU_ADDRESS,
            MENU_RESTAURANT,
            MENU_CONSOLE,
        };

        static bool gui_halt;
        static lwp_t gui_thread;
        static menus::state current_menu;
        static menus::state prev_menu;
        static gui_window* main_window;
        static gui_image_data* pointer[4];
        static gui_image* bg_img;
        static bool shutdown;
        static gui_sound* current_song;

        static void initialize(state menu);
        static void update();
        static void shutdown_app();

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

        static state next(menus::state next_menu)
        {
            prev_menu = current_menu;
            return next_menu;
        }

        static void keyboard(char* var, u16 maxlen);
        static void num_keyboard(char* var, u16 maxlen);
        static int window_prompt(const char *title, const char *msg, const char *btn1Label, const char* btn2Label = "");

};

#endif /* MENUS */
