TARGET = ./bin/av_system
C_SOURCE = ./main/main.c ./src/*.c
INCLUDE_PATH = -I./include -I./lib/libjpeg
LIBRARY_PATH = -L./lib -lfont 
LIBRARY_JPEG_PATH = -L./lib/libjpeg -ljpeg
CC = arm-linux-gcc
$(TARGET):$(C_SOURCE)
	$(CC) $^ -o $@ $(INCLUDE_PATH) $(LIBRARY_PATH) $(LIBRARY_JPEG_PATH) -Wall -pedantic -O0 -lm
clean:
	rm $(TARGET) -f