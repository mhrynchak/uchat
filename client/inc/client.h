#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <ctype.h>
#include <curses.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libintl.h>
#include <glib.h>
#include <curses.h>
#include "sqlite3.h"
#include "cJSON.h"
#include "vmchat.h"
#include "structs.h"
#include "defines.h"

///**********************|client lib|**********************///
void valid_argv(int argc, char **argv);
void init_chat(t_chat *chat, GSocketClient *client, GSocketConnection *connection, gint argc, char **argv);
void receiver(GObject *source_object, GAsyncResult *res, gpointer user_data);
gssize sender(GDataOutputStream *out, gchar *dtp);

///**********************|api utils|**********************///
bool match_search(char *str, char *regex);
bool match_nsearch(char *str, char *regex, size_t size);
bool is_valid_auth_data(char *login, char *pass, GtkLabel *label);
void choose_auth(GtkButton *btn, t_chat *chat);
gchar *encrypt_str(gchar *str);


///**********************|json objects|**********************///
void json_manager(char *request, t_chat *chat);
///---requests
void sign_up_request(t_chat *chat);
void sign_in_request(t_chat *chat);
void add_room_request(t_chat *chat);

///---responses
void sign_up_response(cJSON *j_response, t_chat *chat);
void sign_in_response(cJSON *j_response, t_chat *chat);
void add_room_response(cJSON *j_response, t_chat *chat);
void mx_init_errors(t_chat *chat);


///**********************|event handlers|**********************///
void connect_authorization(t_chat *chat);
void connect_theme_switcher(t_chat *chat);
void connect_addroom(t_chat *chat);


///**********************|gui functions|**********************///
void init_gui(t_chat *chat);
gint start_gui(t_chat *chat);
void start_main_window(t_chat *chat);
GtkBuilder *init_window(gint argc, char **argv);

void css_connect_from_file(t_chat *chat);
void mx_css_connect(char *theme, t_chat *chat);
void change_theme_icon(t_chat *chat, gchar *icon_name);

t_gmsg *create_gmsg(cJSON *msg, t_chat *chat);
gboolean delete_gmsg(t_gmsg *gmsg);
t_groom *mx_get_groom_by_id(guint64 room_id, GtkBuilder *builder);
void entry_set_icon_by_path(GtkEntry *entry, gchar *path, GtkEntryIconPosition icon_pos);
void mx_widget_switch_visibility_by_name(GtkBuilder *builder, gchar *name);
void mx_widget_switch_visibility(GtkWidget *usr_ctrl, GtkWidget *widget);

GtkWidget *mx_msgcreate_eventbox();
GtkWidget *mx_msgcreate_box_main(GtkWidget *eventbox, gboolean is_own);
void mx_msgcreate_box_info(GtkWidget *box_main, t_gmsg *gmsg, gboolean is_own, t_chat *chat);

GtkWidget *mx_create_reg_message_row(t_gmsg *gmsg, gboolean is_own, t_chat *chat);
GtkWidget *mx_create_message_row(t_chat *chat, t_gmsg *gmsg);
void mx_widget_set_class(GtkWidget *widget, gchar *class);


///**********************|gui utils|**********************///
void clear_buffer_text(gchar *buff_name, GtkBuilder *builder);
void clear_label_by_name(gchar *label_name, GtkBuilder *builder);
void sig_reset_auth(GtkNotebook *note, GtkWidget *page, guint page_num, GtkBuilder *builder);
gchar *get_buffer_text(gchar *buff_name, GtkBuilder *builder);
