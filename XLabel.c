#include "XGui.h"
#include "XLabel.h"


XWidget *XLabelNew(XWidget *parent)
{
    XWidget *lbl;
    lbl=(XWidget*)malloc(sizeof(XWidget));
    if(lbl==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	lbl=AddWidget(lbl);
    lbl->window=gtk_label_new(NULL);
	gtk_widget_set_size_request(lbl->window,50,30);
	gtk_fixed_put(GTK_FIXED(parent->container),lbl->window,10,10);
	lbl->parent=parent;
	lbl->type=LABEL;
    lbl->size.x=10;
    lbl->size.y=10;
    lbl->size.width=50;
    lbl->size.height=30;
    return (XWidget*)lbl;
}
