## List of utility functions
define temp_object
$(foreach fil, $(notdir $(1)), $(addprefix bin/temp/, $(fil:.c=.o)))
endef

define compile
$(strip $(call temp_object, $(1))): $(1)
	gcc -c $(1) -o $(call temp_object, $(1)) $(INCLUDE)
endef


INC=./source/
INCLUDE = $(foreach file, $(INC), $(addprefix -I,$(file)))

SLICES := source/core/core_*.c source/libraries/lib_*.c source/datamodel/dm_*.c
SLICE_FILES := $(foreach slice, $(SLICES), $(wildcard $(slice)))
SLICE_FILES_TEMP := $(foreach file, $(SLICE_FILES), $(call temp_object, $(file)))

bin/main: main.c bin/ClassDB.a Makefile
	gcc main.c bin/ClassDB.a -o $@ $(INCLUDE)
	@echo Done Compiling.

bin/ClassDB.a: $(SLICE_FILES_TEMP)
	ar r $@ $^

$(foreach file, $(SLICE_FILES), $(eval $(call compile, $(file))))

.PHONY: clean

clean:
	-rm bin/temp/*.o
	-rm bin/main
	-rm bin/ClassDB.a