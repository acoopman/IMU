TARGET = imu
INCLUDE = -I./

CC = g++ 

CFLAGS = -O3 -Wall `pkg-config opencv --cflags` 
LIBS = `pkg-config opencv --libs`
HDRS = socket.h
SRCS = 	cvplot.cpp main.cpp socket.cpp

.cpp.o:

all: $(TARGET)	


$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) 

