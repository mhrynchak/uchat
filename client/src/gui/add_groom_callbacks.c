#include "client.h"

void reset_addroom(GtkButton *btn, GtkBuilder *builder) {
    clear_buffer_text("buffer_roomname", builder);
    (void)btn;
}

static void set_main_info_current_room_sett(GtkBuilder *builder,
                                            t_groom *groom) {
    GObject *name = gtk_builder_get_object(builder, "label_prefs_roomname");
    GObject *go_down = gtk_builder_get_object(builder, "btn_go_down");
    gtk_widget_hide(GTK_WIDGET(go_down));
    gtk_label_set_text(GTK_LABEL(name), groom->room_name);
}

void set_current_room_sett(GtkBuilder *builder) {
    t_groom *groom = get_selected_groom(builder, "listbox_rooms");
    GObject *customer = gtk_builder_get_object(builder, "label_prefs_customer");
    GObject *desc = gtk_builder_get_object(builder, "buffer_room_desc");
    GObject *header_name = gtk_builder_get_object(builder,
                                                  "label_header_roomname");
    GObject *message_field = gtk_builder_get_object(builder, "msg_entry");

    if (groom) {
        gtk_widget_grab_focus(GTK_WIDGET(message_field));
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(desc), groom->desc, -1);
        gtk_label_set_text(GTK_LABEL(customer), groom->customer);
        gtk_label_set_text(GTK_LABEL(header_name), groom->room_name);
        set_room_members(builder, groom);  
        set_main_info_current_room_sett(builder, groom);
        reset_select_count(groom);
    }
}

void select_room(GtkWidget *event_box, GdkEventButton *event, gpointer *user_data) {
    t_signal_data *data = g_object_get_data(G_OBJECT(event_box), "sigdata");
    GObject *btn_room_sett = gtk_builder_get_object(data->chat->builder,
                                                    "btn_show_room_sett");
    if (is_same_groom(data->groom, data->chat->builder)) {
        reset_room_message(data->groom, data->chat->builder);
        gtk_stack_set_visible_child(data->groom->stack_msg, GTK_WIDGET(data->groom->page));
        gtk_list_box_select_row(data->groom->box_rooms, data->groom->row_room);
        set_current_room_sett(data->chat->builder);
        set_room_widgets_visibility(data->chat->builder, TRUE);
        widget_remove_class(GTK_WIDGET(data->groom->label_name),"has-messages");
        if (!g_strcmp0(data->chat->login, data->groom->customer)) {
            widget_set_visibility(GTK_WIDGET(btn_room_sett), TRUE);
        }
        else {
            widget_set_visibility(GTK_WIDGET(btn_room_sett), FALSE);   //del after set FALSE here
        }
    }
    (void)event;
    (void)user_data;
}

void show_join_to_room(GtkWidget *event_box, GdkEventButton *event,
                          gpointer *user_data) {
    t_signal_data *data = g_object_get_data(G_OBJECT(event_box), "sigdata");
    t_groom *lgroom = get_selected_groom(data->chat->builder, "listbox_rooms");
    GObject *label_name = gtk_builder_get_object(data->chat->builder,
                                                 "label_join_roomname");

    gtk_list_box_select_row(data->groom->box_rooms, data->groom->row_room);
    reset_room_message(lgroom, data->chat->builder);
    widget_switch_visibility_by_name(data->chat->builder,
                                        "dialog_join_to_room");
    gtk_label_set_text(GTK_LABEL(label_name), data->groom->room_name);
    (void)event;
    (void)user_data;
}
