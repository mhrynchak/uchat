#include "client.h"

void sig_hide_msg_editing(GtkButton *btn, GtkBuilder *builder) {
    GObject *box_editing = gtk_builder_get_object(builder, "box_editing_msg");
    GObject *btn_apply = gtk_builder_get_object(builder, "btn_edit_msg_apply");
    GObject *btn_send = gtk_builder_get_object(builder, "btn_send_msg");
    GObject *btn_stick = gtk_builder_get_object(builder, "btn_choose_sticker");

    gtk_widget_hide(GTK_WIDGET(box_editing));
    gtk_widget_hide(GTK_WIDGET(btn_apply));
    gtk_widget_show(GTK_WIDGET(btn_send));
    gtk_widget_show(GTK_WIDGET(btn_stick));
    clear_buffer_text("buffer_message", builder);
    (void)btn;
}
