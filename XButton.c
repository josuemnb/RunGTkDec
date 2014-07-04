#include "XGui.h"
#include "XButton.h"
#include "Interpreter.h"

XWidget *XButtonNew(XWidget *parent)
{
    XWidget *btn;
    btn=(XWidget*)malloc(sizeof(XWidget));
    if(btn==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	btn=AddWidget(btn);
    btn->window=gtk_button_new();
	gtk_widget_set_size_request(btn->window,50,30);
	gtk_fixed_put(GTK_FIXED(parent->container),btn->window,10,10);
	btn->type=BUTTON;
	btn->parent=parent;
    btn->size.x=10;
    btn->size.y=10;
    btn->size.width=50;
    btn->size.height=30;
    return (XWidget*)btn;
}

gboolean XButtonClick( GtkWidget *wid, gpointer data)
{
	if(((XWidget*)data)->event.Click.pChar>-1)
		MovePointer(((XWidget*)data)->event.Click);
}

gboolean XButtonPress( GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Press.pChar>-1)
		MovePointer(((XWidget*)data)->event.Press);
}

gboolean XButtonRelease( GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Release.pChar>-1)
		MovePointer(((XWidget*)data)->event.Release);
}

gboolean XButtonEnter( GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Enter.pChar>-1)
		MovePointer(((XWidget*)data)->event.Enter);
}

gboolean XButtonLeave( GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Leave.pChar>-1)
		MovePointer(((XWidget*)data)->event.Leave);
}

gboolean XButtonDown(GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Down.pChar>-1)
		MovePointer(((XWidget*)data)->event.Down);
}

gboolean XButtonUp( GtkWidget *wid, GdkEventButton *event, gpointer data)
{
	if(((XWidget*)data)->event.Up.pChar>-1)
		MovePointer(((XWidget*)data)->event.Up);
}

void XButtonEvent(XWidget *w, char event)
{
    switch(event)
    {
    case CLICK:g_signal_connect(w->window,"clicked",G_CALLBACK(XButtonClick),w);break;
    case DOWN:g_signal_connect(w->window,"button-press-event",G_CALLBACK(XButtonDown),w);break;
    case UP:g_signal_connect(w->window,"button-release-event",G_CALLBACK(XButtonUp),w);break;
    case ENTER:g_signal_connect(w->window,"enter-notify-event",G_CALLBACK(XButtonEnter),w);break;
    case LEAVE:g_signal_connect(w->window,"leave-notify-event",G_CALLBACK(XButtonLeave),w);break;
    case PRESS:g_signal_connect(w->window,"key-press-event",G_CALLBACK(XButtonPress),w);break;
    case RELEASE:g_signal_connect(w->window,"key-release-event",G_CALLBACK(XButtonRelease),w);break;
    default:puts("Evento desconhecido");break;
    }
}
