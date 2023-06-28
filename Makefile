AS = /home/nicholas/Applications/gbdk/bin/lcc -c
CC = /home/nicholas/Applications/gbdk/bin/lcc -Wa-l -Wl-m

project_name = helloworld

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := build

BIN := $(BIN_DIR)/$(project_name).gb
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(BIN)

%.s: %.ms
	maccer -o $@ $<

$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir $@

ifdef OS
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
   endif
endif

clean:
	@$(RM) $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
