#include "menus.hpp"
#include "../main.hpp"

bool menus::gui_halt = true;
lwp_t menus::gui_thread = LWP_THREAD_NULL;
menus::state menus::current_menu = MENU_NONE;
menus::state menus::prev_menu = MENU_NONE;
gui_window* menus::main_window = NULL;
gui_image_data* menus::pointer[4];
bool menus::shutdown = false;

void* menus::update_gui(void *arg)
{
    int i;

    while(1)
    {
        if(menus::gui_halt)
        {
            LWP_SuspendThread(menus::gui_thread);
        }
        else
        {
            UpdatePads();
            menus::main_window->Draw();

            #ifdef HW_RVL
            for(i=3; i >= 0; i--) // so that player 1's cursor appears on top!
            {
                if(userInput[i].wpad->ir.valid)
                {
                    Menu_DrawImg(userInput[i].wpad->ir.x-48, userInput[i].wpad->ir.y-48,
                        96, 96, pointer[i]->GetImage(), userInput[i].wpad->ir.angle, 1, 1, 255);
                }
                DoRumble(i);
            }
            #endif

            Menu_Render();

            for(i=0; i < 4; i++)
            {
                menus::main_window->Update(&userInput[i]);
            }

            if(shutdown)
            {
                for(i = 0; i <= 255; i += 10)
                {
                    menus::main_window->Draw();
                    Menu_DrawRectangle(0,0,screenwidth,screenheight,(GXColor){0, 0, 0, i},1);
                    Menu_Render();
                }
                main::shutdown_app();
            }
        }
    }
    return NULL;
}