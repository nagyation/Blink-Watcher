CC=gcc
CFLAGS= -c -Wall 
LDFLAGS= 
SOURCES= bwatch.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bwatch


all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS)  -o $@

.c.o:
	$(CC) $< $(CFLAGS)  -o $@

install: all
	install -t /usr/bin/ $(EXECUTABLE)
	install -t /etc/systemd/system/ blink-watcher.service
	chmod 664 /etc/systemd/system/blink-watcher.service
	systemctl enable blink-watcher.service
	systemctl start blink-watcher.service
clean:
	rm $(OBJECTS) $(EXECUTABLE)
