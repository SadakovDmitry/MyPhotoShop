CXX = g++
CXXFLAGS = -std=c++17 -fsanitize=address -I/usr/local/include
LDFLAGS = -L/Users/dima/MIPT/SecondSem/MyPaint/implementation -limplementation -lsfml-graphics -lsfml-window -lsfml-system

TARGET = ps
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS) libimplementation.dylib
	$(CXX) -dynamiclib -o libimplementation.dylib photoshop.cpp my_sfm.cpp canvas.cpp plugin_example.cpp toolbar.cpp api_system.cpp -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@./$(TARGET)
	@rm -f $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)
	rm libimplementation.dylib

libimplementation.dylib:
	$(CXX) -dynamiclib -o libimplementation.dylib photoshop.cpp my_sfm.cpp canvas.cpp plugin_example.cpp toolbar.cpp api_system.cpp -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system

dylib: libimplementation.dylib
