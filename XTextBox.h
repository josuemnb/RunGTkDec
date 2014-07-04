#ifndef XTEXTBOX_H
#define XTEXTBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "XGui.h"

XWidget *XTextBoxNew(XWidget *parent);
void XTextBoxEvent(XWidget *w, char event);

#ifdef __cplusplus
}
#endif

#endif // XEDITTEXT_H
