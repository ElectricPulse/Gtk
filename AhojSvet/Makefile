LIBS=$(shell pkg-config --libs gstreamer-1.0 gtk4)
CFLAGS=$(shell pkg-config --cflags gstreamer-1.0 gtk4)

1:
	if prep main; then prep main -k; fi
	gcc $(CFLAGS) -o main main.c $(LIBS)
	./main &
