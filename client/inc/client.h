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
//gssize sender(GDataOutputStream *out, gchar *dtp);


///**********************|api utils|**********************///
bool match_search(char *str, char *regex);
bool match_nsearch(char *str, char *regex, size_t size);
bool is_valid_auth_data(char *login, char *pass, GtkLabel *label);
gchar *encrypt_str(gchar *str);
void choose_auth(GtkButton *btn, t_chat *chat);
void insert_member(cJSON *member, gint room_id, t_chat *chat);
void trim_message(gchar **message);
void switch_room_header(GtkBuilder *builder, gint page_index);
void vm_file_read(gsize size, gchar *name, GInputStream *in);
void send_ready(GSocketConnection *conn);
gboolean is_file_animation(gchar *filename);
gboolean is_file_image(gchar *filename);
void gupd_msg_text(gint msg_id, gint room_id, char *text, GtkBuilder *builder);
void unselect_curr_room_messages(GtkBuilder *builder);
void insert_old_msg(cJSON *msg, t_chat *chat, gint room_id);
void insert_room(cJSON *room, t_chat *chat);
void set_profile_info(GtkButton *btn, t_chat *chat);
void add_message_to_found(t_gmsg *gmsg, t_chat *chat);
void insert_new_msg(cJSON *msg, t_chat *chat, gint room_id);
void add_member(char *login, guint64 user_id, guint64 room_id, t_chat *chat);
void delete_row_msg(GtkListBoxRow *row, GtkBuilder *builder);

///**********************|json objects|**********************///
void json_manager(gchar *response, t_chat *chat);

///---requests
void sign_up_request(t_chat *chat);
void sign_in_request(t_chat *chat);
void new_room_request(GtkButton *btn, t_chat *chat);
void old_messages_request(GDataOutputStream *out, gint date, gint room_id);
void get_rooms_request(GDataOutputStream *out, gint date);
void get_members_request(t_chat *chat, gint room_id);
void send_message_request(GtkButton *btn, t_chat *chat);
void download_file_request(gint room_id, gint msg_id, t_chat *chat);
void upload_file_request(gchar *path, gint room_id, t_chat *chat);
void edit_message_request(GtkButton *btn, t_chat *chat);
gchar *join_room_request(gint room_id);
void log_out_request(GtkButton *btn, t_chat *chat);
void edit_user_request(GtkButton *btn, t_chat *chat);
void get_member_info_request(GObject *popup, t_chat *chat);
void search_room_request(GtkSearchEntry *sentry, t_chat *chat);
void search_msg_request(GtkSearchEntry *sentry, t_chat *chat);
void search_member_request(GtkSearchEntry *sentry, t_chat *chat);
void send_sticker_request(GtkButton *btn, t_chat *chat);
void join_to_room_request(GtkButton *btn, t_chat *chat);
void new_messages_request(GDataOutputStream *out, gint date, gint room_id);
void del_msg_request(GtkListBox *box, GtkListBoxRow *row, t_chat *chat);
void ban_member_request(GObject *btn, t_chat *chat);
void del_room_request(GtkButton *btn, t_chat *chat);
void del_msg_response(cJSON *j_response, t_chat *chat);
void edit_room_name_request(GDataOutputStream *out, gint room_id, gchar *new_name);
void edit_room_desc_request(GDataOutputStream *out, gint room_id, gchar *new_desc);
void clear_room_request(GtkButton *btn, t_chat *chat);

///---responses
void sign_up_response(cJSON *j_response, t_chat *chat);
void sign_in_response(cJSON *j_response, t_chat *chat);
void new_room_response(cJSON *j_response, t_chat *chat);
void get_rooms_response(cJSON *j_response, t_chat *chat);
void get_members_response(cJSON *j_response, t_chat *chat);
void send_message_response(cJSON *j_response, t_chat *chat);
void edit_message_response(cJSON *j_response, t_chat *chat);
void old_messages_response(cJSON *j_response, t_chat *chat);
void join_room_response(cJSON *j_respose, t_chat *chat);
void log_out_response(cJSON *j_response, t_chat *chat);
void search_room_response(cJSON *j_response, t_chat *chat);
void search_msg_response(cJSON *j_response, t_chat *chat);
void join_to_room_response(cJSON *j_response, t_chat *chat);
void new_member_response(cJSON *j_response, t_chat *chat);
void new_messages_response(cJSON *j_response, t_chat *chat);
void ban_member_response(cJSON *j_response, t_chat *chat);
void del_room_response(cJSON *j_response, t_chat *chat);
void edit_user_response(cJSON *j_response, t_chat *chat);
void get_member_info_response(cJSON *j_response, t_chat *chat);
void edit_room_name_response(cJSON *j_response, t_chat *chat);
void edit_room_desc_response(cJSON *j_response, t_chat *chat);
void clear_room_response(cJSON *j_response, t_chat *chat);

///**********************|gui functions|**********************///
void init_gui(t_chat *chat);
gint start_gui(t_chat *chat);
void start_main_window(t_chat *chat);
GtkBuilder *init_window(gint argc, char **argv);

void css_connect_from_file(t_chat *chat);
void css_connect(char *theme, t_chat *chat);
void change_theme_icon(t_chat *chat, gchar *icon_name);

t_gmsg *create_gmsg(cJSON *msg, t_chat *chat);
gboolean delete_gmsg(t_gmsg *gmsg);
void entry_set_icon_by_path(GtkEntry *entry, gchar *path, GtkEntryIconPosition icon_pos);

GtkWidget *msgcreate_eventbox();
GtkWidget *msgcreate_box_main(GtkWidget *eventbox, gboolean is_own);
void msgcreate_box_info(GtkWidget *box_main, t_gmsg *gmsg, gboolean is_own, t_chat *chat);

GtkWidget *create_reg_message_row(t_gmsg *gmsg, gboolean is_own, t_chat *chat);
GtkWidget *create_message_row(t_chat *chat, t_gmsg *gmsg);

void add_messages_box(t_groom *room, t_chat *chat);
void add_room_row(t_groom *room, t_chat *chat, gchar *listbox_name);

void reset_select_count(t_groom *groom);

t_signal_data *create_sigdata(t_chat *chat, t_groom *groom, GtkListBoxRow *row_msg);
void vm_free_sigdata(t_signal_data *data);
void clear_global_search(GtkBuilder *builder);
void search_local_rooms(GtkBuilder *builder, t_filter_data *data);
gboolean stop_search_room(gpointer *entry, gpointer *data, GtkBuilder *builder);
void search_delim_set_visibility(GtkBuilder *builder, gboolean is_visible);
void set_default_room_sett(GtkBuilder *builder);
void switch_room_header(GtkBuilder *builder, gint page_index);
void set_room_widgets_visibility(GtkBuilder *builder, gboolean visibility);
void unselect_room(t_groom *groom, GtkBuilder *builder);
void delete_row_room(GtkListBoxRow *row, GtkBuilder *builder);

void reset_addroom(GtkButton *btn, GtkBuilder *builder);
void set_current_room_sett(GtkBuilder *builder);
void select_room(GtkWidget *event_box, GdkEventButton *event, gpointer *user_data);
void show_join_to_room(GtkWidget *event_box, GdkEventButton *event, gpointer *user_data);
void reset_room_message(t_groom *new_selected, GtkBuilder *builder);
void reset_room_for_search(GtkBuilder *builder);
void clear_found_msgs(GtkBuilder *builder);
void set_room_members(GtkBuilder *builder, t_groom *groom); 
void widget_remove_class(GtkWidget *widget, gchar *class);
void widget_set_class(GtkWidget *widget, gchar *class);
void add_to_sett_members(gint *key, gchar *value, GtkBuilder *builder);
void add_to_info_members(gint *key, gchar *value, GtkBuilder *builder);
void search_members(GtkBuilder *builder, gchar *search_login);

t_filter_data *create_filter_data(gchar *search_name);

void gupd_room_name(guint64 id, char *name, GtkBuilder *builder);
void gupd_room_desc(guint64 id, char *desc, GtkBuilder *builder);
void gupd_clear_history(GtkBuilder *builder, guint64 room_id);

///**********************|gui utils|**********************///
void clear_buffer_text(gchar *buff_name, GtkBuilder *builder);
gchar *get_buffer_text(gchar *buff_name, GtkBuilder *builder);
void focus_out(GtkWidget *widget, GdkEvent *event, gpointer *user_data);
gchar *entry_get_text(gchar *entry_name, GtkBuilder *builder);
void widget_set_visibility(GtkWidget *widget, gboolean is_visible);
void widget_set_visibility_by_name(GtkBuilder *builder, gchar *name, gboolean is_visible);
void widget_switch_visibility_by_name(GtkBuilder *builder, gchar *name);
gboolean widget_is_visible(gchar *widget_name, GtkBuilder *builder);
void send_message_handle_enter(GtkTextView *textview, GdkEvent *event, t_chat *chat);
void send_message_handle_shift(GtkWidget *textview, GdkEvent *event, t_chat *chat);
void add_message_to_room_new(t_gmsg *msg, t_chat *chat);
void add_message_to_room_old(t_gmsg *msg, t_chat *chat);
void msgcreate_label_time(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own);
void msgcreate_label_text(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own);
void msgcreate_img_sticker(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own);
void msgcreate_label_login(GtkWidget *box_main, t_gmsg *gmsg);
void select_msg(gpointer *eventbox, gpointer *event, t_signal_data *data);
void label_set_num(gchar *widgetname, GtkBuilder *builder, gint number);
void clear_label_by_name(gchar *label_name, GtkBuilder *builder);
void msgcreate_file(GtkWidget *box_info, t_gmsg *gmsg, gboolean is_own, t_chat *chat);
void open_files_dir(GtkButton *btn, t_chat *chat);
gchar *format_text(gchar *text);
t_gmsg *get_gmsg_by_id(guint64 msg_id, guint64 room_id, GtkBuilder *builder);
void set_room_sett(GtkButton *btn, t_chat *chat);
void req_room_sett(GtkButton *btn, t_chat *chat);
void req_room_name_sett(GtkButton *btn, t_chat *chat);
void delete_selected_msgs(GtkButton *btn, t_chat *chat);
void show_edit_msg(GtkButton *btn, t_chat *chat);
void unselect_msg(GtkButton *btn, GtkBuilder *builder);

///**********************|event handlers|**********************///
void connect_authorization(t_chat *chat);
void connect_addroom(t_chat *chat);
void connect_send_message(t_chat *chat);
void connect_profile_settings(t_chat *chat);
void connect_room_settings(t_chat *chat);
void connect_message_ctrl(t_chat *chat);
void connect_search(t_chat *chat);
void connect_join_to_room(t_chat *chat);
void connect_stickers(t_chat *chat);
void connect_ban_member(t_chat *chat);
void connect_set_placeholder(t_chat *chat);
void connect_unset_placeholder(t_chat *chat);
void connect_filechooser(t_chat *chat);
void connect_theme_switcher(t_chat *chat);

///**********************|signals|**********************///
void sig_reset_auth(GtkNotebook *note, GtkWidget *page, gint page_num, GtkBuilder *builder);
void sig_widget_switch_visibility(GtkWidget *usr_ctrl, GtkWidget *widget);
void sig_switch_theme(GtkWidget *widget, t_chat *chat);
void sig_hide_msg_editing(GtkButton *btn, GtkBuilder *builder);
gboolean sig_stop_search_room(gpointer *entry, gpointer *data, GtkBuilder *builder);
gboolean sig_stop_search_message(gpointer *entry, gpointer *data, GtkBuilder *builder);
gboolean sig_stop_search_members(gpointer *entry, gpointer *data, GtkBuilder *builder);

///**********************|groom utils|**********************///
t_groom *create_groom(cJSON *j_data);
void delete_groom(t_groom *room);
gboolean is_same_groom(t_groom *groom, GtkBuilder *builder);
t_groom *get_selected_groom(GtkBuilder *builder, gchar *list_name);
t_groom *get_groom_by_id(guint64 room_id, GtkBuilder *builder);


///**********************|gmsg utils|**********************///
t_gmsg *get_selected_gmsg(GtkBuilder *builder);
