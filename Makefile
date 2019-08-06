ifeq ($(CC),)
 CC := cc
endif

# TODO: I think I need shared, but I need to support fPIC with asm.S
elfenv: elfenv.c asm.S
	$(CC) -o $@ $(CFLAGS) -nostdlib -nostartfiles -shared -fPIC \
	-Wl,-z,combreloc -Wl,-z,defs \
	elfenv.c asm.S

clean:
	rm -rf elfenv
