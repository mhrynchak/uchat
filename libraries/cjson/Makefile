CJSON_OBJ = cJSON.o
CJSON_LIBNAME = libcjson

CC = clang -std=c89

PIC_FLAGS = -fPIC
R_CFLAGS = $(PIC_FLAGS) -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion

#library file extensions
STATIC = a

CJSON_STATIC = ../lib/$(CJSON_LIBNAME).$(STATIC)

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

static: $(CJSON_STATIC)

$(CJSON_STATIC): $(CJSON_OBJ)
	@#mkdir ../inc ../lib
	@cp cJSON.h ../inc/
	@$(AR) rcs $@ $<
	@printf "$M$(notdir $@)	$Gcreated$D\n"

$(CJSON_OBJ): cJSON.c cJSON.h
	@$(CC) -c $(R_CFLAGS) $<

uninstall: clean
	@rm -rf $(CJSON_STATIC)
	@printf "$M$(CJSON_LIBNAME).$(STATIC)	$Runinstalled$D\n"

clean:
	@rm -rf $(CJSON_OBJ)
	@rm -rf ../inc/cJSON.h
	@printf "$Gcjson	objs	cleaned$D\n"

reinstall: uninstall all

.PHONY: all static clean uninstall reinstall
