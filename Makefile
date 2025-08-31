DIR_BUILD = build
DIR_SRC   = src
EXE       = $(DIR_BUILD)/snake
OBJS      = $(DIR_BUILD)/main.o $(DIR_BUILD)/point.o $(DIR_BUILD)/startup.o $(DIR_BUILD)/draw.o
HEADERS   = $(DIR_SRC)/prototypes.h $(DIR_SRC)/definitions.h 
LD        = clang
FLAGS     = -g

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $(FLAGS) $(OBJS) -o $@

$(DIR_BUILD)/main.o: $(DIR_SRC)/main.c $(DIR_BUILD)/startup.o $(DIR_BUILD)/point.o $(DIR_BUILD)/draw.o $(HEADERS)
	$(LD) -c $(FLAGS) $< -o $@

$(DIR_BUILD)/point.o: $(DIR_SRC)/point.c $(DIR_BUILD)/startup.o $(HEADERS)
	$(LD) -c $(FLAGS) $< -o $@

$(DIR_BUILD)/draw.o: $(DIR_SRC)/draw.c $(DIR_BUILD)/startup.o $(HEADERS)
	$(LD) -c $(FLAGS) $< -o $@

$(DIR_BUILD)/startup.o: $(DIR_SRC)/startup.c $(HEADERS)
	$(LD) -c $(FLAGS) $< -o $@

clean:
	rm -f $(EXE) $(OBJS)

.PHONY: all default