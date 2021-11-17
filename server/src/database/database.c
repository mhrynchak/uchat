//
// Created by Максим Гринчак on 10/29/21.
//

#include "database.h"

void init_client() {
    
}

void insert_socket(sqlite3 *db, char *username, int sockfd) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "UPDATE USERS SET SOCKET = ?1 WHERE LOGIN = ?2", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, sockfd);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    sqlite3_finalize(stmt);
}

bool user_not_online(sqlite3 *db, char *username) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE LOGIN = ?1 AND SOCKET = ?2", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, -1);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool correct_credentials(sqlite3 *db, char *username, char *password) {
    sqlite3_stmt *stmt;

//    fprintf(stdout, "checking for user: { %s; %s }\n", username, password);

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE LOGIN = ?1 AND PASS = ?2", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
    //    fprintf(stdout, "correct credentials\n\n");
        sqlite3_finalize(stmt);
        return true;
    }

    // fprintf(stdout, "invalid credentials\n\n");
    sqlite3_finalize(stmt);
    return false;
}

void create_new_user(sqlite3 *db, char *username, char *password) {
    sqlite3_stmt *stmt;

//    fprintf(stdout, "creating new user: { %s; %s }\n", username, password);

    sqlite3_prepare_v2(db, "INSERT INTO USERS(LOGIN,PASS,SOCKET) VALUES (?1, ?2, ?3);", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, -1);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

bool user_exists(sqlite3 *db, char *username) {
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT ID FROM USERS WHERE LOGIN = ?1", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

void create_rooms_table(sqlite3 *db) {
    vm_logger(VM_LOG_FILE, "creating rooms table");
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ROOMS("
                     "ID                INTEGER     PRIMARY     KEY     NOT NULL,"
                     "NAME              TEXT                            NOT NULL,"
                     "DESC              TEXT,"
                     /*"CUSTOMER_ID        INTEGER             NOT NULL,"
                     "DATE               INT                 NOT NULL,"
                     "TYPE               INT                 NOT NULL,"
                     "POWER              INTEGER             NOT NULL);"*/,
                0, 0, 0);
}

void create_users_table(sqlite3 *db) {
    vm_logger(VM_LOG_FILE, "creating users table");
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS USERS("
                     "ID        INTEGER PRIMARY KEY NOT NULL,"
                    //  "NAME      TEXT                NOT NULL,"
                     "LOGIN     TEXT    UNIQUE      NOT NULL,"
                     "PASS      TEXT                NOT NULL,"
                     "SOCKET    INTEGER                    );",
                 0, 0, 0);
}

void create_tables(sqlite3 *db) {
    create_users_table(db);
    create_rooms_table(db);
}

sqlite3 *open_db() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        vm_logger(VM_LOG_FILE, "error: cannot open the database");
    } else {
        vm_logger(VM_LOG_FILE, "database has been opened successfully");
    }
    create_tables(db);
    return db;
}
