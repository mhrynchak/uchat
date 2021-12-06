#include "client.h"

void open_file_folder(gchar *filename) {
    gchar *path = g_path_get_dirname(filename);
    GFile *file = g_file_new_for_path(path);
    gchar *uri = NULL;

    if (file)
        uri = g_file_get_uri(file);
    if (uri) {
        g_app_info_launch_default_for_uri(uri, NULL, NULL);
        g_free(path);
        g_free(uri);
    }
    if (file)
        g_object_unref(file);
}

static gboolean show_image_viewer(gchar *file_path, GtkBuilder *builder) {
    GObject *img = gtk_builder_get_object(builder, "img_view");
    GdkPixbuf *image = NULL;
    GdkPixbufAnimation *anim = NULL;

    if (is_file_image(file_path)) {
        image = gdk_pixbuf_new_from_file_at_size(file_path, 800, -1, NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(img), image);
        return TRUE;
    }
    else if (is_file_animation(file_path)) {
        anim = gdk_pixbuf_animation_new_from_file(file_path, NULL);
        gtk_image_set_from_animation(GTK_IMAGE(img), anim);
        return TRUE;
    }
    return FALSE;
}

static void unselect_msg_file(t_gmsg *gmsg) {
    t_signal_data *data = g_object_get_data(G_OBJECT(gmsg->row_msg), "sigdata");

    select_msg(NULL, NULL, data);
}

void open_files_dir(GtkButton *btn, t_chat *chat) {
    GObject *viewer = gtk_builder_get_object(chat->builder, "popup_image_viewer");
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");
    t_gmsg *gmsg = g_object_get_data(G_OBJECT(btn), "gmsg");
    GtkImage *img = GTK_IMAGE(gtk_button_get_image(btn));

    unselect_msg_file(gmsg);
    if (g_file_test(gmsg->msg, G_FILE_TEST_EXISTS)) {
        if (show_image_viewer(gmsg->msg, chat->builder)) {
            widget_set_visibility(GTK_WIDGET(viewer), TRUE);
        }
        else {
            open_file_folder(gmsg->msg);
        }
    }
    else {
        download_file_request(groom->id, gmsg->msg_id, chat);
        if (is_file_image(gmsg->msg) || is_file_animation(gmsg->msg)) {
            gtk_image_set_from_icon_name(img, "image", GTK_ICON_SIZE_DND);
        }
        else {
            gtk_image_set_from_icon_name(img, "document", GTK_ICON_SIZE_DND);
        }
    }
}
