LIBS=$(shell pkg-config --libs gstreamer-1.0 gtk4)
CFLAGS=$(shell pkg-config --cflags gstreamer-1.0 gtk4)

1: compile
	./main 1>log 2>&1 &\
	sleep 0.1; \
	if [ -s log ]; then \
		cat log | less; \
	fi 


compile:
	if prep main; then prep main -k; fi
	-gcc -g -fdiagnostics-color=always $(CFLAGS) -o main main.c $(LIBS) 2>log
	-if [ -s log ]; then \
		cat log | less; \
	fi 


