#include "client.h"

static void delete_older_message(t_groom *groom, GtkBuilder *builder) {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(groom->box_messages, 0);

    reset_room_message(NULL, builder);
    gtk_widget_destroy(GTK_WIDGET(row));
}

static void delete_message_from_upper(GtkScrolledWindow *scroll, t_groom *groom, GtkBuilder *builder) {
    GtkRequisition req;
    GtkRequisition req2;
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(scroll);
    GtkListBoxRow *tmp = gtk_list_box_get_row_at_index(groom->box_messages, 0);

    gtk_widget_get_preferred_size(GTK_WIDGET(tmp), &req, &req2);
    delete_older_message(groom, builder);
    gtk_adjustment_set_upper(adj, gtk_adjustment_get_upper(adj) - req2.height);
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    groom->uploaded--;
}

static void my_gtk_post_top(GtkPositionType pos, t_chat *chat, t_groom *groom, GtkScrolledWindow *scroll) {
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(scroll);
    GtkListBoxRow *glb = NULL;
    t_gmsg *msg = NULL;

    if (pos == GTK_POS_TOP && !chat->upl_old_msgs) {
        if (!gtk_list_box_get_row_at_index(groom->box_messages, 0))
            return;
        glb = gtk_list_box_get_row_at_index(groom->box_messages, 0);
        msg = (t_gmsg*)g_object_get_data(G_OBJECT(glb), "gmsg");
        old_messages_request(chat->out, msg->date, groom->id);
        chat->upl_old_msgs = TRUE;
    }
    if (pos == GTK_POS_TOP)
        gtk_adjustment_set_value(adj, gtk_adjustment_get_value(adj) + 1);
}

static void box_messages_reached(GtkScrolledWindow *scroll, GtkPositionType pos, t_chat *chat) {
    t_groom *groom = get_selected_groom(chat->builder, "listbox_rooms");

    my_gtk_post_top(pos, chat, groom, scroll);
    if (pos == GTK_POS_BOTTOM) {
        chat->upl_old_msgs = TRUE;
        while (groom->uploaded > BUF_MSGS)
            delete_message_from_upper(scroll, groom, chat->builder);
        chat->upl_old_msgs = FALSE;
    }
}

/*
 * Show "Go Down" butoon when user go up
 */
static void show_go_down(GtkAdjustment *adj, GtkBuilder *builder) {
    GObject *go_down = gtk_builder_get_object(builder, "btn_go_down");
    gdouble upper = gtk_adjustment_get_upper(adj);
    gdouble value = gtk_adjustment_get_value(adj);

    if (value < (upper - gtk_adjustment_get_page_size(adj) - 50))
        widget_set_visibility(GTK_WIDGET(go_down), TRUE);
    else
        widget_set_visibility(GTK_WIDGET(go_down), FALSE);
}

/*
 * Do GtkScrolledWindow autoscroll to down by name or pointer
 */
static void scroll_to_end(GtkAdjustment *adj, gpointer user_data) {
    static GtkAdjustment *last_adj = 0;
    static gdouble last_upper = 0;
    gdouble upper = gtk_adjustment_get_upper(adj);
    gdouble value = gtk_adjustment_get_value(adj);

    if (last_adj != adj)
        value = 0;
    if (upper < gtk_adjustment_get_page_size(adj)) {}
    else if (value > (last_upper - gtk_adjustment_get_page_size(adj) - 50) ||
            value == 0) {
        gtk_adjustment_set_value(adj,
                                 upper - gtk_adjustment_get_page_size(adj));
    }
    last_upper = gtk_adjustment_get_upper(adj);
    last_adj = adj;
    (void)user_data;
}

static void scrlldwnd_connect(gchar *name,
                                 GtkWidget *scroll,
                                 GtkBuilder *builder,
                                 t_groom *room) {
    GtkWidget *scrlldwnd = NULL;
    GtkAdjustment *adj = NULL;

    if (name) {
        scrlldwnd = GTK_WIDGET(gtk_builder_get_object(builder, name));
    }
    else {
        scrlldwnd = scroll;
    }
    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrlldwnd));
   g_signal_connect(adj, "changed", G_CALLBACK(scroll_to_end), room);
   g_signal_connect(adj, "changed", G_CALLBACK(show_go_down), builder);
   g_signal_connect(adj, "value-changed", G_CALLBACK(show_go_down), builder);
}

static gchar *msgpage_name(gint id) {
    return g_strdup_printf("%s_%d", "msgpage_", id);
}

static void create_viewport(GtkWidget *scroll, GtkWidget *box) {
    GtkWidget *view = gtk_viewport_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_container_add(GTK_CONTAINER(view), box);
}

void add_messages_box(t_groom *room, t_chat *chat) {
    GObject *stack = gtk_builder_get_object(chat->builder, "stack_message_rooms");
    GtkWidget *box = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    
    create_viewport(scroll, box);
    gtk_widget_set_can_focus(box, FALSE);
    room->box_messages = GTK_LIST_BOX(box);
    gtk_list_box_set_selection_mode(room->box_messages, GTK_SELECTION_MULTIPLE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_stack_add_named(GTK_STACK(stack), scroll, msgpage_name(room->id));
    room->stack_msg = GTK_STACK(stack);
    room->page = GTK_SCROLLED_WINDOW(scroll);
    gtk_widget_show_all(scroll);
    scrlldwnd_connect(NULL, scroll, chat->builder, room);
    g_signal_connect(scroll, "edge-reached", G_CALLBACK(box_messages_reached), chat);
}
