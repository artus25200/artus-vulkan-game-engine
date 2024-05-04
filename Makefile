# tool macros
CC ?= clang # FILL: the compiler
LFLAGS := -L/opt/local/lib -lglfw
CFLAGS := -g -c -Wall -ggdb -O0 -I/opt/local/include -D ENGINE_DEBUG
UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX 
	LFLAGS += -lvulkan
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX 
	LFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -lMoltenVK  -mmacosx-version-min=10.15
    endif
COBJFLAGS := $(CFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

# compile macros
TARGET_NAME := engine
TARGET := $(BIN_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
LIBS := src/nicelog/nicelog.a
# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ) $(LIBS)
	$(CC) $(LFLAGS) -o $@ $(OBJ) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

$(LIBS):
	make -C src/nicelog/

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	@make clean -C src/nicelog/
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)

.PHONY: run
run:
	./bin/$(TARGET)
