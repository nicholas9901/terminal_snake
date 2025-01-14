SOURCE = snake.c
EXE    = snake
LD     = clang

all: $(EXE)

$(EXE): 
	$(LD) $(SOURCE) -o $(EXE) -g

clean:
	-rm -f $(EXE)     

.PHONY: all default