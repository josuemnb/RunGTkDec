#include "XGui.h"
#include "XWindow.h"

#include <gtk/gtk.h>

XWidget *XWindowNew(void)
{
    XWidget *win;
    win=(XWidget*)malloc(sizeof(XWidget));
    if(win==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	win=AddWidget(win);
    win->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win->window),100,100);
	gtk_window_set_position(GTK_WINDOW(win->window),GTK_WIN_POS_NONE);
	g_signal_connect(win->window,"destroy", G_CALLBACK (gtk_main_quit), NULL);
	win->container=gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(win->window),win->container);
    gtk_window_get_position(GTK_WINDOW(win->window),&win->size.x,&win->size.y);
    win->type=WINDOW;
    win->parent=NULL;
    win->size.width=100;
    win->size.height=100;

    return (XWidget*)win;
}
void XWindowClick(GtkWidget *wid, gpointer data)
{
	XWidget *w=(XWidget*)data;
	MovePointer(w->event.Click);
}

void XWindowEvent(XWidget *w, char event)
{
    switch(event)
    {
    case CLICK:g_signal_connect(w->window,"clicked",G_CALLBACK(XWindowClick),w);break;
    case DOWN:g_signal_connect(w->window,"button-press-event",G_CALLBACK(XWindowClick),w);break;
    case UP:g_signal_connect(w->window,"button-release-event",G_CALLBACK(XWindowClick),w);break;
    case ENTER:g_signal_connect(w->window,"enter-notify-event",G_CALLBACK(XWindowClick),w);break;
    case LEAVE:g_signal_connect(w->window,"leave-notify-event",G_CALLBACK(XWindowClick),w);break;
    case MOVE:g_signal_connect(w->window,"motion-notify-event",G_CALLBACK(XWindowClick),w);break;
    case RESIZE:g_signal_connect(w->window,"configure-event",G_CALLBACK(XWindowClick),w);break;
    case SHOW:g_signal_connect(w->window,"map-event",G_CALLBACK(XWindowClick),w);break;
    case HIDE:g_signal_connect(w->window,"unmap-event",G_CALLBACK(XWindowClick),w);break;
    case PAINT:g_signal_connect(w->window,"expose-event",G_CALLBACK(XWindowClick),w);break;
    case FOCUSOUT:g_signal_connect(w->window,"focus-out-event",G_CALLBACK(XWindowClick),w);break;
    case FOCUSIN:g_signal_connect(w->window,"focus-in-event",G_CALLBACK(XWindowClick),w);break;
    case PRESS:g_signal_connect(w->window,"key-press-event",G_CALLBACK(XWindowClick),w);break;
    case RELEASE:g_signal_connect(w->window,"key-release-event",G_CALLBACK(XWindowClick),w);break;
    default:puts("Evento desconhecido");break;
    }
}
