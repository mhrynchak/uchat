#include "client.h"

void init_gui(t_chat *chat) {
    connect_authorization(chat);
    connect_addroom(chat);
    // mx_connect_send_message(chat);
    // mx_connect_profile_settings(chat);
    // mx_connect_room_settings(chat);
    // mx_connect_message_ctrl(chat);
    // mx_connect_search(chat);
    // mx_connect_join_to_room(chat);
    // mx_connect_stickers(chat);
    // mx_connect_ban_member(chat);
    // mx_connect_set_placeholder(chat);
    // mx_connect_unset_placeholder(chat);
    // mx_connect_filechooser(chat);
    connect_theme_switcher(chat);
}
