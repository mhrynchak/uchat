#include "client.h"

void sig_reset_addroom(GtkButton *btn, GtkBuilder *builder) {
    clear_buffer_text("buffer_roomname", builder);
    (void)btn;
}
