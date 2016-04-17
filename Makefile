#C_PARAMS = -rdynamic

###############################################################################
##                              UTILITY FUNCTIONS                            ##
###############################################################################

define temp_object
$(foreach fil, $(notdir $(1)), $(addprefix bin/temp/, $(fil:.c=.o)))
endef

define get_header
$(foreach file, $(notdir $(1)), $(dir $(1))headers/$(file:.c=.h) $(dir $(1))headers/init.h)
endef

define compile
$(strip $(call temp_object, $(1))): $(1) $(COMMON_HEADERS) $(call get_header, $(1))
	gcc -g $(C_PARAMS) -c $(1) -o $(call temp_object, $(1)) $(INCLUDE)
endef



#######################################################################@#######
##                                VARIABLES                                  ##
###########################################################################@###

COMMON_HEADERS = source/common/headers/stdheaders.h
INC=./source/
INCLUDE = $(foreach file, $(INC), $(addprefix -I,$(file)))

SLICES := source/core/core_*.c source/libraries/lib_*.c source/datamodel/dm_*.c source/common/common_*.c
SLICE_FILES := $(foreach slice, $(SLICES), $(wildcard $(slice)))
SLICE_FILES_TEMP := $(foreach file, $(SLICE_FILES), $(call temp_object, $(file)))






###############################################################################
##                            MAIN DEFINITIONS                               ##
###############################################################################

bin/main: main.c bin/ClassDB.a Makefile
	gcc -g $(C_PARAMS) main.c bin/ClassDB.a -o $@ $(INCLUDE)
	@echo Done Compiling.

bin/ClassDB.a: $(SLICE_FILES_TEMP)
	ar r $@ $^

$(foreach file, $(SLICE_FILES), $(eval $(call compile, $(file))))




.PHONY: clean run
	
run: bin/main
	clear
	@bin/main

clean:
	-rm bin/temp/*.o
	-rm bin/main
	-rm bin/ClassDB.a