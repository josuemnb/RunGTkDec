#include "XTextBox.h"
#include "XGui.h"

#include <string.h>

XWidget *XTextBoxNew(XWidget *parent)
{
    XWidget *txt;
    txt=(XWidget*)malloc(sizeof(XWidget));
    if(txt==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	txt=AddWidget(txt);
    txt->window=gtk_entry_new();
	gtk_widget_set_size_request(txt->window,50,30);
	gtk_fixed_put(GTK_FIXED(parent->container),txt->window,10,10);
	txt->type=TEXTBOX;
	txt->parent=parent;
    txt->size.x=10;
    txt->size.y=10;
    txt->size.width=50;
    txt->size.height=30;
    return (XWidget*)txt;
}

void XTextBoxAction( GtkWidget *wid, gpointer data)
{
	if(((XWidget*)data)->event.Click.pChar>-1)
		MovePointer(((XWidget*)data)->event.Click);
}

void XTextBoxChanged( GtkWidget *wid, gpointer data)
{
	if(((XWidget*)data)->event.Press.pChar>-1)
		MovePointer(((XWidget*)data)->event.Press);
}

void XTextBoxDeleting(GtkEntry *wid, GtkDeleteType type, gint count, gpointer data)
{
	if(((XWidget*)data)->event.Press.pChar>-1)
		MovePointer(((XWidget*)data)->event.Press);
}

void XTextBoxInserting (GtkEntry *wid, gchar *string, gpointer data)
{
	if(((XWidget*)data)->event.Press.pChar>-1)
		MovePointer(((XWidget*)data)->event.Press);
}

void XTextBoxEvent(XWidget *w, char event)
{
    switch(event)
    {
    case CLICK:g_signal_connect(w->window,"activate",G_CALLBACK(XTextBoxAction),w);break;
    case PRESS:g_signal_connect(w->window,"backspace",G_CALLBACK(XTextBoxChanged),w);
				g_signal_connect(w->window,"copy-clipboard",G_CALLBACK(XTextBoxChanged),w);
				g_signal_connect(w->window,"cut-clipboard",G_CALLBACK(XTextBoxChanged),w);
				g_signal_connect(w->window,"paste-clipboard",G_CALLBACK(XTextBoxChanged),w);
				g_signal_connect(w->window,"insert-at-cursor",G_CALLBACK(XTextBoxInserting),w);
				g_signal_connect(w->window,"delete-from-cursor",G_CALLBACK(XTextBoxDeleting),w);break;
    default:puts("Evento desconhecido");break;
    }
}

