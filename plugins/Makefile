CXX = g++
CXXFLAGS = -std=c++17 -shared -I$(SRC_PATH)
LDFLAGS = -L$(SRC_PATH) -limplementation -lsfml-graphics -lsfml-window -lsfml-system
SRC_PATH = /Users/dima/MIPT/SecondSem/MyPaint/implementation
COMMON_SRCS = #$(SRC_PATH)/api_sfm.cpp $(SRC_PATH)/api_system.cpp
PLUGINS = pencil eraser sprayer scrollbar_gor scrollbar_vert blur brightness

all: $(PLUGINS:%=%.dylib)

%.dylib: %.cpp $(COMMON_SRCS)
	$(CXX) $(CXXFLAGS) $(COMMON_SRCS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.dylib
