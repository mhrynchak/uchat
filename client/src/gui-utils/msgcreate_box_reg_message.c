#include "client.h"

static void msgcreate_box_status(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own) {
    GtkWidget *box_status = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    if (is_own) {
        gtk_box_pack_end(GTK_BOX(box_info), box_status, FALSE, TRUE, 0);
    }
    else {
        gtk_box_pack_start(GTK_BOX(box_info), box_status, FALSE, TRUE, 0);
    }
    gtk_widget_set_valign(box_status, GTK_ALIGN_END);
    msgcreate_label_time(box_status, gmsg, is_own);
}

static void msgcreate_content(GtkWidget *box_main, GtkWidget *box_info, t_gmsg *gmsg, t_chat *chat) {
    if (gmsg->msg_type == DB_TEXT_MSG) {
        widget_set_class(box_info, "box_msg_info");
        msgcreate_label_text(box_info, gmsg, FALSE);
    }
    else if (gmsg->msg_type == DB_STICKER) {
        widget_set_class(box_info, "box_sticker_info");
        msgcreate_img_sticker(box_info, gmsg, FALSE);
    }
    else {
        widget_set_class(box_info, "box_file_info");
        msgcreate_file(box_info, gmsg, FALSE, chat);
    }
    msgcreate_label_login(box_main, gmsg);
}

static void msgcreate_own_content(GtkWidget *box_info,
                              t_gmsg *gmsg, t_chat *chat) {
    if (gmsg->msg_type == DB_TEXT_MSG) {
        widget_set_class(box_info, "box_own_msg_info");
        msgcreate_label_text(box_info, gmsg, TRUE);
    }
    else if (gmsg->msg_type == DB_STICKER) {
        widget_set_class(box_info, "box_own_sticker_info");
        msgcreate_img_sticker(box_info, gmsg, TRUE);
    }
    else {
        widget_set_class(box_info, "box_own_file_info");
        msgcreate_file(box_info, gmsg, TRUE, chat);
    }
}

void msgcreate_box_info(GtkWidget *box_main, t_gmsg *gmsg, gboolean is_own, t_chat *chat) {
    GtkWidget *box_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    if (is_own) {
        msgcreate_own_content(box_info, gmsg, chat);
    }
    else {
        msgcreate_content(box_main, box_info, gmsg, chat);
    }
    gtk_box_pack_end(GTK_BOX(box_main), box_info, FALSE, TRUE, 0);
    msgcreate_box_status(box_info, gmsg, is_own);
}
