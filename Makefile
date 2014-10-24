CFILES=shell.c process.c 
HFILES=shell.h process.h 
OFILES=shell.o process.o
CFLAGS=-g
all: shell

shell: $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o shell

%.o: %.c $(HFILES)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm *.o shell