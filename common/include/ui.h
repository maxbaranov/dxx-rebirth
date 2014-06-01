/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Header file for user interface
 *
 */

#ifndef _UI_H
#define _UI_H

#include "dxxsconf.h"
#include "event.h"
#include "fmtcheck.h"

#ifdef __cplusplus
#include "varutil.h"

struct grs_bitmap;
struct grs_canvas;
struct grs_font;
struct window;

struct UI_KEYPAD {
	char	description[100];
	char 	* buttontext[17];
	int	numkeys;
	short keycode[100];
	int 	function_number[100];
};

struct UI_EVENT
{
	unsigned int frame;
	int type;
	int data;
};

#define BASE_GADGET             \
	short           kind;       \
	struct UI_GADGET  * prev;     \
	struct UI_GADGET  * next;     \
	struct UI_GADGET  * when_tab;  \
	struct UI_GADGET  * when_btab; \
	struct UI_GADGET  * when_up;    \
	struct UI_GADGET  * when_down;   \
	struct UI_GADGET  * when_left;   \
	struct UI_GADGET  * when_right;  \
	struct UI_GADGET  * parent;    \
	int             status;     \
	int             oldstatus;  \
	grs_canvas *    canvas;     \
	int             hotkey;     \
	short           x1,y1,x2,y2;


struct UI_GADGET
{
	BASE_GADGET
	unsigned char rsvd[256];
};

struct UI_GADGET_KEYTRAP {
	BASE_GADGET
	int         trap_key;
	int      (*user_function)(void);
};

struct UI_GADGET_USERBOX {
	BASE_GADGET
	short           width, height;
	short           b1_held_down;
	short           b1_clicked;
	short           b1_double_clicked;
	short           b1_dragging;
	short           b1_drag_x1, b1_drag_y1;
	short           b1_drag_x2, b1_drag_y2;
	short           b1_done_dragging;
	int             keypress;
	short           mouse_onme;
	short           mouse_x, mouse_y;
	grs_bitmap *    bitmap;
};

struct UI_GADGET_BUTTON {
	BASE_GADGET
	short           width, height;
	char            * text;
	short           position;
	short           oldposition;
	short           pressed;
	int          	 (*user_function)(void);
	int          	 (*user_function1)(void);
	int				 hotkey1;
	int				 dim_if_no_function;
};

struct UI_GADGET_INPUTBOX {
	BASE_GADGET
	short           width, height;
	char            * text;
	short           length;
	short           slength;
	short           position;
	short           oldposition;
	short           pressed;
	short           first_time;
};

struct UI_GADGET_RADIO {
	BASE_GADGET
	short           width, height;
	char            * text;
	short           position;
	short           oldposition;
	short           pressed;
	short           group;
	short           flag;
};

struct UI_GADGET_ICON {
	BASE_GADGET
	char 				 *text;
	short 		    width, height;
	sbyte           flag;
	sbyte           pressed;
	sbyte           position;
	sbyte           oldposition;
	int             trap_key;
	int          	(*user_function)(void);
};

struct UI_GADGET_CHECKBOX {
	BASE_GADGET
	short           width, height;
   char            * text;
	short           position;
	short           oldposition;
	short           pressed;
	short           group;
	short           flag;
};

struct UI_GADGET_SCROLLBAR {
	BASE_GADGET
	short           horz;
	short           width, height;
	int             start;
	int             stop;
	int             position;
	int             window_size;
	int             fake_length;
	int             fake_position;
	int             fake_size;
	UI_GADGET_BUTTON * up_button;
	UI_GADGET_BUTTON * down_button;
	fix64           last_scrolled;
	short           drag_x, drag_y;
	int             drag_starting;
	int             dragging;
	int             moved;
};

struct UI_GADGET_LISTBOX {
	BASE_GADGET
	short           width, height;
	char            **list;
	int             num_items;
	int             num_items_displayed;
	int             first_item;
	int             old_first_item;
	int             current_item;
	int             selected_item;
	int             old_current_item;
	fix64           last_scrolled;
	int             dragging;
	int             textheight;
	UI_GADGET_SCROLLBAR * scrollbar;
	int             moved;
};

enum dialog_flags
{
	DF_BORDER  = 1,
	DF_FILLED = 2,
	DF_SAVE_BG = 4,
	DF_DIALOG = (4+2+1),
	DF_MODAL = 8		// modal = accept all user input exclusively
};

struct UI_DIALOG
{
	struct window	*wind;
	int				(*callback)(struct UI_DIALOG *, struct d_event *, void *);
	UI_GADGET *     gadget;
	UI_GADGET *     keyboard_focus_gadget;
	void			*userdata;
	short           x, y;
	short           width, height;
	short           text_x, text_y;
	enum dialog_flags flags;
};

#define B1_JUST_PRESSED     (event->type == EVENT_MOUSE_BUTTON_DOWN && event_mouse_get_button(event) == 0)
#define B1_JUST_RELEASED    (event->type == EVENT_MOUSE_BUTTON_UP && event_mouse_get_button(event) == 0)
#define B1_DOUBLE_CLICKED   (event->type == EVENT_MOUSE_DOUBLE_CLICKED && event_mouse_get_button(event) == 0)

extern grs_font * ui_small_font;

extern unsigned char CBLACK,CGREY,CWHITE,CBRIGHT,CRED;
extern UI_GADGET * selected_gadget;

extern void Hline(short x1, short x2, short y );
extern void Vline(short y1, short y2, short x );
extern void ui_string_centered( short x, short y, const char * s );
extern void ui_draw_box_out( short x1, short y1, short x2, short y2 );
extern void ui_draw_box_in( short x1, short y1, short x2, short y2 );
extern void ui_draw_line_in( short x1, short y1, short x2, short y2 );
extern void ui_draw_frame( short x1, short y1, short x2, short y2 );
extern void ui_draw_shad( short x1, short y1, short x2, short y2, short c1, short c2 );

void ui_init();
void ui_close();

typedef cstring_tie<10> ui_messagebox_tie;
int ui_messagebox( short xc, short yc, const char * text, const ui_messagebox_tie &Button );
#define ui_messagebox(X,Y,N,T,...)	((ui_messagebox)((X),(Y),(T), ui_messagebox_tie(__VA_ARGS__)))

template <typename T>
class ui_subfunction_t
{
public:
	typedef int (*type)(UI_DIALOG *, d_event *, T *);
};

class unused_ui_userdata_t;
static unused_ui_userdata_t *const unused_ui_userdata = NULL;

template <typename T>
UI_DIALOG * ui_create_dialog( short x, short y, short w, short h, enum dialog_flags flags, typename ui_subfunction_t<T>::type callback, T *userdata )
{
	return ui_create_dialog(x, y, w, h, flags, (ui_subfunction_t<void>::type) callback, (void *)userdata);
}

template <>
UI_DIALOG * ui_create_dialog( short x, short y, short w, short h, enum dialog_flags flags, typename ui_subfunction_t<void>::type callback, void *userdata );

extern struct window *ui_dialog_get_window(UI_DIALOG *dlg);
extern void ui_dialog_set_current_canvas(UI_DIALOG *dlg);
extern void ui_close_dialog( UI_DIALOG * dlg );

#define GADGET_PRESSED(g) ((event->type == EVENT_UI_GADGET_PRESSED) && (ui_event_get_gadget(event) == (UI_GADGET *)g))

extern UI_GADGET * ui_gadget_add( UI_DIALOG * dlg, short kind, short x1, short y1, short x2, short y2 );
extern UI_GADGET_BUTTON * ui_add_gadget_button( UI_DIALOG * dlg, short x, short y, short w, short h, const char * text, int (*function_to_call)(void) );
extern void ui_gadget_delete_all( UI_DIALOG * dlg );
extern int ui_gadget_send_event(UI_DIALOG *dlg, enum event_type type, UI_GADGET *gadget);
extern UI_GADGET *ui_event_get_gadget(struct d_event *event);
extern int ui_dialog_do_gadgets( UI_DIALOG * dlg, struct d_event *event );
extern void ui_draw_button( UI_DIALOG *dlg, UI_GADGET_BUTTON * button );

extern int ui_mouse_on_gadget( UI_GADGET * gadget );

extern int ui_button_do( UI_DIALOG *dlg, UI_GADGET_BUTTON * button, struct d_event *event );

extern int ui_listbox_do( UI_DIALOG *dlg, UI_GADGET_LISTBOX * listbox, struct d_event *event );
extern void ui_draw_listbox( UI_DIALOG *dlg, UI_GADGET_LISTBOX * listbox );
extern UI_GADGET_LISTBOX *ui_add_gadget_listbox(UI_DIALOG *dlg, short x, short y, short w, short h, short numitems, char **list);

extern void ui_mega_process();

extern void ui_get_button_size( const char * text, int * width, int * height );

extern UI_GADGET_SCROLLBAR * ui_add_gadget_scrollbar( UI_DIALOG * dlg, short x, short y, short w, short h, int start, int stop, int position, int window_size  );
extern int ui_scrollbar_do( UI_DIALOG *dlg, UI_GADGET_SCROLLBAR * scrollbar, struct d_event *event );
extern void ui_draw_scrollbar( UI_DIALOG *dlg, UI_GADGET_SCROLLBAR * scrollbar );


void ui_dputs_at( UI_DIALOG * dlg, short x, short y, const char * str );
extern void ui_dprintf_at( UI_DIALOG * dlg, short x, short y, const char * format, ... ) __attribute_format_printf(4, 5);
#define ui_dprintf_at(A1,A2,A3,F,...)	dxx_call_printf_checked(ui_dprintf_at,ui_dputs_at,(A1,A2,A3),(F),##__VA_ARGS__)

extern void ui_draw_radio( UI_DIALOG *dlg, UI_GADGET_RADIO * radio );
extern UI_GADGET_RADIO * ui_add_gadget_radio( UI_DIALOG * dlg, short x, short y, short w, short h, short group, const char * text );
extern int ui_radio_do( UI_DIALOG *dlg, UI_GADGET_RADIO * radio, struct d_event *event );
extern void ui_radio_set_value(UI_GADGET_RADIO *radio, int value);

extern void ui_draw_checkbox( UI_DIALOG *dlg, UI_GADGET_CHECKBOX * checkbox );
extern UI_GADGET_CHECKBOX * ui_add_gadget_checkbox( UI_DIALOG * dlg, short x, short y, short w, short h, short group, const char * text );
extern int ui_checkbox_do( UI_DIALOG *dlg, UI_GADGET_CHECKBOX * checkbox, struct d_event *event );
extern void ui_checkbox_check(UI_GADGET_CHECKBOX * checkbox, int check);

extern UI_GADGET * ui_gadget_get_prev( UI_GADGET * gadget );
extern UI_GADGET * ui_gadget_get_next( UI_GADGET * gadget );
extern void ui_gadget_calc_keys( UI_DIALOG * dlg);

extern void ui_listbox_change(UI_DIALOG *dlg, UI_GADGET_LISTBOX *listbox, short numitems, char **list);


extern void ui_draw_inputbox( UI_DIALOG *dlg, UI_GADGET_INPUTBOX * inputbox );
extern UI_GADGET_INPUTBOX * ui_add_gadget_inputbox( UI_DIALOG * dlg, short x, short y, short w, short h, const char * text );
extern int ui_inputbox_do( UI_DIALOG *dlg, UI_GADGET_INPUTBOX * inputbox, struct d_event *event );
extern void ui_inputbox_set_text(UI_GADGET_INPUTBOX *inputbox, const char *text);


extern int ui_userbox_do( UI_DIALOG *dlg, UI_GADGET_USERBOX * userbox, struct d_event *event );
extern UI_GADGET_USERBOX * ui_add_gadget_userbox( UI_DIALOG * dlg, short x, short y, short w, short h );
extern void ui_draw_userbox( UI_DIALOG *dlg, UI_GADGET_USERBOX * userbox );


int MenuX( int x, int y, int NumButtons, const char *const text[] );

char **file_getdirlist(int *NumFiles, const char *dir);
char **file_getfilelist(int *NumDirs, const char *filespec, const char *dir);
int ui_get_filename( char * filename, const char * Filespec, const char * message  );


void * ui_malloc( int size );
void ui_free( void * buffer );

UI_GADGET_KEYTRAP * ui_add_gadget_keytrap( UI_DIALOG * dlg, int key_to_trap, int (*function_to_call)(void)  );
int ui_keytrap_do( UI_GADGET_KEYTRAP * keytrap, struct d_event *event );

#define UI_RECORD_MOUSE     1
#define UI_RECORD_KEYS      2
#define UI_STATUS_NORMAL    0
#define UI_STATUS_RECORDING 1
#define UI_STATUS_PLAYING   2
#define UI_STATUS_FASTPLAY  3

int ui_record_events( int NumberOfEvents, UI_EVENT * buffer, int Flags );
int ui_play_events_realtime( int NumberOfEvents, UI_EVENT * buffer );
int ui_play_events_fast( int NumberOfEvents, UI_EVENT * buffer );
int ui_recorder_status();
void ui_set_playback_speed( int speed );

extern unsigned int ui_number_of_events;
extern unsigned int ui_event_counter;


int ui_get_file( char * filename, const char * Filespec  );

void ui_draw_icon( UI_GADGET_ICON * icon );
int ui_icon_do( UI_DIALOG *dlg, UI_GADGET_ICON * icon, struct d_event *event );
UI_GADGET_ICON * ui_add_gadget_icon( UI_DIALOG * dlg, const char * text, short x, short y, short w, short h, int k,int (*f)(void) );

int DecodeKeyText( const char * text );
void GetKeyDescription( char * text, int keypress );

extern void menubar_init(const char * filename );
extern void menubar_close();
extern void menubar_hide();
extern void menubar_show();

void ui_pad_init();
void ui_pad_close();
void ui_pad_activate( UI_DIALOG * dlg, int x, int y );
void ui_pad_deactivate();
void ui_pad_goto(int n);
void ui_pad_goto_next();
void ui_pad_goto_prev();
void ui_pad_read( int n, const char * filename );
int ui_pad_get_current();
void ui_pad_draw(UI_DIALOG *dlg, int x, int y);

void ui_barbox_open( const char * text, int length );
void ui_barbox_update( int position );
void ui_barbox_close();

extern int ui_button_any_drawn;

#endif

#endif

