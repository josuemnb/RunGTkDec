#ifndef XCHECKBOX_H
#define XCHECKBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "XGui.h"

//XWidget *XCheckBoxNew(XWidget *parent,char caption[128], int x, int y);
XWidget *XCheckBoxNew(XWidget *parent);
void XCheckBoxEvent(XWidget *w, char event);

#ifdef __cplusplus
}
#endif

#endif // XCHECKBOX_H
