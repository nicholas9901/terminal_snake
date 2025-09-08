DIR_BUILD = build
DIR_SRC   = src
EXE       = $(DIR_BUILD)/terminal_snake
OBJS      = $(DIR_BUILD)/main.o    \
			$(DIR_BUILD)/startup.o \
			$(DIR_BUILD)/input.o   \
			$(DIR_BUILD)/point.o   \
			$(DIR_BUILD)/draw.o    
HEADERS   = $(DIR_SRC)/prototypes.h $(DIR_SRC)/definitions.h 
CC        = clang
FLAGS     = -g

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@

$(DIR_BUILD)/main.o: $(DIR_SRC)/main.c $(HEADERS) $(filter-out $(DIR_BUILD)/main.o, $(OBJS))
	$(CC) -c $(FLAGS) $< -o $@

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.c $(HEADERS)
	$(CC) -c $(FLAGS) $< -o $@
	
clean:
	rm -f $(EXE) $(OBJS)

.PHONY: all clean
