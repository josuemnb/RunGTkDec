#ifndef XGUI
#define XGUI

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

typedef struct
{
	short pChar;
	short pInt;
}POINTER;

enum TYPES { WINDOW=1, BUTTON, LABEL, TEXTBOX, CHECKBOX, IMAGE};

enum EVENTS { CLICK=1, DOWN, UP, ENTER, LEAVE, MOVE, RESIZE, SHOW, HIDE,
			PAINT, FOCUSOUT, FOCUSIN, PRESS, RELEASE};

typedef struct
{
	POINTER Press;
	POINTER Release;
	POINTER Down;
	POINTER Up;
	POINTER Click;
	POINTER Enter;
	POINTER Leave;
	POINTER Move;
	POINTER Resize;
	POINTER Paint;
	POINTER Minimize;
	POINTER Hide;
	POINTER Show;
	POINTER FocusIn;
	POINTER FocusOut;
	POINTER Create;
	POINTER Destroy;
}EVENT;

typedef struct
{
	gint x,y;
	gint width,height;
}RECT;

typedef union
{
	char Char;
	unsigned int Int;
	long long Long;
	float Float;
	double Double;
	char *String;
	void *Pointer;
}TUPLE;

typedef struct XWidget
{
    TUPLE data;
	RECT size;
	char visible;
	char type;
	GtkWidget *window;
	GtkWidget *parent;
	GtkWidget *container;
    EVENT event;
} XWidget;

typedef struct WIDGET
{
	XWidget widget;
	struct WIDGET *next;
}WIDGET;

#include "XWindow.h"
#include "XLabel.h"
#include "XImage.h"
#include "XButton.h"
#include "XTextBox.h"
#include "XCheckBox.h"
#include "XColors.h"

void XConnect(XWidget *w, char event);
void XWidgetDelete(XWidget *win);
int XBegin();
void XEnd();
void XLoop();
int XWidgetShow(XWidget*);
int XWidgetHide(XWidget*);
void XWidgetSetWidth(XWidget *win,int width);
int XWidgetGetWidth(XWidget *win);
void XWidgetSetHeight(XWidget *win,int);
int XWidgetGetHeight(XWidget *win);
int XWidgetGetLeft(XWidget*);
void XWidgetSetLeft(XWidget*,int);
int XWidgetGetTop(XWidget*);
void XWidgetSetTop(XWidget*,int);
char *XWidgetGetCaption(XWidget *win);
void XWidgetSetCaption(XWidget *win,char *caption);
void XWidgetSetFocus(XWidget *w);
void XWidgetLoseFocus(XWidget *w);
int LoadFont(XWidget *win,char *name);

#ifdef __cplusplus
}
#endif

#endif // XGUI_H_INCLUDED
