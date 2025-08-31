

#ifndef WIDGET_DIALOG_H
#define WIDGET_DIALOG_H

#include <gtk/gtk.h>

typedef void (*DialogResponseCb)(int responseid, gpointer udata);

void widget_dialog_new(GtkWindow *parent, const char *message,
                       const char **buttons, DialogResponseCb cb,
                       gpointer udata);

#endif // WIDGET_DIALOG_H
