all: default
default: g600tool

CXXFLAGS=-g
LDFLAGS=-lusb-1.0

g600tool:
	$(CXX) $(CXXFLAGS) main.cpp -o g600tool $(LDFLAGS)
