#include "server.h"

/*
 * Function: close_db
 * -------------------------------
 * cleans structure sqlite3
 * 
 * db: pointer to the structure sqlite3
 */
void close_db(sqlite3 *db) {
    gint32 rv = sqlite3_close(db);

    if (rv != SQLITE_OK) {
        vm_logger(VM_LOG_FILE, "close database");
    }
}
