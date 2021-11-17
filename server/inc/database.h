//
// Created by Максим Гринчак on 10/29/21.
//

#pragma once

#include "server.h"

#define DB_PATH "database.db"

void init_client();
void insert_socket(sqlite3 *db, char *username, int sockfd);
bool user_not_online(sqlite3 *db, char *username);
bool correct_credentials(sqlite3 *db, char *username, char *password);
void create_new_user(sqlite3 *db, char *username, char *password);
bool user_exists(sqlite3 *db, char *username);
sqlite3 *open_db();

