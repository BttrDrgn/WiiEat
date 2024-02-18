/*!\mainpage libwiigui Documentation
 *
 * \section Introduction
 * libwiigui is a GUI library for the Wii, created to help structure the
 * design of a complicated GUI interface, and to enable an author to create
 * a sophisticated, feature-rich GUI. It was originally conceived and written
 * after I started to design a GUI for Snes9x GX, and found libwiisprite and
 * GRRLIB inadequate for the purpose. It uses GX for drawing, and makes use
 * of PNGU for displaying images and FreeTypeGX for text. It was designed to
 * be flexible and is easy to modify - don't be afraid to change the way it
 * works or expand it to suit your GUI's purposes! If you do, and you think
 * your changes might benefit others, please share them so they might be
 * added to the project!
 *
 * \section Quickstart
 * Start from the supplied template example. For more advanced uses, see the
 * source code for Snes9x GX, FCE Ultra GX, and Visual Boy Advance GX.

 * \section Contact
 * If you have any suggestions for the library or documentation, or want to
 * contribute, please visit the libwiigui website:
 * http://code.google.com/p/libwiigui/

 * \section Credits
 * This library was wholly designed and written by Tantric. Thanks to the
 * authors of PNGU and FreeTypeGX, of which this library makes use. Thanks
 * also to the authors of GRRLIB and libwiisprite for laying the foundations.
 *
*/

#ifndef LIBWIIGUI_H
#define LIBWIIGUI_H

#include <gccore.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <exception>
#include <wchar.h>
#include <math.h>
#include <asndlib.h>
#include <wiiuse/wpad.h>

#include "pngu.h"
#include "FreeTypeGX.h"
#include "video.h"
#include "filelist.h"
#include "input.h"
#include "oggplayer.h"

extern FreeTypeGX *fontSystem[];

#define SCROLL_DELAY_INITIAL	200000
#define SCROLL_DELAY_LOOP		30000
#define SCROLL_DELAY_DECREASE	300
#define FILE_PAGESIZE 			8
#define PAGESIZE 				8
#define MAX_OPTIONS 			150
#define MAX_KEYBOARD_DISPLAY	32

typedef void (*update_callback)(void * e);

enum
{
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_TOP,
	ALIGN_BOTTOM,
	ALIGN_MIDDLE
};

enum
{
	STATE_DEFAULT,
	STATE_SELECTED,
	STATE_CLICKED,
	STATE_HELD,
	STATE_DISABLED
};

enum
{
	SOUND_PCM,
	SOUND_OGG
};

enum
{
	IMAGE_TEXTURE,
	IMAGE_COLOR,
	IMAGE_DATA
};

enum
{
	TRIGGER_SIMPLE,
	TRIGGER_HELD,
	TRIGGER_BUTTON_ONLY,
	TRIGGER_BUTTON_ONLY_IN_FOCUS
};

enum
{
	SCROLL_NONE,
	SCROLL_HORIZONTAL
};

typedef struct _paddata {
	u16 btns_d;
	u16 btns_u;
	u16 btns_h;
	s8 stickX;
	s8 stickY;
	s8 substickX;
	s8 substickY;
	u8 triggerL;
	u8 triggerR;
} PADData;

#define EFFECT_SLIDE_TOP			1
#define EFFECT_SLIDE_BOTTOM			2
#define EFFECT_SLIDE_RIGHT			4
#define EFFECT_SLIDE_LEFT			8
#define EFFECT_SLIDE_IN				16
#define EFFECT_SLIDE_OUT			32
#define EFFECT_FADE					64
#define EFFECT_SCALE				128
#define EFFECT_COLOR_TRANSITION		256

//!Sound conversion and playback. A wrapper for other sound libraries - ASND, libmad, ltremor, etc
class gui_sound
{
	public:
		//!Constructor
		//!\param s Pointer to the sound data
		//!\param l Length of sound data
		//!\param t Sound format type (SOUND_PCM or SOUND_OGG)
		gui_sound(const u8 * s, s32 l, int t);
		//!Destructor
		~gui_sound();
		//!Start sound playback
		void Play();
		//!Stop sound playback
		void Stop();
		//!Pause sound playback
		void Pause();
		//!Resume sound playback
		void Resume();
		//!Checks if the sound is currently playing
		//!\return true if sound is playing, false otherwise
		bool IsPlaying();
		//!Set sound volume
		//!\param v Sound volume (0-100)
		void SetVolume(int v);
		//!Set the sound to loop playback (only applies to OGG)
		//!\param l Loop (true to loop)
		void SetLoop(bool l);
	protected:
		const u8 * sound; //!< Pointer to the sound data
		int type; //!< Sound format type (SOUND_PCM or SOUND_OGG)
		s32 length; //!< Length of sound data
		s32 voice; //!< Currently assigned ASND voice channel
		s32 volume; //!< Sound volume (0-100)
		bool loop; //!< Loop sound playback
};

//!Menu input trigger management. Determine if action is neccessary based on input data by comparing controller input data to a specific trigger element.
class gui_trigger
{
	public:
		//!Constructor
		gui_trigger();
		//!Destructor
		~gui_trigger();
		//!Sets a simple trigger. Requires: element is selected, and trigger button is pressed
		//!\param ch Controller channel number
		//!\param wiibtns Wii controller trigger button(s) - classic controller buttons are considered separately
		//!\param gcbtns GameCube controller trigger button(s)
		void set_simple_trigger(s32 ch, u32 wiibtns, u16 gcbtns);
		//!Sets a held trigger. Requires: element is selected, and trigger button is pressed
		//!\param ch Controller channel number
		//!\param wiibtns Wii controller trigger button(s) - classic controller buttons are considered separately
		//!\param gcbtns GameCube controller trigger button(s)
		void SetHeldTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
		//!Sets a button-only trigger. Requires: Trigger button is pressed
		//!\param ch Controller channel number
		//!\param wiibtns Wii controller trigger button(s) - classic controller buttons are considered separately
		//!\param gcbtns GameCube controller trigger button(s)
		void set_button_only_trigger(s32 ch, u32 wiibtns, u16 gcbtns);
		//!Sets a button-only trigger. Requires: trigger button is pressed and parent window of element is in focus
		//!\param ch Controller channel number
		//!\param wiibtns Wii controller trigger button(s) - classic controller buttons are considered separately
		//!\param gcbtns GameCube controller trigger button(s)
		void SetButtonOnlyInFocusTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
		//!Get X or Y value from Wii Joystick (classic, nunchuk) input
		//!\param stick Controller stick (left = 0, right = 1)
		//!\param axis Controller stick axis (x-axis = 0, y-axis = 1)
		//!\return Stick value
		s8 WPAD_Stick(u8 stick, int axis);
		//!Get X value from Wii Joystick (classic, nunchuk) input
		//!\param stick Controller stick (left = 0, right = 1)
		//!\return Stick value
		s8 WPAD_StickX(u8 stick);
		//!Get Y value from Wii Joystick (classic, nunchuk) input
		//!\param stick Controller stick (left = 0, right = 1)
		//!\return Stick value
		s8 WPAD_StickY(u8 stick);
		//!Move menu selection left (via pad/joystick). Allows scroll delay and button overriding
		//!\return true if selection should be moved left, false otherwise
		bool Left();
		//!Move menu selection right (via pad/joystick). Allows scroll delay and button overriding
		//!\return true if selection should be moved right, false otherwise
		bool Right();
		//!Move menu selection up (via pad/joystick). Allows scroll delay and button overriding
		//!\return true if selection should be moved up, false otherwise
		bool Up();
		//!Move menu selection down (via pad/joystick). Allows scroll delay and button overriding
		//!\return true if selection should be moved down, false otherwise
		bool Down();

		WPADData wpaddata; //!< Wii controller trigger data
		PADData pad; //!< GameCube controller trigger data
		WPADData * wpad; //!< Wii controller trigger
		s32 chan; //!< Trigger controller channel (0-3, -1 for all)
		u8 type; //!< trigger type (TRIGGER_SIMPLE,	TRIGGER_HELD, TRIGGER_BUTTON_ONLY, TRIGGER_BUTTON_ONLY_IN_FOCUS)
};

extern gui_trigger userInput[4];

//!Primary GUI class. Most other classes inherit from this class.
class gui_element
{
	public:
		//!Constructor
		gui_element();
		//!Destructor
		~gui_element();
		//!Set the element's parent
		//!\param e Pointer to parent element
		void set_parent(gui_element * e);
		//!Gets the element's parent
		//!\return Pointer to parent element
		gui_element * GetParent();
		//!Gets the current leftmost coordinate of the element
		//!Considers horizontal alignment, x offset, width, and parent element's GetLeft() / GetWidth() values
		//!\return left coordinate
		int GetLeft();
		//!Gets the current topmost coordinate of the element
		//!Considers vertical alignment, y offset, height, and parent element's GetTop() / GetHeight() values
		//!\return top coordinate
		int GetTop();
		//!Sets the minimum y offset of the element
		//!\param y Y offset
		void SetMinY(int y);
		//!Gets the minimum y offset of the element
		//!\return Minimum Y offset
		int GetMinY();
		//!Sets the maximum y offset of the element
		//!\param y Y offset
		void SetMaxY(int y);
		//!Gets the maximum y offset of the element
		//!\return Maximum Y offset
		int GetMaxY();
		//!Sets the minimum x offset of the element
		//!\param x X offset
		void SetMinX(int x);
		//!Gets the minimum x offset of the element
		//!\return Minimum X offset
		int GetMinX();
		//!Sets the maximum x offset of the element
		//!\param x X offset
		void SetMaxX(int x);
		//!Gets the maximum x offset of the element
		//!\return Maximum X offset
		int GetMaxX();
		//!Gets the current width of the element. Does not currently consider the scale
		//!\return width
		int GetWidth();
		//!Gets the height of the element. Does not currently consider the scale
		//!\return height
		int GetHeight();
		//!Sets the size (width/height) of the element
		//!\param w Width of element
		//!\param h Height of element
		void SetSize(int w, int h);
		//!Checks whether or not the element is visible
		//!\return true if visible, false otherwise
		bool is_visible();
		//!Checks whether or not the element is selectable
		//!\return true if selectable, false otherwise
		bool IsSelectable();
		//!Checks whether or not the element is clickable
		//!\return true if clickable, false otherwise
		bool IsClickable();
		//!Checks whether or not the element is holdable
		//!\return true if holdable, false otherwise
		bool IsHoldable();
		//!Sets whether or not the element is selectable
		//!\param s Selectable
		void SetSelectable(bool s);
		//!Sets whether or not the element is clickable
		//!\param c Clickable
		void SetClickable(bool c);
		//!Sets whether or not the element is holdable
		//!\param h Holdable
		void SetHoldable(bool h);
		//!Gets the element's current state
		//!\return state
		int get_state();
		//!Gets the controller channel that last changed the element's state
		//!\return Channel number (0-3, -1 = no channel)
		int get_stateChan();
		//!Sets the element's alpha value
		//!\param a alpha value
		void SetAlpha(int a);
		//!Gets the element's alpha value
		//!Considers alpha, alphaDyn, and the parent element's GetAlpha() value
		//!\return alpha
		int GetAlpha();
		//!Sets the element's x and y scale
		//!\param s scale (1 is 100%)
		void set_scale(float s);
		//!Sets the element's x scale
		//!\param s scale (1 is 100%)
		void set_scale_x(float s);
		//!Sets the element's y scale
		//!\param s scale (1 is 100%)
		void set_scale_y(float s);
		//!Sets the element's x and y scale, using the provided max width/height
		//!\param w Maximum width
		//!\param h Maximum height
		void set_scale(int w, int h);
		//!Gets the element's current scale
		//!Considers scale, scaleDyn, and the parent element's GetScale() value
		float GetScale();
		//!Gets the element's current x scale
		//!Considers scale, scaleDyn, and the parent element's GetScale() value
		float GetScaleX();
		//!Gets the element's current y scale
		//!Considers scale, scaleDyn, and the parent element's GetScale() value
		float GetScaleY();
		//!Set a new gui_trigger for the element
		//!\param t Pointer to gui_trigger
		void set_trigger(gui_trigger * t);
		//!\overload
		//!\param i Index of trigger array to set
		//!\param t Pointer to gui_trigger
		void set_trigger(u8 i, gui_trigger * t);
		//!Checks whether rumble was requested by the element
		//!\return true is rumble was requested, false otherwise
		bool Rumble();
		//!Sets whether or not the element is requesting a rumble event
		//!\param r true if requesting rumble, false if not
		void SetRumble(bool r);
		//!Set an effect for the element
		//!\param e Effect to enable
		//!\param a Amount of the effect (usage varies on effect)
		//!\param t Target amount of the effect (usage varies on effect)
		void SetEffect(int e, int a, int t=0);
		//!Sets an effect to be enabled on wiimote cursor over
		//!\param e Effect to enable
		//!\param a Amount of the effect (usage varies on effect)
		//!\param t Target amount of the effect (usage varies on effect)
		void SetEffectOnOver(int e, int a, int t=0);
		//!Shortcut to SetEffectOnOver(EFFECT_SCALE, 4, 110)
		void SetEffectGrow();
		//!Gets the current element effects
		//!\return element effects
		int GetEffect();
		//!Checks whether the specified coordinates are within the element's boundaries
		//!\param x X coordinate
		//!\param y Y coordinate
		//!\return true if contained within, false otherwise
		bool IsInside(int x, int y);
		//!Sets the element's position
		//!\param x X coordinate
		//!\param y Y coordinate
		void set_position(int x, int y);
		//!Updates the element's effects (dynamic values)
		//!Called by Draw(), used for animation purposes
		void update_effects();
		//!Sets a function to called after after Update()
		//!Callback function can be used to response to changes in the state of the element, and/or update the element's attributes
		void set_update_callback(update_callback u);
		//!Checks whether the element is in focus
		//!\return true if element is in focus, false otherwise
		int IsFocused();
		//!Sets the element's visibility
		//!\param v Visibility (true = visible)
		virtual void SetVisible(bool v);
		//!Sets the element's focus
		//!\param f Focus (true = in focus)
		virtual void SetFocus(int f);
		//!Sets the element's state
		//!\param s State (STATE_DEFAULT, STATE_SELECTED, STATE_CLICKED, STATE_DISABLED)
		//!\param c Controller channel (0-3, -1 = none)
		virtual void set_state(int s, int c = -1);
		//!Resets the element's state to STATE_DEFAULT
		virtual void reset_state();
		//!Gets whether or not the element is in STATE_SELECTED
		//!\return true if selected, false otherwise
		virtual int GetSelected();
		//!Sets the element's alignment respective to its parent element
		//!\param hor Horizontal alignment (ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER)
		//!\param vert Vertical alignment (ALIGN_TOP, ALIGN_BOTTOM, ALIGN_MIDDLE)
		virtual void set_alignment(int hor, int vert);
		//!Called when the language has changed, to obtain new text values for all text elements
		virtual void reset_text();
		//!Called constantly to allow the element to respond to the current input data
		//!\param t Pointer to a gui_trigger, containing the current input data from PAD/WPAD
		virtual void Update(gui_trigger * t);
		//!Called constantly to redraw the element
		virtual void Draw();
		//!Called constantly to redraw the element's tooltip
		virtual void draw_tool_tip();
	protected:
		gui_trigger * trigger[3]; //!< gui_triggers (input actions) that this element responds to
		update_callback update_cb; //!< Callback function to call when this element is updated
		gui_element * parentElement; //!< Parent element
		int focus; //!< Element focus (-1 = focus disabled, 0 = not focused, 1 = focused)
		int width; //!< Element width
		int height; //!< Element height
		int xoffset; //!< Element X offset
		int yoffset; //!< Element Y offset
		int ymin; //!< Element's min Y offset allowed
		int ymax; //!< Element's max Y offset allowed
		int xmin; //!< Element's min X offset allowed
		int xmax; //!< Element's max X offset allowed
		int xoffsetDyn; //!< Element X offset, dynamic (added to xoffset value for animation effects)
		int yoffsetDyn; //!< Element Y offset, dynamic (added to yoffset value for animation effects)
		int alpha; //!< Element alpha value (0-255)
		int alphaDyn; //!< Element alpha, dynamic (multiplied by alpha value for blending/fading effects)
		f32 xscale; //!< Element X scale (1 = 100%)
		f32 yscale; //!< Element Y scale (1 = 100%)
		f32 scaleDyn; //!< Element scale, dynamic (multiplied by alpha value for blending/fading effects)
		int effects; //!< Currently enabled effect(s). 0 when no effects are enabled
		int effectAmount; //!< Effect amount. Used by different effects for different purposes
		int effectTarget; //!< Effect target amount. Used by different effects for different purposes
		int effectsOver; //!< Effects to enable when wiimote cursor is over this element. Copied to effects variable on over event
		int effectAmountOver; //!< EffectAmount to set when wiimote cursor is over this element
		int effectTargetOver; //!< EffectTarget to set when wiimote cursor is over this element
		int alignmentHor; //!< Horizontal element alignment, respective to parent element (LEFT, RIGHT, CENTRE)
		int alignmentVert; //!< Horizontal element alignment, respective to parent element (TOP, BOTTOM, MIDDLE)
		int state; //!< Element state (DEFAULT, SELECTED, CLICKED, DISABLED)
		int stateChan; //!< Which controller channel is responsible for the last change in state
		bool selectable; //!< Whether or not this element selectable (can change to SELECTED state)
		bool clickable; //!< Whether or not this element is clickable (can change to CLICKED state)
		bool holdable; //!< Whether or not this element is holdable (can change to HELD state)
		bool visible; //!< Visibility of the element. If false, Draw() is skipped
		bool rumble; //!< Wiimote rumble (on/off) - set to on when this element requests a rumble event
};

//!Allows gui_elements to be grouped together into a "window"
class gui_window : public gui_element
{
	public:
		//!Constructor
		gui_window();
		//!\overload
		//!\param w Width of window
		//!\param h Height of window
		gui_window(int w, int h);
		//!Destructor
		~gui_window();
		//!appends a gui_element to the gui_window
		//!\param e The gui_element to append. If it is already in the gui_window, it is removed first
		void append(gui_element* e);
		//!Inserts a gui_element into the gui_window at the specified index
		//!\param e The gui_element to insert. If it is already in the gui_window, it is removed first
		//!\param i Index in which to insert the element
		void Insert(gui_element* e, u32 i);
		//!removes the specified gui_element from the gui_window
		//!\param e gui_element to be removed
		void remove(gui_element* e);
		//!removes all gui_elements
		void removeAll();
		//!Looks for the specified gui_element
		//!\param e The gui_element to find
		//!\return true if found, false otherwise
		bool Find(gui_element* e);
		//!Returns the gui_element at the specified index
		//!\param index The index of the element
		//!\return A pointer to the element at the index, NULL on error (eg: out of bounds)
		gui_element* Getgui_elementAt(u32 index) const;
		//!Returns the size of the list of elements
		//!\return The size of the current element list
		u32 get_size();
		//!Sets the visibility of the window
		//!\param v visibility (true = visible)
		void SetVisible(bool v);
		//!Resets the window's state to STATE_DEFAULT
		void reset_state();
		//!Sets the window's state
		//!\param s State
		void set_state(int s);
		//!Gets the index of the gui_element inside the window that is currently selected
		//!\return index of selected gui_element
		int GetSelected();
		//!Sets the window focus
		//!\param f Focus
		void SetFocus(int f);
		//!Change the focus to the specified element
		//!This is intended for the primary gui_window only
		//!\param e gui_element that should have focus
		void change_focus(gui_element * e);
		//!Changes window focus to the next focusable window or element
		//!If no element is in focus, changes focus to the first available element
		//!If B or 1 button is pressed, changes focus to the next available element
		//!This is intended for the primary gui_window only
		//!\param t Pointer to a gui_trigger, containing the current input data from PAD/WPAD
		void ToggleFocus(gui_trigger * t);
		//!Moves the selected element to the element to the left or right
		//!\param d Direction to move (-1 = left, 1 = right)
		void MoveSelectionHor(int d);
		//!Moves the selected element to the element above or below
		//!\param d Direction to move (-1 = up, 1 = down)
		void MoveSelectionVert(int d);
		//!Resets the text for all contained elements
		void reset_text();
		//!Draws all the elements in this gui_window
		void Draw();
		//!Draws all of the tooltips in this gui_window
		void draw_tool_tip();
		//!Updates the window and all elements contains within
		//!Allows the gui_window and all elements to respond to the input data specified
		//!\param t Pointer to a gui_trigger, containing the current input data from PAD/WPAD
		void Update(gui_trigger * t);
	protected:
		std::vector<gui_element*> _elements; //!< Contains all elements within the gui_window
};

//!Converts image data into GX-useable RGBA8. Currently designed for use only with PNG files
class gui_image_data
{
	public:
		//!Constructor
		//!Converts the image data to RGBA8 - expects PNG format
		//!\param i Image data
		//!\param w Max image width (0 = not set)
		//!\param h Max image height (0 = not set)
		gui_image_data(const u8 * i, int w=0, int h=0);
		//!Destructor
		~gui_image_data();
		//!Gets a pointer to the image data
		//!\return pointer to image data
		u8 * get_image();
		//!Gets the image width
		//!\return image width
		int GetWidth();
		//!Gets the image height
		//!\return image height
		int GetHeight();
	protected:
		u8 * data; //!< Image data
		int height; //!< Height of image
		int width; //!< Width of image
};

//!Display, manage, and manipulate images in the GUI
class gui_image : public gui_element
{
	public:
		//!Constructor
		gui_image();
		//!\overload
		//!\param img Pointer to gui_image_data element
		gui_image(gui_image_data * img);
		//!\overload
		//!Sets up a new image from the image data specified
		//!\param img
		//!\param w Image width
		//!\param h Image height
		gui_image(u8 * img, int w, int h);
		//!\overload
		//!Creates an image filled with the specified color
		//!\param w Image width
		//!\param h Image height
		//!\param c Image color
		gui_image(int w, int h, GXColor c);
		//!Destructor
		~gui_image();
		//!Sets the image rotation angle for drawing
		//!\param a Angle (in degrees)
		void SetAngle(float a);
		//!Sets the number of times to draw the image horizontally
		//!\param t Number of times to draw the image
		void SetTile(int t);
		//!Constantly called to draw the image
		void Draw();
		//!Gets the image data
		//!\return pointer to image data
		u8 * get_image();
		//!Sets up a new image using the gui_image_data object specified
		//!\param img Pointer to gui_image_data object
		void set_image(gui_image_data * img);
		//!\overload
		//!\param img Pointer to image data
		//!\param w Width
		//!\param h Height
		void set_image(u8 * img, int w, int h);
		//!Gets the pixel color at the specified coordinates of the image
		//!\param x X coordinate
		//!\param y Y coordinate
		GXColor GetPixel(int x, int y);
		//!Sets the pixel color at the specified coordinates of the image
		//!\param x X coordinate
		//!\param y Y coordinate
		//!\param color Pixel color
		void SetPixel(int x, int y, GXColor color);
		//!Directly modifies the image data to create a color-striped effect
		//!Alters the RGB values by the specified amount
		//!\param s Amount to increment/decrement the RGB values in the image
		void ColorStripe(int s);
		//!Sets a stripe effect on the image, overlaying alpha blended rectangles
		//!Does not alter the image data
		//!\param s Alpha amount to draw over the image
		void SetStripe(int s);
	protected:
		int imgType; //!< Type of image data (IMAGE_TEXTURE, IMAGE_COLOR, IMAGE_DATA)
		u8 * image; //!< Poiner to image data. May be shared with gui_image_data data
		f32 imageangle; //!< Angle to draw the image
		int tile; //!< Number of times to draw (tile) the image horizontally
		int stripe; //!< Alpha value (0-255) to apply a stripe effect to the texture
};

//!Display, manage, and manipulate text in the GUI
class gui_text : public gui_element
{
	public:
		//!Constructor
		//!\param t Text
		//!\param s Font size
		//!\param c Font color
		gui_text(const char * t, int s, GXColor c);
		//!\overload
		//!Assumes SetPresets() has been called to setup preferred text attributes
		//!\param t Text
		gui_text(const char * t);
		//!Destructor
		~gui_text();
		//!Sets the text of the gui_text element
		//!\param t Text
		void set_text(const char * t);
		//!Sets the text of the gui_text element
		//!\param t UTF-8 Text
		void SetWText(wchar_t * t);
		//!Gets the translated text length of the gui_text element
		int GetLength();
		//!Sets up preset values to be used by gui_text(t)
		//!Useful when printing multiple text elements, all with the same attributes set
		//!\param sz Font size
		//!\param c Font color
		//!\param w Maximum width of texture image (for text wrapping)
		//!\param s Font size
		//!\param h Text alignment (horizontal)
		//!\param v Text alignment (vertical)
		void SetPresets(int sz, GXColor c, int w, u16 s, int h, int v);
		//!Sets the font size
		//!\param s Font size
		void SetFontSize(int s);
		//!Sets the maximum width of the drawn texture image
		//!\param w Maximum width
		void SetMaxWidth(int w);
		//!Gets the width of the text when rendered
		int GetTextWidth();
		//!Enables/disables text scrolling
		//!\param s Scrolling on/off
		void SetScroll(int s);
		//!Enables/disables text wrapping
		//!\param w Wrapping on/off
		//!\param width Maximum width (0 to disable)
		void SetWrap(bool w, int width = 0);
		//!Sets the font color
		//!\param c Font color
		void SetColor(GXColor c);
		//!Sets the FreeTypeGX style attributes
		//!\param s Style attributes
		void SetStyle(u16 s);
		//!Sets the text alignment
		//!\param hor Horizontal alignment (ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER)
		//!\param vert Vertical alignment (ALIGN_TOP, ALIGN_BOTTOM, ALIGN_MIDDLE)
		void set_alignment(int hor, int vert);
		//!Updates the text to the selected language
		void reset_text();
		//!Constantly called to draw the text
		void Draw();
	protected:
		GXColor color; //!< Font color
		wchar_t* text; //!< Translated Unicode text value
		wchar_t *textDyn[20]; //!< Text value, if max width, scrolling, or wrapping enabled
		int textDynNum; //!< Number of text lines
		char * origText; //!< Original text data (English)
		int size; //!< Font size
		int maxWidth; //!< Maximum width of the generated text object (for text wrapping)
		int textScroll; //!< Scrolling toggle
		int textScrollPos; //!< Current starting index of text string for scrolling
		int textScrollInitialDelay; //!< Delay to wait before starting to scroll
		int textScrollDelay; //!< Scrolling speed
		u16 style; //!< FreeTypeGX style attributes
		bool wrap; //!< Wrapping toggle
};

//!Display, manage, and manipulate tooltips in the GUI
class gui_tooltip : public gui_element
{
	public:
		//!Constructor
		//!\param t Text
		gui_tooltip(const char *t);
		//!Destructor
		~gui_tooltip();
		//!Gets the element's current scale
		float GetScale();
		//!Sets the text of the gui_tooltip element
		//!\param t Text
		void set_text(const char * t);
		//!Constantly called to draw the gui_tooltip
		void draw_tool_tip();
	
		time_t time1, time2; //!< Tooltip times

	protected:
		gui_image leftImage; //!< Tooltip left image
		gui_image tileImage; //!< Tooltip tile image
		gui_image rightImage; //!< Tooltip right image
		gui_text *text; //!< Tooltip text
};

//!Display, manage, and manipulate buttons in the GUI. Buttons can have images, icons, text, and sound set (all of which are optional)
class gui_button : public gui_element
{
	public:
		//!Constructor
		//!\param w Width
		//!\param h Height
		gui_button(int w = 0, int h = 0);
		//!Destructor
		~gui_button();
		//!Sets the button's image
		//!\param i Pointer to gui_image object
		void set_image(gui_image* i);
		//!Sets the button's image on over
		//!\param i Pointer to gui_image object
		void set_image_hover(gui_image* i);
		//!Sets the button's image on hold
		//!\param i Pointer to gui_image object
		void set_image_hold(gui_image* i);
		//!Sets the button's image on click
		//!\param i Pointer to gui_image object
		void set_image_click(gui_image* i);
		//!Sets the button's icon
		//!\param i Pointer to gui_image object
		void set_icon(gui_image* i);
		//!Sets the button's icon on over
		//!\param i Pointer to gui_image object
		void set_icon_hover(gui_image* i);
		//!Sets the button's icon on hold
		//!\param i Pointer to gui_image object
		void set_icon_hold(gui_image* i);
		//!Sets the button's icon on click
		//!\param i Pointer to gui_image object
		void set_icon_click(gui_image* i);
		//!Sets the button's label
		//!\param t Pointer to gui_text object
		//!\param n Index of label to set (optional, default is 0)
		void set_label(gui_text* t, int n = 0);
		//!Sets the button's label on over (eg: different colored text)
		//!\param t Pointer to gui_text object
		//!\param n Index of label to set (optional, default is 0)
		void set_label_hover(gui_text* t, int n = 0);
		//!Sets the button's label on hold
		//!\param t Pointer to gui_text object
		//!\param n Index of label to set (optional, default is 0)
		void set_label_hold(gui_text* t, int n = 0);
		//!Sets the button's label on click
		//!\param t Pointer to gui_text object
		//!\param n Index of label to set (optional, default is 0)
		void set_label_click(gui_text* t, int n = 0);
		//!Sets the sound to play on over
		//!\param s Pointer to gui_sound object
		void set_sound_hover(gui_sound * s);
		//!Sets the sound to play on hold
		//!\param s Pointer to gui_sound object
		void set_sound_hold(gui_sound * s);
		//!Sets the sound to play on click
		//!\param s Pointer to gui_sound object
		void set_sound_click(gui_sound * s);
		//!Sets the tooltip for the button
		//!\param t Tooltip
		void set_tooltip(gui_tooltip * t);
		//!Constantly called to draw the gui_button
		void Draw();
		//!Constantly called to draw the gui_button's tooltip
		void draw_tool_tip();
		//!Resets the text for all contained elements
		void reset_text();
		//!Constantly called to allow the gui_button to respond to updated input data
		//!\param t Pointer to a gui_trigger, containing the current input data from PAD/WPAD
		void Update(gui_trigger * t);
	protected:
		gui_image * image; //!< Button image (default)
		gui_image * image_hover; //!< Button image for STATE_SELECTED
		gui_image * image_hold; //!< Button image for STATE_HELD
		gui_image * image_click; //!< Button image for STATE_CLICKED
		gui_image * icon; //!< Button icon (drawn after button image)
		gui_image * icon_hover; //!< Button icon for STATE_SELECTED
		gui_image * icon_hold; //!< Button icon for STATE_HELD
		gui_image * icon_click; //!< Button icon for STATE_CLICKED
		gui_text * label[3]; //!< Label(s) to display (default)
		gui_text * label_hover[3]; //!< Label(s) to display for STATE_SELECTED
		gui_text * label_hold[3]; //!< Label(s) to display for STATE_HELD
		gui_text * label_click[3]; //!< Label(s) to display for STATE_CLICKED
		gui_sound * sound_hover; //!< Sound to play for STATE_SELECTED
		gui_sound * sound_hold; //!< Sound to play for STATE_HELD
		gui_sound * sound_click; //!< Sound to play for STATE_CLICKED
		gui_tooltip * tooltip; //!< Tooltip to display on over
};

typedef struct _keytype {
	char ch, chShift;
} Key;

//!On-screen keyboard
class gui_keyboard : public gui_window
{
	public:
		gui_keyboard(char * t, u32 m);
		~gui_keyboard();
		void Update(gui_trigger * t);
		char kbtextstr[256];
	protected:
		u32 kbtextmaxlen;
		int shift;
		int caps;
		gui_text * kbText;
		gui_image * keyTextboxImg;
		gui_text * keyCapsText;
		gui_image * keyCapsImg;
		gui_image * keyCapsOverImg;
		gui_button * keyCaps;
		gui_text * keyShiftText;
		gui_image * keyShiftImg;
		gui_image * keyShiftOverImg;
		gui_button * keyShift;
		gui_text * keyBackText;
		gui_image * keyBackImg;
		gui_image * keyBackOverImg;
		gui_button * keyBack;
		gui_image * keySpaceImg;
		gui_image * keySpaceOverImg;
		gui_button * keySpace;
		gui_button * keyBtn[4][11];
		gui_image * keyImg[4][11];
		gui_image * keyImgOver[4][11];
		gui_text * keyTxt[4][11];
		gui_image_data * keyTextbox;
		gui_image_data * key;
		gui_image_data * keyOver;
		gui_image_data * keyMedium;
		gui_image_data * keyMediumOver;
		gui_image_data * keyLarge;
		gui_image_data * keyLargeOver;
		gui_sound * keysound_hover;
		gui_sound * keysound_click;
		gui_trigger * trig_a;
		gui_trigger * trig2;
		Key keys[4][11]; // two chars = less space than one pointer
};

typedef struct _optionlist {
	int length;
	char name[MAX_OPTIONS][50];
	char value[MAX_OPTIONS][50];
} OptionList;

//!Display a list of menu options
class GuiOptionBrowser : public gui_element
{
	public:
		GuiOptionBrowser(int w, int h, OptionList * l);
		~GuiOptionBrowser();
		void SetCol1Position(int x);
		void SetCol2Position(int x);
		int FindMenuItem(int c, int d);
		int GetClickedOption();
		void reset_state();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void reset_text();
		void Update(gui_trigger * t);
		gui_text * optionVal[PAGESIZE];
	protected:
		int optionIndex[PAGESIZE];
		gui_button * optionBtn[PAGESIZE];
		gui_text * optionTxt[PAGESIZE];
		gui_image * optionBg[PAGESIZE];

		int selectedItem;
		int listOffset;
		OptionList * options;

		gui_button * arrowUpBtn;
		gui_button * arrowDownBtn;

		gui_image * bgOptionsImg;
		gui_image * scrollbarImg;
		gui_image * arrowDownImg;
		gui_image * arrowDownOverImg;
		gui_image * arrowUpImg;
		gui_image * arrowUpOverImg;

		gui_image_data * bgOptions;
		gui_image_data * bgOptionsEntry;
		gui_image_data * scrollbar;
		gui_image_data * arrowDown;
		gui_image_data * arrowDownOver;
		gui_image_data * arrowUp;
		gui_image_data * arrowUpOver;

		gui_sound * btn_sound_hover;
		gui_sound * btnsound_click;
		gui_trigger * trig_a;
		gui_trigger * trig2;

		bool listChanged;
};

//!Display a list of files
class GuiFileBrowser : public gui_element
{
	public:
		GuiFileBrowser(int w, int h);
		~GuiFileBrowser();
		void reset_state();
		void SetFocus(int f);
		void Draw();
		void draw_tool_tip();
		void TriggerUpdate();
		void Update(gui_trigger * t);
		gui_button * fileList[FILE_PAGESIZE];
	protected:
		gui_text * fileListText[FILE_PAGESIZE];
		gui_image * fileListBg[FILE_PAGESIZE];
		gui_image * fileListFolder[FILE_PAGESIZE];

		gui_button * arrowUpBtn;
		gui_button * arrowDownBtn;
		gui_button * scrollbarBoxBtn;

		gui_image * bgFileSelectionImg;
		gui_image * scrollbarImg;
		gui_image * arrowDownImg;
		gui_image * arrowDownOverImg;
		gui_image * arrowUpImg;
		gui_image * arrowUpOverImg;
		gui_image * scrollbarBoxImg;
		gui_image * scrollbarBoxOverImg;

		gui_image_data * bgFileSelection;
		gui_image_data * bgFileSelectionEntry;
		gui_image_data * fileFolder;
		gui_image_data * scrollbar;
		gui_image_data * arrowDown;
		gui_image_data * arrowDownOver;
		gui_image_data * arrowUp;
		gui_image_data * arrowUpOver;
		gui_image_data * scrollbarBox;
		gui_image_data * scrollbarBoxOver;

		gui_sound * btn_sound_hover;
		gui_sound * btnsound_click;
		gui_trigger * trig_a;
		gui_trigger * trig2;
		gui_trigger * trigHeldA;

		int selectedItem;
		int numEntries;
		bool listChanged;
};

#endif
