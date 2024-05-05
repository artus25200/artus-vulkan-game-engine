##
# AVGE
#
# @file
# @version 0.1
CC := clang

LIB := bin/AVGE.a
APP := bin/AVGETestApp
APP_SRC := bin/app.a bin/AVGE.a
LIB_SRC := bin/engine.a bin/nicelog.a

INCLUDE := -I$(shell pwd)/include
export INCLUDE
export CC

default: clean makedir incl $(APP)

clean:
	rm -rf bin include

makedir:
	@mkdir bin
	@mkdir include

incl:
	$(eval INC := $(shell find . -name *.h))
	$(foreach i, $(INC), $(shell ln -f $(i) include/$(notdir $(i))))
	echo $(INCLUDE) > compile_flags.txt

$(APP): $(APP_SRC)
	$(CC) -o $@ $^

$(LIB): $(LIB_SRC)
	@-mkdir bin/tmp
	$(foreach src, $^, $(shell ar x --output bin/tmp $(src)))
	ar rcs $@ $(pastsub %, bin/tmp/%, $(shell find bin/tmp -name *.o))
	@rm -rf bin/tmp/

%.a:
	$(MAKE) -C $(basename $(notdir $@))
	mv $(basename $(notdir $@))/$@ $@

# end
