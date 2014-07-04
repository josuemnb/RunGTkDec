#ifndef XLABEL_H
#define XLABEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "XGui.h"

void XLabelEvent(XWidget *w, char event);
XWidget *XLabelNew(XWidget *parent);
#ifdef __cplusplus
}
#endif

#endif // XLABEL_H
