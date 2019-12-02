TARGET = imu_plot

SRC = 	cvplot.cpp main.cpp 

INCLUDE = -I./

CC = g++ 

CFLAGS = `pkg-config opencv --cflags` `pkg-config opencv --libs`\
	-O3 \
	-Wall \

OBJ = $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) $(CFLAGS) $(DEFINE) $(INCLUDE) -c $< -o $@

all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIB) $(CFLAGS)

clean:
	rm $(TARGET) $(OBJ)

