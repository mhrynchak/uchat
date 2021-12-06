#pragma once

#define GTK_STYLE_CLASS_TOOLTIP "tooltip"

#define MAX_CLIENTS 10
#define BUF_SIZE 2048
#define BACKLOG 10
#define NAME_LEN 32

/*
 * Icons and images paths
 */
#define IMGS_PATH "../resources/icons/"
#define STICKER_PATH IMGS_PATH"../stickers/"
#define GUI_PATH "../resources/gui.glade"
#define IMG_EYE IMGS_PATH"eye.png"
#define IMG_CLOSEDEYE IMGS_PATH"closed-eye.png"
#define CACHE "client/cache"


/* Themes */
#define DARK_THEME "../resources/dark_theme.css"
#define LIGHT_THEME "../resources/light_theme.css"
#define THEME_FILE "set_theme"

/*
 * Error messages for authorization
 */
#define ERRMSG_INVALID_LOGIN "Login can be minimum 3 symbol of a-z, 0-9, -"
#define ERRMSG_NODATA "Please, enter login and password"
#define ERRMSG_DIFPASS "Passwords must match"
#define ERRMSG_INCCRDATA "The login or password inccorect"
#define ERRMSG_USEREXIST "User already exist"
#define ERRMSG_CLIEXIST "User already authorized"

// /*
//  * GtkListBox of room lists
//  */
#define LOCAL_ROOMS "listbox_rooms"
#define GLOBAL_ROOMS "listbox_global_rooms"

/*
 * Keybord
 */
#define ENTER_KEY (gint)65293
#define SHIFT_KEY (gint)65505
/*
 * Controll-headerbar
 */
#define ROOM_CTRL 0
#define MSG_CTRL 1
#define BUF_MSGS 30


/*
 * Formatting characters
 */
#define VM_FT_SCRATCH "~~"
#define VM_FT_BOLD "**"
#define VM_FT_IMPORTANT "``"
#define VM_FT_ITALIC "##"
#define VM_FT_UNDER "__"

/*
 * Formatting spans
 */
#define VM_OP_SCRATCH "<span strikethrough=\"true\">%s</span>"
#define VM_OP_BOLD "<span font_weight=\"bold\">%s</span>"
#define VM_OP_IMPORTANT "<span background=\"#FF698C7F\">%s</span>"
#define VM_OP_ITALIC "<span font_style=\"italic\">%s</span>"
#define VM_OP_UNDER "<span underline=\"single\">%s</span>"
#define VM_CLOSE_SPAN "</span>"
