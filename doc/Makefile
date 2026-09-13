CC       ?= cc
CFLAGS   ?= -O2 -Wall -Wextra -Wpedantic -std=c99 -Wno-unused-local-typedefs
NAME     := example

INCLUDE_DIR       := ../include
LIBFINESWORD_NAME := finesword
LIBFINESWORD_DIR  := ../build/

SRC := $(NAME).c
OUT := $(NAME)
FLG := -I$(INCLUDE_DIR) $(CFLAGS) -L$(LIBFINESWORD_DIR) -l$(LIBFINESWORD_NAME) -Wl,-rpath,$(LIBFINESWORD_DIR)

$(OUT):
	$(CC) $(FLG) $(SRC) -o $(OUT)


.PHONY: clean
clean:
	rm -f $(OUT)
