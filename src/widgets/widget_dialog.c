#include "widget_dialog.h"

static void on_button_clicked(GtkButton *button, gpointer udata) {
  (void)button;
  gpointer *data = udata;
  DialogResponseCb cb = data[0];
  gpointer cb_data = data[1];
  int rid = GPOINTER_TO_INT(data[2]);
  GtkWindow *dialog = data[3];

  if (cb)
    cb(rid, cb_data);
  gtk_window_destroy(dialog);

  g_free(data);
}

void widget_dialog_new(GtkWindow *parent, const char *message,
                       const char **buttons, DialogResponseCb cb,
                       gpointer udata) {
  GtkWidget *dialog = gtk_window_new();
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
  gtk_window_set_title(GTK_WINDOW(dialog), "Message");
  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 300, -1);
  gtk_widget_add_css_class(dialog, "custom-dialog");

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  gtk_widget_set_margin_top(box, 20);
  gtk_widget_set_margin_bottom(box, 20);
  gtk_widget_set_margin_start(box, 20);
  gtk_widget_set_margin_end(box, 20);

  GtkWidget *label = gtk_label_new(message);
  gtk_label_set_wrap(GTK_LABEL(label), TRUE);
  gtk_label_set_xalign(GTK_LABEL(label), 0.5f);
  gtk_widget_add_css_class(label, "custom-dialog-label");

  gtk_box_append(GTK_BOX(box), label);

  GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_add_css_class(btn_box, "custom-dialog-buttons");
  gtk_box_append(GTK_BOX(box), btn_box);

  for (int i = 0; buttons[i] != NULL; i++) {
    GtkWidget *btn = gtk_button_new_with_label(buttons[i]);
    gtk_widget_add_css_class(btn, "custom-dialog-button");
    gtk_widget_set_hexpand(btn, TRUE);
    gtk_widget_set_halign(btn, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(btn, GTK_ALIGN_CENTER);

    gpointer *data = g_new(gpointer, 4);
    data[0] = (gpointer)cb;
    data[1] = udata;
    data[2] = GINT_TO_POINTER(i);
    data[3] = dialog;

    g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), data);
    gtk_box_append(GTK_BOX(btn_box), btn);
  }

  gtk_window_set_child(GTK_WINDOW(dialog), box);
  gtk_window_present(GTK_WINDOW(dialog));
}
