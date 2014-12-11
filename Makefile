CFLAGS=-g -O2 -Wall -Isrc -DNDEBUG $(OPTFLAGS)
LDFLAGS=$(shell curl-config --libs)

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TARGET=bin/pushoverc

# The Target Build
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f src/*.o $(TARGET)
