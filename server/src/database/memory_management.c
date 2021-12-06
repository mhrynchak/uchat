#include "server.h"

void free_db_user(t_db_user *user) {
    if (user) {
        if (user->name) {
            g_free(user->name);
        }
        if (user->login) {
            g_free(user->login);
        }
        if (user->pass) {
            g_free(user->pass);
        }
        if (user->desc) {
            g_free(user->desc);
        }
        free((void *)user);
        user = NULL;
    }
}

/*
 * Function: free_room
 * -------------------------------
 * free struct t_db_room
 * 
 * room: structure t_db_room created with g_malloc
 */
void free_room(t_db_room **room) {
    if (!room || !*room)
        return;
    if ((*room)->room_name)
        g_free((*room)->room_name);
    if ((*room)->desc)
        g_free((*room)->desc);
    vm_free((void **)room);
    room = NULL;
}

/*
 * Function: free_message
 * -------------------------------
 * free struct t_db_message
 * 
 * message: structure t_db_message created with g_malloc
 */
void free_message(t_db_message **message) {
    if (!message || !*message)
        return;
    if ((*message)->message)
        g_free((*message)->message);
    if ((*message)->file_name)
        g_free((*message)->file_name);
    vm_free((void **)message);
    message = NULL;
}
