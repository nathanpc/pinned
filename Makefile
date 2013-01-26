CXX = clang++
CXXFLAGS = -Wall -Isrc/jsoncpp
LDFLAGS = -pthread $(shell curl-config --libs)
OBJ = src/pinned.o src/request.o src/config.o src/jsoncpp/jsoncpp.o

all: pinned

pinned: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

debug: CXXFLAGS += -g3 -DDEBUG
debug: pinned

%.o: %.c
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -rf src/*.o
	rm -rf src/jsoncpp/*.o
	rm -rf pinned
