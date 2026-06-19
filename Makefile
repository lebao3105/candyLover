PREFIX=/usr
BUILD_DIR=build
# Or release
MODE=debug
CFLAGS=-Iinclude -I3rdparty/concord/include \
	   -I3rdparty/concord/core -I3rdparty/concord/gencodecs \
	   $(shell pkg-config --cflags libcurl) \
	   $(shell pkg-config --cflags lua) \
	   -Wall -Wextra -Wpedantic -std=c23
LIBS=-L3rdparty/concord/lib -ldiscord \
	 $(shell pkg-config --libs libcurl) \
	 $(shell pkg-config --libs lua)
CC=cc

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRCS=$(call rwildcard,src,*.c)
OBJS=$(SRCS:%=$(BUILD_DIR)/%)
OBJS:=$(OBJS:.c=.o)

all: config soc

config:
	@mkdir -p $(BUILD_DIR)
ifeq ($(MODE),debug)
CFLAGS+=-O0 -g
else
CFLAGS+=-O3
endif

soc: $(OBJS)
	$(MAKE) -C 3rdparty/concord static
	$(CC) $(OBJS) -o build/soc $(LIBS)

compile_commands:
	bear --output build/compile_commands.json -- $(MAKE) soc

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

.PHONY: all config soc compile_commands
