#include "client.h"

/*
 * Unselect all messages in current room
 */
void unselect_curr_room_messages(GtkBuilder *builder) {
    t_groom *groom = get_selected_groom(builder, "listbox_rooms");

    if (groom) {
        gtk_list_box_unselect_all(groom->box_messages);
        reset_select_count(groom);
        sig_hide_msg_editing(NULL, builder);
    }
}

void reset_room_for_search(GtkBuilder *builder) {
    switch_room_header(builder, ROOM_CTRL);
    unselect_curr_room_messages(builder);
    sig_hide_msg_editing(NULL, builder);
}

void reset_room_message(t_groom *new_selected, GtkBuilder *builder) {
    t_groom *groom = get_selected_groom(builder, "listbox_rooms");
    if (groom && (!new_selected || new_selected->id != groom->id)) {
        reset_room_for_search(builder);
        sig_stop_search_message(NULL, NULL, builder); 
    }
}

