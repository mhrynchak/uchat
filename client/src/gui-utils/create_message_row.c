#include "client.h"

GtkWidget *msgcreate_box_main(GtkWidget *eventbox, gboolean is_own) {
    GtkWidget *box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    if (is_own)
        widget_set_class(box_main, "main_own_msg_box");
    else
        widget_set_class(box_main, "main_msg_box");
    gtk_container_add(GTK_CONTAINER(eventbox), GTK_WIDGET(box_main));
    return box_main;
}

GtkWidget *msgcreate_eventbox() {
    GtkWidget *eventbox = gtk_event_box_new();

    gtk_widget_set_can_focus(eventbox, false);
    return eventbox;
}

GtkWidget *create_reg_message_row(t_gmsg *gmsg, gboolean is_own, t_chat *chat) {
    GtkWidget *eventbox = msgcreate_eventbox();
    GtkWidget *box_main = msgcreate_box_main(eventbox, is_own);

    msgcreate_box_info(box_main, gmsg, is_own, chat);
    return eventbox;
}

GtkWidget *create_message_row(t_chat *chat, t_gmsg *gmsg) {
    gboolean is_own = !g_strcmp0(chat->login, gmsg->login);
    GtkWidget *eventbox = NULL;

    eventbox = create_reg_message_row(gmsg, is_own, chat);
    return eventbox;
}
