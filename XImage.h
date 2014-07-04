#ifndef XIMAGE_H
#define XIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "XGui.h"

void XImageLoad(XWidget *wid, char *fname);
XWidget *XImageNew(XWidget *parent);
void XImageSetWidth(XWidget *wid,int width);
void XImageSetHeight(XWidget *wid,int height);

#ifdef __cplusplus
}
#endif

#endif // XIMAGE_H
