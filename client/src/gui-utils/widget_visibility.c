#include "client.h"


void widget_set_visibility(GtkWidget *widget, gboolean is_visible) {
    if (is_visible)
        gtk_widget_show(widget);
    else
        gtk_widget_hide(widget);    
}


void widget_set_visibility_by_name(GtkBuilder *builder, gchar *name, gboolean is_visible) {
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(builder, name));

    widget_set_visibility(widget, is_visible);
}


gboolean widget_is_visible(gchar *widget_name, GtkBuilder *builder) {
    GObject *widget = gtk_builder_get_object(builder, widget_name);

    return gtk_widget_is_visible(GTK_WIDGET(widget));
}

void widget_switch_visibility_by_name(GtkBuilder *builder, gchar *name) {
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(builder, name));

    sig_widget_switch_visibility(NULL, widget);
}
