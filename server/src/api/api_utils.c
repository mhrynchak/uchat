#include "server.h"

gboolean is_valid_room_name(gchar *roomname) {
    size_t len = strlen(roomname);
    if (!roomname) {
        return false;
    }
    if (len <= 0 || len > MAX_ROOM_LEN) {
        return false;
    }
    return true;
}
