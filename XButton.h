#ifndef XBUTTON_H
#define XBUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

XWidget *XButtonNew(XWidget* parent);
void XButtonEvent(XWidget *w, char event);

#ifdef __cplusplus
}
#endif

#endif // XBUTTON_H_INCLUDED
