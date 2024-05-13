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

INCLUDE := -I$(shell pwd)/include
export INCLUDE
export CC

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
	echo $(INCLUDE) > compile_flags.txt

$(APP): $(APP_SRC)
	$(CC) -o $@ $^

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
