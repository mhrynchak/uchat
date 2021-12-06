#include "client.h"

void sig_widget_switch_visibility(GtkWidget *usr_ctrl, GtkWidget *widget) {
    widget_set_visibility(widget, !gtk_widget_is_visible(widget));
    (void)usr_ctrl;
}
