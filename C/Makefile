

HEADERS = KabukProjesi.h
OBJECTS = KabukProjesi.o

default: program

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

program: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program

run: program
	./program
