#include "XImage.h"
#include "XGui.h"

#include <gtk/gtk.h>
#include <string.h>

GdkPixbuf *pixbuf;

XWidget *XImageNew(XWidget *parent)
{
    XWidget *img;
    img=(XWidget*)malloc(sizeof(XWidget));
    if(img==NULL)
	{
		puts("ERRO ao criar widget");
		exit(-2);
	}
	img=AddWidget(img);
    img->window=gtk_image_new();
	gtk_widget_set_size_request(img->window,100,100);
	gtk_fixed_put(GTK_FIXED(parent->container),img->window,10,10);
	img->type=IMAGE;
	img->parent=parent;
    img->size.x=10;
    img->size.y=10;
    img->size.width=100;
    img->size.height=100;
    return (XWidget*)img;
}

void XImageSetWidth(XWidget *wid,int width)
{
	XWidgetSetWidth(wid,width);
//	pixbuf=gdk_pixbuf_new_from_file(wid->data.String,NULL);
//	pixbuf=gdk_pixbuf_scale_simple(pixbuf,wid->size.width,wid->size.height,GDK_INTERP_BILINEAR);
//	gtk_image_set_from_pixbuf(wid->window,pixbuf);
}

void XImageSetHeight(XWidget *wid, int height)
{
	XWidgetSetHeight(wid,height);
//	pixbuf=gdk_pixbuf_new_from_file(wid->data.String,NULL);
//	pixbuf=gdk_pixbuf_scale_simple(pixbuf,wid->size.width,wid->size.height,GDK_INTERP_BILINEAR);
//	gtk_image_set_from_pixbuf(wid->window,pixbuf);
}

void XImageLoad(XWidget *wid, char *fname)
{
	pixbuf=gdk_pixbuf_new_from_file(fname,NULL);
	pixbuf=gdk_pixbuf_scale_simple(pixbuf,wid->size.width,wid->size.height,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(wid->window,pixbuf);
	wid->data.String=malloc(strlen(fname));
	strcpy(wid->data.String,fname);
}

