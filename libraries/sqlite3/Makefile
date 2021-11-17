SQLITE_OBJ = sqlite3.o
SQLITE_LIBNAME = libsqlite3

CC = gcc 
CFLAGS = -std=c89 -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT=1

#library file extensions
STATIC = a

SQLITE_STATIC = ../lib/$(SQLITE_LIBNAME).$(STATIC)


#colors:
B = \033[1;94m#   BLUE
G = \033[1;92m#   GREEN
Y = \033[1;93m#   YELLOW
R = \033[1;31m#   RED
M = \033[1;95m#   MAGENTA
K = \033[K#       ERASE END OF LINE
D = \033[0m#      DEFAULT
A = \007#         BEEP


all: static

static: $(SQLITE_STATIC)

$(SQLITE_STATIC): $(SQLITE_OBJ)
	@cp sqlite3.h ../inc/
	@$(AR) rcs $@ $<
	@printf "$M$(notdir $@)	$Gcreated$D\n"

$(SQLITE_OBJ): sqlite3.c sqlite3.h
	@$(CC) $(CFLAGS) -c $<

uninstall: clean
	@rm -rf $(SQLITE_STATIC)
	@printf "$M$(SQLITE_LIBNAME).$(STATIC)	$Runinstalled$D\n"

clean:
	@rm -rf $(SQLITE_OBJ)
	@rm -rf ../inc/sqlite3.h
	@printf "$Gsqlite3	objs	cleaned$D\n"

reinstall: uninstall all

.PHONY: all static clean uninstall reinstall
