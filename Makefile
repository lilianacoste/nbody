CC = g++
CFLAGS = -std=c++11 -O2 -Wall
TARGET = nbody

$(TARGET): nbody.cpp
    $(CC) $(CFLAGS) -o $(TARGET) nbody.cpp

clean:
    rm -f $(TARGET)
