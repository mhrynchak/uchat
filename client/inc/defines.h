#pragma once

#define MAX_CLIENTS 10
#define BUF_SIZE 2048
#define BACKLOG 10
#define NAME_LEN 32

/*
 * Icons and images paths
 */
#define IMGS_PATH "../src/gui/resources/"
#define STICKER_PATH IMGS_PATH"stickers/"
#define GUI_PATH "../src/gui/gui.glade"
#define IMG_EYE IMGS_PATH"eye.png"
#define IMG_CLOSEDEYE IMGS_PATH"closed-eye.png"
#define CACHE "client/cache"


/* Themes */
#define DARK_THEME "../src/gui/dark_theme.css"
#define LIGHT_THEME "../src/gui/light_theme.css"
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


/* ERROR_TYPE
 * ----------
 * All types of errors in protocol
 */
typedef enum e_error_type {
    ER_AUTH_DATA = 0,
    ER_USER_EXST,
    ER_CLI_EXST,
    ER_COUNT
}            t_error_type;
