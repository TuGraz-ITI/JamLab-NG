all: jammer jammer-viewer

jammer-viewer: jammer-viewer.c
	$(CC) -o $@ $<

jammer: jammer.c
	$(CC) -o $@ $<

clean:
	rm -f jammer
	rm -f jammer-viewer
