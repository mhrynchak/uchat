#include "client.h"

GtkWidget *mx_create_message_row(t_chat *chat, t_gmsg *gmsg) {
    gboolean is_own = !g_strcmp0(chat->login, gmsg->login);
    GtkWidget *eventbox = NULL;

    eventbox = mx_create_reg_message_row(gmsg, is_own, chat);
    return eventbox;
}

GtkWidget *mx_create_reg_message_row(t_gmsg *gmsg,
                                     gboolean is_own, t_chat *chat) {
    GtkWidget *eventbox = mx_msgcreate_eventbox();
    GtkWidget *box_main = mx_msgcreate_box_main(eventbox, is_own);

    mx_msgcreate_box_info(box_main, gmsg, is_own, chat);
    return eventbox;
}
