#include "XGui.h"
//#include "XButton.h"
//#include "XWindow.h"
//#include "XLabel.h"
//#include "XTextBox.h"
//#include "XCheckBox.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

XWidget *lastXWidget;

WIDGET *WIDGET_T;

XWidget *AddWidget(XWidget *w)
{
    WIDGET *nWid;
    nWid=(WIDGET*)malloc(sizeof(WIDGET));
    if(nWid==NULL)
    {
        puts("Erro ao alocar um novo widget. Memória insuficient");
        exit(-3);
    }
    nWid->widget=*w;
    nWid->widget.visible=0;
    nWid->widget.event.Down.pChar=-1;
    nWid->widget.event.Up.pChar=-1;
    nWid->widget.event.Click.pChar=-1;
    nWid->widget.event.Enter.pChar=-1;
    nWid->widget.event.Leave.pChar=-1;
    nWid->widget.event.Move.pChar=-1;
    nWid->widget.event.Press.pChar=-1;
    nWid->widget.event.Release.pChar=-1;
    nWid->widget.event.Create.pChar=-1;
    nWid->widget.event.Destroy.pChar=-1;
    nWid->widget.event.FocusIn.pChar=-1;
    nWid->widget.event.Hide.pChar=-1;
    nWid->widget.event.FocusOut.pChar=-1;
    nWid->widget.event.Paint.pChar=-1;
    nWid->widget.event.Resize.pChar=-1;
    nWid->widget.event.Minimize.pChar=-1;
    nWid->widget.event.Show.pChar=-1;
    nWid->next=NULL;
    if(WIDGET_T==NULL)
        WIDGET_T=nWid;
    else
    {
        nWid->next=WIDGET_T;
        WIDGET_T=nWid;
    }
    return &nWid->widget;
}

void XConnect(XWidget *w, char event)
{

}

int XWidgetShow(XWidget *win)
{
    if(win->visible==1)
        return 0;
	if(win->type==WINDOW)
		gtk_widget_show_all(win->window);
    else
		gtk_widget_show(win->window);
    win->visible=1;
    return 1;
}

int XWidgetHide(XWidget *win)

{
    if(win->visible==0)
        return 0;
	gtk_widget_hide(win->window);
    win->visible=0;
    return 1;
}

void XWidgetDelete(XWidget *win)
{
//    XUnmapWindow(display,win->window);
//    XDestroyWindow(display,win->window);
}

int XWidgetGetWidth(XWidget *win)
{
    return win->size.width;
}

void XWidgetSetWidth(XWidget *win,int width)
{
	if(win->size.width==width)
        return;
    win->size.width=width;
	if(win->type==WINDOW)
		gtk_window_resize(GTK_WINDOW(win->window),width,win->size.height);
	else if(win->type=IMAGE)
		XImageSetWidth(win,width);
	else
		gtk_widget_set_usize(win->window,width,win->size.height);
}

int XWidgetGetHeight(XWidget *win)
{
    return win->size.height;
}

void XWidgetSetHeight(XWidget *win,int height)
{
    if(win->size.height==height)
        return;
    win->size.height=height;
    if(win->type==WINDOW)
		gtk_window_resize(GTK_WINDOW(win->window),win->size.width,height);
	else if(win->type=IMAGE)
		XImageSetHeight(win,height);
	else
		gtk_widget_set_usize(win->window,win->size.width,height);
}

void XWidgetSetLeft(XWidget *win,int left)
{
    if(win->size.x==left)
        return;
    win->size.x=left;
    if(win->type==WINDOW)
		gtk_window_move(win->window,left,win->size.y);
	else
		gtk_widget_set_uposition(GTK_WIDGET(win->window), left, win->size.y);
}

int XWidgetGetLeft(XWidget *win)
{
    return win->size.x;
}

void XWidgetSetTop(XWidget *win,int top)
{
    if(win->size.y==top)
        return;
    win->size.y=top;
    if(win->type==WINDOW)
		gtk_window_move(win->window,win->size.x,top);
	else
		gtk_widget_set_uposition(GTK_WIDGET(win->window), win->size.x,top);
}

int XWidgetGetTop(XWidget *win)
{
    return win->size.y;
}

void XWidgetSetCaption(XWidget *win,char *caption)
{
	switch(win->type)
	{
	case WINDOW:gtk_window_set_title(GTK_WINDOW(win->window),caption);break;
	case TEXTBOX:gtk_entry_set_text(GTK_ENTRY(win->window),caption);break;
	case CHECKBOX:gtk_button_set_label(GTK_BUTTON(win->window),caption);break;
	case BUTTON:gtk_button_set_label(GTK_BUTTON(win->window),caption);break;
	case LABEL:gtk_label_set_text(GTK_LABEL(win->window),caption);break;
	}
}

