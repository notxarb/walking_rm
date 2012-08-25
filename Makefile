walking_rm: walking_rm.c
	gcc -o walking_rm walking_rm.c

.PHONY: clean

clean:
	rm -f walking_rm