all: default
default: g600tool

CXXFLAGS=-g
LDFLAGS=-Llib/libusb-1.0.21 -lusb-1.0

g600tool:
	$(CXX) $(CXXFLAGS) main.cpp -o g600tool $(LDFLAGS)
