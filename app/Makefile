##
# AVGE
#
# @file
# @version 0.1

SRC := $(shell find src -name *.c)
OBJ_DIR := obj
OBJ := $(SRC:%.c=%.o)
TARGET := bin/app.a

default: clean makedir $(TARGET)

.PHONY: clean
clean:
	@-rm -rf bin $(OBJ_DIR)

.PHONY: makedir
makedir:
	@mkdir bin
	@mkdir $(OBJ_DIR)

$(TARGET): $(OBJ)
	@echo $(OBJ)
	ar rcs $@ $(foreach obj, $^, $(OBJ_DIR)/$(notdir $(obj)))

%.o: %.c
	$(CC) $(INCLUDE) -c $< -o $(OBJ_DIR)/$(notdir $@)
# end
