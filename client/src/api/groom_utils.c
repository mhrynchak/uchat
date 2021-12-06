#include "client.h"

void reset_select_count(t_groom *groom) {
    groom->select_own = 0;
    groom->select_another = 0;
    groom->select_notedit = 0;
    groom->select_all = 0;
}

void delete_groom(t_groom *room) {
    if (room) {
        vm_free((void**)&(room->room_name));
        vm_free((void**)&(room->customer));
        vm_free((void**)&(room->desc));
        vm_free((void**)&room);
        room = NULL;
    }
}

t_groom *init_groom(void) {
    t_groom *room = vm_malloc(sizeof(t_groom));

    room->room_name = NULL;
    room->customer = NULL;
    room->box_rooms = NULL;
    room->box_messages = NULL;
    room->page = NULL;
    room->row_room = NULL;
    room->stack_msg = NULL;
    room->id = -1;
    room->date = -1;
    room->is_updated = TRUE;
    room->desc = NULL;
    room->is_watched = FALSE;
    room->user_id = 0;
    room->members = g_hash_table_new(g_direct_hash, g_direct_equal);
    room->uploaded = 0;
    return room;
}

t_groom *create_groom(cJSON *j_data) {
    t_groom *groom = init_groom();

    reset_select_count(groom);
    groom->room_name = vm_get_valuestring(j_data, "name");
    groom->user_id = vm_get_object(j_data, "user_id")->valueint;
    groom->id = vm_get_object(j_data, "id")->valueint;
    groom->date = vm_get_object(j_data, "date")->valueint;
    groom->desc = vm_get_valuestring(j_data, "desc");
    return groom;
}
