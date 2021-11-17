//
// Created by Максим Гринчак on 11/9/21.
//
#pragma once
#include "client.h"
#include "defines.h"


/* Groom
 * ----------
 * box_rooms: GtkLisBox of all rooms
 * page: GtkStack page of this room with messages
 * row_room: pointer to room GtkListBoxRow
 * stack_msg: GtkStack of pages with messages
 * box_messages: GtkListBox of all messages or room
 * label_name: pointer to GtkLabel with room name
 * members: hash table with all members in room
 * is_watched: flag that check is watched new messages in room
 * id: room id
 * room_name: name of room
 * customer: customer's login
 * customer_id: customer's id
 * date: creation date of room
 * desc: description of room
 * is_updated: flag that check is room has new messages
 * uploaded: count of uploaded messages
 * power: power that room used
 */
typedef struct s_groom {
    GtkListBox *box_rooms;
    GtkScrolledWindow *page;
    GtkListBoxRow *row_room;
    GtkStack *stack_msg;
    GtkListBox *box_messages;
    GtkLabel *label_name;
    GHashTable *members;
    gboolean is_watched;
    guint64 id;
    char *room_name;
    char *customer;
    guint64 customer_id;
    guint64 date;
    char *desc;
    gboolean is_updated;
    gint uploaded;
    gdouble power;
    gboolean select_all;
    gboolean select_own;
    gboolean select_another;
    gboolean select_notedit;
}                   t_groom;


/* Gmsg
 * ----------
 * row_msg: pointer to message GtkListBoxRow
 * label_text: pointer to GtkLabel with message content
 * label_power: pointer to label with message power
 * type: type of message (STICKER, FILE ...)
 * msg: text of message
 * login: login of sender
 * date: send date
 * room_id: room that contains this message
 * message_id: message id
 * power: power that message used
 * select_all: count of all selected messages
 * select_own: count of own selected messages
 * select_another: count of selected messages from another users
 * select_notedit: count of selected not editable messages
 */
typedef struct s_gmsg {
    GtkListBoxRow *row_msg;
    GtkLabel *label_text;
    GtkLabel *label_power;
    gint type;
    char *msg;
    char *login;
    guint64 date;
    guint64 room_id;
    guint64 message_id;
    gdouble power;
}           t_gmsg;

/* REQUEST_TYPE
 * ----------
 * All types of request in data transport protocol
 */
typedef struct s_dtp {
    char *str;
    cJSON *json;
    size_t len;
    gint type;
}               t_dtp;


/* out: data output stream to server
 * in: data input stream to server
 * client_conn: client connection to server
 * auth_token: authorization token
 * login: user login
 * desc: user description
 * argc: arguments count
 * argv: arguments array
 * id: user id
 * curr_room: current selected room in chat      
 * data: dtp data                               
 * upl_old_msgs: status of uploading      
 * builder: gtk builder
 * valid: validation packets
 * error_handler: array of error handlers    
 * request_hanlder: array of request handlers 
 * msg_placeholder: status of placeholder
 * shift_hold: flag if shift is hold 
 * css_prov: contain css-styles
 */
typedef struct s_chat {
    // int sockfd;
    // int port;
    GDataOutputStream *out;
    GDataInputStream *in;
    GSocketConnection *conn;
    GSocketClient *client_conn;
    char *auth_token;
    char *login;
    gchar *desc;
    gint argc;
    char **argv;
    gsize id;
    t_groom *curr_room;
    char *data;
    gboolean upl_old_msgs;
    GtkBuilder *builder;
    gboolean valid;
    void (*error_handler[ER_COUNT])(GtkBuilder *builder);
    gboolean (*request_handler[RQ_COUNT])(t_dtp *dtp, struct s_chat *chat);                                              
    gboolean msg_placeholder;
    gboolean shift_hold;
    GtkCssProvider *css_prov;
}               t_chat;
