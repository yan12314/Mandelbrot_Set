CXX = g++
CXXFLAGS = -std=c++17 -Wall
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = Mandelbrot
SRCS = main.cpp ComplexPlane.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)