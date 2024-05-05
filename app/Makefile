##
# AVGE
#
# @file
# @version 0.1

SRC := $(shell find src -name *.c)
OBJ_DIR := obj
OBJ := $(pastsub %, $(OBJ_DIR)/%, $(notdir $(SRC:%.c=%.o)))
TARGET := bin/app.a

default: clean makedir $(TARGET)

clean:
	@-rm -rf bin $(OBJ_DIR)

makedir:
	@mkdir bin
	@mkdir $(OBJ_DIR)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(INCLUDE) -c $< -o $@

# end
