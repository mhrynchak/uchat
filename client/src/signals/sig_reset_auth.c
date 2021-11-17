#include "client.h"

void sig_reset_auth(GtkNotebook *note, GtkWidget *page,
                   guint page_num, GtkBuilder *builder) {
    GObject *login = gtk_builder_get_object(builder, "entry_login_password");
    GObject *signup = gtk_builder_get_object(builder, "entry_signup_password");
    GObject *confirm = gtk_builder_get_object(builder, "entry_signup_confirm");

    entry_set_icon_by_path(GTK_ENTRY(login), IMG_EYE,
                              GTK_ENTRY_ICON_SECONDARY);
    entry_set_icon_by_path(GTK_ENTRY(signup), IMG_EYE,
                              GTK_ENTRY_ICON_SECONDARY);
    gtk_entry_set_visibility(GTK_ENTRY(login), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(signup), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(confirm), FALSE);
    clear_buffer_text("buffer_login", builder);
    clear_buffer_text("buffer_password", builder);
    clear_buffer_text("buffer_password_confirm", builder);
    clear_label_by_name("label_autherror_login", builder);
    clear_label_by_name("label_autherror_signup", builder);
    (void)note;
    (void)page;
    (void)page_num;
}
