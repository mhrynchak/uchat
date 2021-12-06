#include "client.h"

void init_gui(t_chat *chat) {
    connect_authorization(chat);          //
    connect_addroom(chat);              
    connect_send_message(chat);         
    connect_profile_settings(chat);     
    connect_room_settings(chat);        
    connect_message_ctrl(chat);         
    connect_search(chat);               
    connect_join_to_room(chat);         
    connect_stickers(chat);             
    connect_ban_member(chat);
    connect_set_placeholder(chat);
    connect_unset_placeholder(chat);
    connect_filechooser(chat);
    connect_theme_switcher(chat);
}
