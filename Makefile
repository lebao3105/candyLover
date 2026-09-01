PREFIX?=/usr
BUILD_DIR?=build
# Or release
MODE?=debug
CFLAGS=-Iinclude -I3rdparty/concord/include -I3rdparty/sds/ \
	   -I3rdparty/concord/core -I3rdparty/concord/gencodecs \
	   -I3rdparty/concord/generated -I3rdparty/concord/reflect-c \
	   $(shell pkg-config --cflags libcurl) \
	   $(shell pkg-config --cflags lua) \
	   -Wall -Wextra -Wpedantic -std=c23 \
	   -D_=gettext -D__STDC_WANT_LIB_EXT1__ \
	   -D_XOPEN_SOURCE=600
LIBS=-L3rdparty/concord/lib -ldiscord \
	 $(shell pkg-config --libs libcurl) \
	 $(shell pkg-config --libs lua)
CC?=cc

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRCS=$(call rwildcard,src,*.c)
OBJS=$(SRCS:%=$(BUILD_DIR)/%)
OBJS:=$(OBJS:.c=.o)

all: config soc

config:
	mkdir -p $(BUILD_DIR)
	$(MAKE) -C 3rdparty/concord static
ifeq ($(MODE),debug)
CFLAGS+=-O0 -g
else
CFLAGS+=-O3
endif

soc: config $(OBJS)
	$(CC) $(OBJS) 3rdparty/sds/sds.c -Wall -Wpedantic -o build/soc $(LIBS)

compile_commands:
	bear --output build/compile_commands.json -- $(MAKE) $(OBJS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*

deepclean: clean
	$(MAKE) -C 3rdparty/concord clean

.PHONY: all config soc compile_commands clean deepclean
