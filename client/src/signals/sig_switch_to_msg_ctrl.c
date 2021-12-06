#include "client.h"

void sig_switch_to_msg_ctrl(GtkButton *btn, GtkBuilder *builder) {
    switch_room_header(builder, MSG_CTRL);
    (void)btn;
}
