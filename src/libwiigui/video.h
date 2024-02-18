/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * video.h
 * Video routines
 ***************************************************************************/

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <ogcsys.h>

void init_video ();
void stop_gx();
void ResetVideo_Menu();
void menu_render();
void menu_draw_img(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[], f32 degrees, f32 scaleX, f32 scaleY, u8 alphaF );
void menu_draw_rect(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled);

extern int screen_height;
extern int screen_width;
extern u32 FrameTimer;

#endif
