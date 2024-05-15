##
# AVGE
#
# @file
# @version 0.1
#
CC := clang

LIB := bin/AVGE.a
APP := bin/AVGETestApp
APP_SRC := bin/app.a bin/AVGE.a
LIB_SRC := bin/core.a bin/nicelog.a

INCLUDE := -I$(shell pwd)/include -I/opt/local/include

LFLAGS := -L/opt/local/lib -lglfw
CFLAGS := -g -c -Wall -ggdb -O0

UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
	LFLAGS += -lvulkan
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
	LFLAGS += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -lMoltenVK  -mmacosx-version-min=10.15
    endif

export INCLUDE
export CC
export CFLAGS

default: clean makedir incl $(APP)

.PHONY:silent
silent:
	@$(MAKE) > /dev/null

.PHONY:clean
clean:
	rm -rf bin include

.PHONY:makedir
makedir:
	@mkdir bin
	@mkdir bin/tmp
	@mkdir include

.PHONY:incl
incl:
	$(eval INC := $(shell find . -name *.h))
	$(foreach i, $(INC), $(shell ln -f $(i) include/$(notdir $(i))))
	-@$(shell rm compile_flags.txt)
	$(foreach i, $(INCLUDE), $(shell echo $(i) >> compile_flags.txt))

$(APP): $(APP_SRC)
	$(CC) $(LFLAGS) -o $@ $^

$(LIB): $(LIB_SRC)
	$(foreach src, $^, $(shell ar x --output bin/tmp $(src)))
	ar rcs $@ $(shell find bin/tmp -name *.o)

%.a:
	$(MAKE) -C $(basename $(notdir $@)) TARGET=$@
	mv $(basename $(notdir $@))/$@ $@

.PHONY: run
run: $(APP)
	./$(APP)

# end
