
all: walking_rm

walking_rm:
	gcc -o bin/walking_rm src/walking_rm.c

.PHONY: clean

install:
	cp bin/walking_rm /usr/local/sbin/

uninstall:
	rm -f /usr/local/sbin/walking_rm

clean:
	rm -f bin/walking_rm
