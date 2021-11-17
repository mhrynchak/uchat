OBJ = vmchat.o
LIBNAME = libvmchat

CC = gcc
CFLAGS = -std=c11
WFLAGS = -Wall -Wextra -Werror -Wpedantic 
LFLAGS = -I../inc

GTK_INC = -D_GNU_SOURCE `pkg-config --cflags gtk+-3.0`
LIB_PATH = -L../libraries/lib -lsqlite3 -ldl -lcjson


#library file extensions
STATIC = a

VM_STATIC = ../lib/$(LIBNAME).$(STATIC)


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

static: $(VM_STATIC)

$(VM_STATIC): $(OBJ)
	@cp vmchat.h ../inc/
	@$(AR) rcs $@ $<
	@printf "$M$(notdir $@)	$Gcreated$D\n"

$(OBJ): vmchat.c vmchat.h
	@$(CC) $(CFLAGS) $(WFLAGS) $(LFLAGS) $(GTK_INC) -c $< 

uninstall: clean
	@rm -rf $(VM_STATIC)
	@printf "$M$(LIBNAME).$(STATIC)	$Runinstalled$D\n"

clean:
	@rm -rf $(OBJ)
	@rm -rf ../inc/vmchat.h
	@printf "$Gvmchat	objs	cleaned$D\n"

reinstall: uninstall all

.PHONY: all static clean uninstall reinstall
