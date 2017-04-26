all: default
default: g600tool

CXXFLAGS=-g -std=c++11 -DWIN32
LDFLAGS=-Llib/libusb-1.0.21 -lusb-1.0 -lfltk

SRC=$(wildcard *.cpp)
HEADERS=$(wildcard *.hpp)

g600tool: $(HEADERS) $(SRC)
	@echo "Building program"
	@$(CXX) $(CXXFLAGS) $(SRC) -o g600tool $(LDFLAGS)

ui.hpp: ui.fl
	@echo "Recompiling FLTK window source"
	@fluid -c ui.fl

run_fluid:
	fluid ui.fl &
