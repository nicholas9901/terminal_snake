EXE     = snake
OBJS    = main.o point.o startup.o draw.o
SDIR    = src
HEADERS = $(SDIR)/prototypes.h $(SDIR)/definitions.h 
LD      = clang
FLAGS   = -g

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $(FLAGS) $(OBJS) -o $(EXE)

main.o: $(SDIR)/main.c startup.o point.o draw.o $(HEADERS)
	$(LD) -c $(FLAGS) $(SDIR)/main.c

point.o: $(SDIR)/point.c startup.o $(HEADERS)
	$(LD) -c $(FLAGS) $(SDIR)/point.c

draw.o: $(SDIR)/draw.c startup.o $(HEADERS)
	$(LD) -c $(FLAGS) $(SDIR)/draw.c

startup.o: $(SDIR)/startup.c $(HEADERS)
	$(LD) -c $(FLAGS) $(SDIR)/startup.c

clean:
	rm -f $(EXE) *.o

.PHONY: all default