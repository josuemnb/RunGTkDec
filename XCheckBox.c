#include "XCheckBox.h"
#include "XGui.h"

XWidget *XCheckBoxNew(XWidget *parent)
{
    XWidget *chk;
    chk=(XWidget*)malloc(sizeof(XWidget));
    if(chk==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	chk=AddWidget(chk);
    chk->window=gtk_check_button_new();
	gtk_widget_set_size_request(chk->window,50,30);
	gtk_fixed_put(GTK_FIXED(parent->container),chk->window,10,10);
	chk->type=CHECKBOX;
	chk->parent=parent;
    chk->size.x=10;
    chk->size.y=10;
    chk->size.width=50;
    chk->size.height=30;
    return (XWidget*)chk;
}
void XCheckBoxToogle( GtkWidget *wid, gpointer data)
{
	if(((XWidget*)data)->event.Click.pChar>-1)
		MovePointer(((XWidget*)data)->event.Click);
}

void XCheckBoxEvent(XWidget *w, char event)
{
    switch(event)
    {
	case DOWN:
	case PRESS:
    case CLICK:g_signal_connect(w->window,"toggled",G_CALLBACK(XCheckBoxToogle),w);break;
    default:puts("Evento desconhecido");break;
    }
}
