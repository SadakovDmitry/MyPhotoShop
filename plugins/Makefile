CXX = g++
CXXFLAGS = -std=c++17 -shared
LDFLAGS = -L/$(SRC_PATH) -limplementation -lsfml-graphics -lsfml-window -lsfml-system
SRC_PATH = /Users/dima/MIPT/SecondSem/MyPaint/Standard
COMMON_SRCS = #$(SRC_PATH)/api_sfm.cpp $(SRC_PATH)/api_system.cpp
PLUGINS = pencil eraser sprayer scrollbar_gor scrollbar_vert

all: $(PLUGINS:%=%.dylib)

%.dylib: %.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $(COMMON_SRCS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.dylib


#g++ -shared /Users/dima/MIPT/SecondSem/MyPaint/Standard/api_sfm.cpp /Users/dima/MIPT/SecondSem/MyPaint/Standard/api_system.cpp /Users/dima/MIPT/SecondSem/MyPaint/Standard/photoshop.cpp -o pencil.dylib pencil.cpp -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system
#g++
