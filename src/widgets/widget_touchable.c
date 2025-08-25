#include "widget_touchable.h"
#include "../state/state.h"
#include "widgets_config.h"

GtkWidget *touchable(const char *label, const char *icon, const char *cssClass,
                     WidgetPositioning *positioning) {

  if (label == NULL && icon == NULL) {
    perror("touchable: Either label or icon must be provided.");
    return NULL;
  }

  GtkEventController *ctrl = gtk_event_controller_motion_new();
  GtkWidget *button = gtk_button_new();
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  GtkWidget *labelWidget = NULL;
  GtkWidget *iconwidget = NULL;

  if (icon != NULL) {
    iconwidget = gtk_image_new_from_icon_name(icon);
  }

  if (label != NULL) {
    labelWidget = gtk_label_new(label);
  }

  if (iconwidget != NULL) {
    gtk_box_append(GTK_BOX(hbox), iconwidget);
  }
  if (labelWidget != NULL) {
    gtk_box_append(GTK_BOX(hbox), labelWidget);
  }

  gtk_button_set_child(GTK_BUTTON(button), hbox);
  gtk_widget_add_controller(button, ctrl);

  g_signal_connect(ctrl, "enter", G_CALLBACK(cursor_pointer), button);
  g_signal_connect(ctrl, "leave", G_CALLBACK(cursor_default), button);

  set_configs(button, cssClass, positioning);

  return button;
}